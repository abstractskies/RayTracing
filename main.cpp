
#include "./math.hpp"
#include "./image.hpp"
#include "./world.hpp"
#include "./camera.hpp"
#include "./material.hpp"

#include <SDL2/SDL.h>

#include <thread>
#include <limits>
#include <queue>

static constexpr size_t IMAGE_WIDTH = 1280;
static constexpr size_t IMAGE_HEIGHT = 720;
static constexpr size_t RES_DIVIDER = 1;
static constexpr size_t SAMPLE_COUNT = 128;
static constexpr float ASPECT_RATIO = float(IMAGE_WIDTH) / float(IMAGE_HEIGHT);
static constexpr size_t NUM_TILE_X = 8;
static constexpr size_t NUM_TILE_Y = 10;
static constexpr size_t NUM_WORKERS = 8;

static_assert(IMAGE_WIDTH % NUM_TILE_X == 0, "all tiles must have equal dimensions");
static_assert(IMAGE_HEIGHT % NUM_TILE_Y == 0, "all tiles must have equal dimensions");

struct Tile {
	size_t xStart, yStart;
	size_t width, height;
};

Vector3f color(const Rayf& ray, Hittable& world, int depth){
	Hit hit;
	
	if( world.hit(ray, 0.001f, std::numeric_limits<float>::max(), hit) ){
		Rayf scattered;
		Vector3f attenuation;
		
		if( depth < 50 && hit.material->scatter(ray, hit, attenuation, scattered) )
			return attenuation * color(scattered, world, depth + 1);
		else
			return Vector3f{0, 0, 0};
	} else {
		const float t = .5f * (ray.direction.normalized().y + 1.f);
		return lerp(t, Vector3f{1.f, 1.f, 1.f}, Vector3f{0.5f, 0.7f, 1.0f});
	}
}

void renderTile(ImageRGBAUNorm& img, Hittable& world, const Camera& camera, const Tile& tile){
	const size_t xStart = tile.xStart;
	const size_t yStart = tile.yStart;
	const size_t width = tile.width;
	const size_t height = tile.height;
	
	for(size_t y=yStart; y < yStart + height; ++y){
		for(size_t x=xStart; x < xStart + width; ++x){
			Vector3f result = {0.f, 0.f, 0.f};
			
			for(size_t s=0; s < SAMPLE_COUNT; ++s){
				const Vector2f uv = {
					float(x + drand48()) / float(img.width()),
					float(y + drand48()) / float(img.height()),
				};
				
				const Rayf r = camera.rayFor(uv);
				result += color(r, world, 0);
			}
			
			result /= float(SAMPLE_COUNT);
			
			PixelRGBAUNorm& pixel = img.pixels()[y * img.width() + x];
			pixel.r = static_cast<uint8_t>(sqrt(result.x) * 255);
			pixel.g = static_cast<uint8_t>(sqrt(result.y) * 255);
			pixel.b = static_cast<uint8_t>(sqrt(result.z) * 255);
			pixel.a = 255;
		}
	}
}

void populateWorld(World& world){
	world.add(new Sphere(Vector3f{0, -1000, 0}, 1000, new DiffuseMaterial(Vector3f{.5, .5, .5})));
	
	for(int a=-11; a < 11; ++a){
		for(int b=-11; b < 11; ++b){
			float chooseMaterial = drand48();
			Vector3f center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
			Material *material = nullptr;
			
			if( (center - Vector3f{4.f, .2f, 0}).length() > 0.9f ){
				const Vector3f color(drand48() * drand48(), 
									 drand48() * drand48(), 
									 drand48() * drand48());
				
				material = new DiffuseMaterial(color);
			} else if( chooseMaterial < 0.95f ){
				const Vector3f color(.5f * (1 + drand48()), 
									 .5f * (1 + drand48()), 
									 .5f * (1 + drand48()));
				
				material = new MetalMaterial(color, 0.5f * drand48());
			} else {
				material = new DielectricMaterial(1.5f);
			}
			
			world.add(new Sphere(center, 0.2f, material));
		}
	}
	
	world.add(new Sphere(Vector3f{ 0, 1, 0}, 1.0f, new DielectricMaterial(1.5)));
	world.add(new Sphere(Vector3f{-4, 1, 0}, 1.0f, new DiffuseMaterial(Vector3f{.4, .2, .1})));
	world.add(new Sphere(Vector3f{ 4, 1, 0}, 1.0f, new MetalMaterial(Vector3f{.7, .6, .5}, 0)));
}

void generateTiles(std::queue<Tile>& tiles){
	const size_t tileWidth = IMAGE_WIDTH/NUM_TILE_X;
	const size_t tileHeight = IMAGE_HEIGHT/NUM_TILE_Y;
	
	for(size_t y=0; y < NUM_TILE_Y; ++y){
		for(size_t x=0; x < NUM_TILE_X; ++x){
			tiles.push({
				x * tileWidth, y * tileHeight,
				tileWidth, tileHeight,
			});
		}
	}
}

void updateWindowTitle(SDL_Window *window, size_t ms){
	static char title[100];
	
	if( ms > 0 ){
		sprintf(title, "%zums|%zux%zu@%zu", ms, IMAGE_WIDTH/RES_DIVIDER, IMAGE_HEIGHT/RES_DIVIDER, SAMPLE_COUNT);
	} else {
		sprintf(title, "%zux%zu@%zu", IMAGE_WIDTH/RES_DIVIDER, IMAGE_HEIGHT/RES_DIVIDER, SAMPLE_COUNT);
	}
	
	SDL_SetWindowTitle(window, title);
}

int main(int argc, const char * argv[]) {
	// Set up the rendering
	ImageRGBAUNorm image(IMAGE_WIDTH/RES_DIVIDER, IMAGE_HEIGHT/RES_DIVIDER);
	memset(image.pixels(), 0, sizeof(PixelRGBAUNorm) * image.width() * image.height());
	
	const Vector3f lookFrom = {13, 2, 3};
	const Vector3f lookAt = {0, 0, 0};
	const float distanceToFocus = 10;
	const float aperture = 0.1f;
	Camera camera(lookFrom, lookAt, Vector3f{0, 1, 0}, 20, ASPECT_RATIO, aperture, distanceToFocus);
	
	// Generate the world we'll render
	World world;
	populateWorld(world);
	
	// Start the window for displaying the image
	SDL_Window *window; SDL_Renderer *renderer; SDL_Texture *texture;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(IMAGE_WIDTH, IMAGE_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, IMAGE_WIDTH/RES_DIVIDER, IMAGE_HEIGHT/RES_DIVIDER);
	
	// Generate the list of tiles to render
	std::queue<Tile> tiles;
	generateTiles(tiles);
	
	// Start the worker threads
	std::thread workers[NUM_WORKERS];
	std::mutex lock;
	
	auto msStartTime = SDL_GetTicks();
	bool finished = false;
	
	for(size_t i=0; i < NUM_WORKERS; ++i){
		workers[i] = std::thread([&,i](){
			while(true){
				// Look for a tile to render
				Tile tile;
				
				{
					std::lock_guard<std::mutex> lg(lock);
					if( tiles.empty() ){
						if( !finished ){
							finished = true;
							auto msEndTime = SDL_GetTicks();
							updateWindowTitle(window, msEndTime - msStartTime);
						}
						
						return;
					}
					
					tile = tiles.front();
					tiles.pop();
				}
				
				// Render the tile
				renderTile(image, world, camera, tile);
			}
		});
	}
	
	// Update the stats in the window title
	updateWindowTitle(window, 0);
	
	for(bool running=true; running;){
		// Handle events
		SDL_Event e;
		
		while(SDL_PollEvent(&e)){
			if( e.type == SDL_QUIT )
				running = false;
			
			if( e.type == SDL_KEYDOWN ){
				switch(e.key.keysym.sym){
				}
			}
		}
		
		// Present the result on screen
		SDL_UpdateTexture(texture, nullptr, image.pixels(), static_cast<int>(image.stride()));
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopyEx(renderer, texture, nullptr, nullptr, 0, nullptr, SDL_FLIP_VERTICAL);
		SDL_RenderPresent(renderer);
	}
	
	for(size_t i=0; i < NUM_WORKERS; ++i)
		workers[i].join();
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

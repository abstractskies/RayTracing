
#include "./math.hpp"
#include "./image.hpp"
#include "./world.hpp"
#include "./camera.hpp"
#include "./material.hpp"

#include <SDL2/SDL.h>

#include <thread>
#include <limits>

static constexpr size_t IMAGE_WIDTH = 640;
static constexpr size_t IMAGE_HEIGHT = 360;
static constexpr size_t RES_DIVIDER = 1;
static constexpr size_t SAMPLE_COUNT = 128;
static constexpr float ASPECT_RATIO = float(IMAGE_WIDTH) / float(IMAGE_HEIGHT);

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

void renderFrame(ImageRGBAUNorm& img, Hittable& world, const Camera& camera){
	for(size_t y=0; y < img.height(); ++y){
		for(size_t x=0; x < img.width(); ++x){
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
	world.add(new Sphere(Vector3f{ 0.f,    0.0f, -1.f},   0.5f, new DiffuseMaterial(Vector3f{.1f, .2f, .5f})));
	world.add(new Sphere(Vector3f{ 0.f, -100.5f, -1.f}, 100.0f, new DiffuseMaterial(Vector3f{.8f, .8f, .0f})));
	world.add(new Sphere(Vector3f{ 1.f,    0.0f, -1.f},   0.5f, new MetalMaterial(Vector3f{.8f, .6f, .2f}, 0.f)));
	world.add(new Sphere(Vector3f{-1.f,    0.0f, -1.f},   0.5f, new DielectricMaterial(1.5f)));
	world.add(new Sphere(Vector3f{-1.f,    0.0f, -1.f},  -0.45f, new DielectricMaterial(1.5f)));
}

int main(int argc, const char * argv[]) {
	ImageRGBAUNorm image(IMAGE_WIDTH/RES_DIVIDER, IMAGE_HEIGHT/RES_DIVIDER);
	
	Camera camera(Vector3f{-2, 2, 1}, Vector3f{0, 0, -1}, Vector3f{0, 1, 0}, 45, ASPECT_RATIO);
	World world;
	populateWorld(world);
	
	SDL_Window *window; SDL_Renderer *renderer; SDL_Texture *texture;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(IMAGE_WIDTH, IMAGE_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, IMAGE_WIDTH/RES_DIVIDER, IMAGE_HEIGHT/RES_DIVIDER);
	
	std::thread t([&](){
		// Render the frame
		auto tStart = SDL_GetTicks();
		renderFrame(image, world, camera);
		auto tEnd = SDL_GetTicks();
		
		// Update the stats in the window title
		static char title[100];
		sprintf(title, "%dms|%zux%zu@%zu", tEnd - tStart, image.width(), image.height(), SAMPLE_COUNT);
		SDL_SetWindowTitle(window, title);
	});
	
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
	
	t.join();
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

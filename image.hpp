#ifndef image_h
#define image_h

#include "./math.hpp"

#include <cassert>

template<class Pixel> struct Image {
	Image(){}
	
	~Image(){
		reset();
	}
	
	Image(const Image& o) = delete;
	Image& operator=(const Image& o) = delete;
	
	Image(Image&& o){
		*this = std::move(o);
	}
	
	Image& operator=(Image&& o){
		reset();
		
		pixels_ = o.pixels_;
		width_ = o.width_;
		height_ = o.height_;
		
		o.pixels_ = nullptr;
		o.width_ = 0;
		o.height_ = 0;
	}
	
	Image(size_t w, size_t h){
		assign(w, h);
	}
	
	void assign(size_t width, size_t height){
		assert(width > 0 && height > 0);
		reset();
		
		pixels_ = new Pixel[width * height];
		width_ = width;
		height_ = height;
	}
	
	void reset(){
		if( pixels_ != nullptr )
			delete [] pixels_;
		
		pixels_ = nullptr;
		width_ = 0;
		height_ = 0;
	}
	
	constexpr size_t width() const noexcept { return width_; }
	constexpr size_t height() const noexcept { return height_; }
	constexpr const Pixel * pixels() const noexcept { return pixels_; }
	constexpr Pixel * pixels() noexcept { return pixels_; }
	constexpr size_t stride() const noexcept { return sizeof(Pixel) * width_; }
	
	constexpr operator bool() const noexcept { return pixels_; }
	
private:
	Pixel *pixels_ = nullptr;
	size_t width_ = 0, height_ = 0;
};

template<class T> struct PixelRGBA {
	T r, g, b, a;
};

using PixelRGBAF = PixelRGBA<float>;
using PixelRGBAUNorm = PixelRGBA<uint8_t>;
template<class T> using ImageRGBA = Image<PixelRGBA<T>>;
using ImageRGBAUNorm = Image<PixelRGBAUNorm>;
using ImageRGBAF = Image<PixelRGBAF>;

#endif /* image_h */

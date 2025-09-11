
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "dependencies.hpp"
#include "result.hpp"
#include "device.hpp"
#include "utils.hpp"

class Texture: public HResult {
	Box<IDirect3DTexture9, RELEASE(IDirect3DTexture9)> texture;
	Vec2i size;
	
public:
	Texture(const Device &device, Vec2i size);
	Texture(const Device &device, LPCTSTR file): Texture(device, file, Colors::ZERO) {}
	Texture(const Device &device, LPCTSTR file, Color keycode);
	
	Vec2i get_size() const { return size; }
	
	WRAP(IDirect3DTexture9, texture.get());
	
	// virtual ~Texture() = default;
};

#endif

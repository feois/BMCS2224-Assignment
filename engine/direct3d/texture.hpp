
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "direct3d.hpp"
#include "device.hpp"

// a texture resource
class Texture: public HResult {
	Box<IDirect3DTexture9, RELEASE(IDirect3DTexture9)> texture;
	Vec2i size;
	
public:
	// create empty texture with the specified size
	Texture(const Device &device, Vec2i size);
	// load a texture from a file
	Texture(const Device &device, LPCTSTR file): Texture(device, file, Colors::ZERO) {}
	// load a texture from a file and set the keycode color to transparent
	Texture(const Device &device, LPCTSTR file, Color keycode);
	
	// get texture size
	Vec2i get_size() const { return size; }
	
	WRAP(IDirect3DTexture9, texture.get());
};

#endif

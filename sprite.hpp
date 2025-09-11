
#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "dependencies.hpp"
#include "result.hpp"
#include "texture_rect.hpp"
#include "vec2.hpp"
#include "utils.hpp"
#include "alignment.hpp"
#include "transform.hpp"
#include <vector>
#include <functional>

class Sprite: public HResult, public Drawer {
	Box<ID3DXSprite, RELEASE(ID3DXSprite)> sprite;
	DWORD flag = D3DXSPRITE_ALPHABLEND;
	
public:
	Sprite(const Device &direct3D);
	
	Sprite& set_flag(DWORD flag) { this->flag = flag; return *this; }
	
	void begin() override { result = sprite->Begin(flag); }
	void end() override { result = sprite->End(); }
	
	Sprite& draw(const TextureRect &texture, Vec2i pos = Vec2(), Vec2i center = Vec2(), Color modulate = Colors::WHITE);
	Sprite& draw(const TextureRect &texture, const Transform &transform, Color modulate = Colors::WHITE);
	
	void lost();
	void reset();
	
	WRAP(ID3DXSprite, sprite.get());
};

#endif

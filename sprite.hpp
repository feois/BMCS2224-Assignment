
#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "dependencies.hpp"
#include "result.hpp"
#include "texture_rect.hpp"
#include "texture_locker.hpp"
#include "vec2.hpp"
#include "utils.hpp"
#include "alignment.hpp"
#include "transform.hpp"
#include <vector>
#include <functional>

class Sprite: public HResult, public Renderer {
	Box<ID3DXSprite, RELEASE(ID3DXSprite)> sprite;
	Rc<Texture> rect_texture;
	DWORD flag = D3DXSPRITE_ALPHABLEND;
	
public:
	Sprite(const Device &direct3D, Rc<Texture> rect_texture = {});
	
	Sprite& set_flag(DWORD flag) { this->flag = flag; return *this; }
	
	void begin() override { result = sprite->Begin(flag); }
	void end() override { result = sprite->End(); }
	
	Sprite& draw(const TextureRect &texture, Vec2i pos = Vec2(), Vec2i center = Vec2(), Color modulate = Colors::WHITE);
	Sprite& draw(const TextureRect &texture, const Transform &transform, Color modulate = Colors::WHITE);
	
    Sprite& draw_rect(Vec2f pos, Vec2f size, Color color = Colors::BLACK) {
        if (rect_texture) return draw(*rect_texture, Transform() + Scale(size) + Translate(pos), color);
		else return *this;
    }
	
	static Rc<Texture> create_rect_texture(const Device &device) {
		auto texture = std::make_shared<Texture>(device, Vec2(1, 1));
		TextureWriter(*texture).set_pixel(Vec2(), Colors::WHITE);
		return texture;
	}
	
	Sprite& set_rect_texture(Rc<Texture> texture) {
		rect_texture = texture;
		return *this;
	}
	
	Rc<Texture> get_rect_texture() const { return rect_texture; }
    
	void lost() override;
	void reset() override;
	
	WRAP(ID3DXSprite, sprite.get());
};
\
#endif

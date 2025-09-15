
#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "direct3d.hpp"
#include "texture_locker.hpp"
#include <engine/core/texture_rect.hpp>
#include <engine/core/transform.hpp>

// texture drawing renderer
class Sprite: public HResult, public Renderer {
	Box<ID3DXSprite, RELEASE(ID3DXSprite)> sprite;
	Rc<Texture> rect_texture;
	DWORD flag = D3DXSPRITE_ALPHABLEND;
	
public:
	// rect_texture is used to draw rectangle, create one with create_rect_texture()
	Sprite(const Device &direct3D, Rc<Texture> rect_texture = {});
	
	// set rendering flag
	Sprite& set_flag(DWORD flag) { this->flag = flag; return *this; }
	
	void begin() override;
	void end() override;
	void lost() override;
	void reset() override;
	
	// draw a texture without ransform
	Sprite& draw(const TextureRect &texture, Vec2i pos = Vec2(), Vec2i center = Vec2(), Color modulate = Colors::WHITE);
	// draw a texture with transform
	Sprite& draw(const TextureRect &texture, const Transform &transform, Color modulate = Colors::WHITE);
	
	// draw a rectangle, must have rect_texture
    Sprite& draw_rect(Vec2f pos, Vec2f size, Color color = Colors::BLACK) {
        if (rect_texture) return draw(*rect_texture, Transform() + Scale(size) + Translate(pos), color);
		else return *this;
    }
	
	// create rect_texture
	static Rc<Texture> create_rect_texture(const Device &device) {
		auto texture = std::make_shared<Texture>(device, Vec2(1, 1));
		TextureWriter(*texture).set_pixel(Vec2(), Colors::WHITE);
		return texture;
	}
	
	// set rect_texture
	Sprite& set_rect_texture(Rc<Texture> texture) {
		rect_texture = texture;
		return *this;
	}
	
	// get rect_texture
	Rc<Texture> get_rect_texture() const { return rect_texture; }
    
	WRAP(ID3DXSprite, sprite.get());
};
\
#endif

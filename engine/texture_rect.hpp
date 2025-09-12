
#ifndef TEXTURE_RECT_HPP
#define TEXTURE_RECT_HPP

#include <direct3d/texture.hpp>

struct TextureRect {
    const Texture *texture = nullptr;
    RECT rect {};
    
    TextureRect() {}
    TextureRect(const Texture &texture);
    TextureRect(const Texture &texture, const RECT &rect): texture(&texture), rect(rect) {}
    
    constexpr Vec2i size() const { return { rect.right - rect.left, rect.bottom - rect.top }; }
};

#endif

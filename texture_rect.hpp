
#ifndef TEXTURE_RECT_HPP
#define TEXTURE_RECT_HPP

#include "texture.hpp"

struct TextureRect {
    const Texture *texture = nullptr;
    RECT rect {};
    
    TextureRect() {}
    TextureRect(const Texture &texture);
    TextureRect(const Texture &texture, const RECT &rect): texture(&texture), rect(rect) {}
};

#endif

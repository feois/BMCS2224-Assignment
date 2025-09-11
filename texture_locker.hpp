
#ifndef TEXTURE_LOCKER_HPP
#define TEXTURE_LOCKER_HPP

#include "dependencies.hpp"
#include "texture.hpp"
#include "result.hpp"
#include "color.hpp"
#include "utils.hpp"

class TextureReader: public HResult {
protected:
    Box<const Texture, [](const Texture *p) { (*p)->UnlockRect(0); }> texture;
    D3DLOCKED_RECT locked_rect;
    
    TextureReader(const Texture &texture, const RECT *rect, bool read_only);
    
public:
    TextureReader(const Texture &texture, const RECT *rect = nullptr): TextureReader(texture, rect, true) {}
    
    Color get_pixel(Vec2i pos) const;
};

class TextureWriter: public TextureReader {
public:
    TextureWriter(const Texture &texture, const RECT *rect = nullptr): TextureReader(texture, rect, false) {}

    Color set_pixel(Vec2i pos, Color color) const;
};

#endif


#include "sprite_sheet.hpp"

SpriteSheet::SpriteSheet(const Texture &texture, Vec2i offset, Vec2i tile_size, Vec2i sheet_size, int tile_count): texture(&texture), offset(offset), tile_size(tile_size), sheet_size(sheet_size), tile_count(tile_count) {}

SpriteSheet SpriteSheet::with_tile_size(const Texture &texture, Vec2i tile_size, Vec2i offset) {
    auto sheet_size = (texture.get_size() - offset).divide(tile_size);
    
    return SpriteSheet(texture, offset, tile_size, sheet_size, sheet_size.mul_xy());
}

SpriteSheet SpriteSheet::with_tile_size(const Texture &texture, Vec2i tile_size, int tile_count, Vec2i offset) {
    auto sheet_size = (texture.get_size() - offset).divide(tile_size);
    
    return SpriteSheet(texture, offset, tile_size, sheet_size, tile_count);
}

SpriteSheet SpriteSheet::with_sheet_size(const Texture &texture, Vec2i sheet_size, Vec2i offset) {
    return with_sheet_size(texture, sheet_size, sheet_size.mul_xy(), offset);
}

SpriteSheet SpriteSheet::with_sheet_size(const Texture &texture, Vec2i sheet_size, int tile_count, Vec2i offset) {
    auto tile_size = (texture.get_size() - offset).divide(sheet_size);
    
    return SpriteSheet(texture, offset, tile_size, sheet_size, tile_count);
}

TextureRect SpriteSheet::tile(Vec2i pos) const {
    if (texture && pos.x >= 0 && pos.x < sheet_size.x && pos.y >= 0 && pos.y < sheet_size.y) {
        int index = pos.x + pos.y * sheet_size.x;
        
        if (index < tile_count) {
            RECT rect;
            
            auto lt = offset + pos.multiply(tile_size);
            auto rb = lt + tile_size;
            
            rect.left = lt.x;
            rect.top = lt.y;
            rect.right = rb.x;
            rect.bottom = rb.y;
            
            return TextureRect(*texture, rect);
        }
    }
    
    return TextureRect();
}

TextureRect SpriteSheet::tile(int index) const {
    return tile(Vec2i(index % sheet_size.x, index / sheet_size.x));
}

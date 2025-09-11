
#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include "texture_rect.hpp"

class SpriteSheet {
    const Texture *texture;
    Vec2i offset;
    Vec2i tile_size;
    Vec2i sheet_size;
    int tile_count;
    
public:
    SpriteSheet(const Texture &texture, Vec2i offset, Vec2i tile_size, Vec2i sheet_size, int tile_count);
    
    static SpriteSheet with_tile_size(const Texture &texture, Vec2i tile_size, Vec2i offset = Vec2i());
    static SpriteSheet with_tile_size(const Texture &texture, Vec2i tile_size, int tile_count, Vec2i offset = Vec2i());
    static SpriteSheet with_sheet_size(const Texture &texture, Vec2i sheet_size, Vec2i offset = Vec2i());
    static SpriteSheet with_sheet_size(const Texture &texture, Vec2i sheet_size, int tile_count, Vec2i offset = Vec2i());
    
    int get_tile_count() const { return tile_count; }
    Vec2i get_sheet_size() const { return sheet_size; }
    Vec2i get_tile_size() const { return tile_size; }
    Vec2i get_offset() const { return offset; }
    
    TextureRect tile(Vec2i pos) const;
    TextureRect tile(int index) const;
    TextureRect tile(int row, int col) const { return tile(Vec2i(col, row)); }
};

#endif

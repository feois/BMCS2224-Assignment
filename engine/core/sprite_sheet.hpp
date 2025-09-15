
#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include "texture_rect.hpp"

// automatically calculate a TextureRect for a sprite sheet image
class SpriteSheet {
    const Texture *texture;
    Vec2i offset;
    Vec2i tile_size;
    Vec2i sheet_size;
    int tile_count;
    
public:
    SpriteSheet(const Texture &texture, Vec2i offset, Vec2i tile_size, Vec2i sheet_size, int tile_count);
    
    // create a sprite sheet with the fixed tile size
    static SpriteSheet with_tile_size(const Texture &texture, Vec2i tile_size, Vec2i offset = {});
    // create a sprite sheet with the fixed tile size and tile count
    static SpriteSheet with_tile_size(const Texture &texture, Vec2i tile_size, int tile_count, Vec2i offset = {});
    // create a sprite sheet with the sheet size (row size and column size), automatically calculate tile size
    static SpriteSheet with_sheet_size(const Texture &texture, Vec2i sheet_size, Vec2i offset = {});
    // create a sprite sheet with the sheet size (row size and column size) and tile count, automatically calculate tile size
    static SpriteSheet with_sheet_size(const Texture &texture, Vec2i sheet_size, int tile_count, Vec2i offset = {});
    
    // get how many tiles are available
    int get_tile_count() const { return tile_count; }
    // get sheet size (row size and column size)
    Vec2i get_sheet_size() const { return sheet_size; }
    // get tile size
    Vec2i get_tile_size() const { return tile_size; }
    // get the offset of the texture
    Vec2i get_offset() const { return offset; }
    
    // get the tile with the row index and column index
    TextureRect tile(Vec2i pos) const;
    // get the tile with the index by counting from left to right and top to bottom
    TextureRect tile(int index) const;
    // get the tile with the row index and column index
    TextureRect tile(int row, int col) const { return tile(Vec2i(col, row)); }
};

#endif

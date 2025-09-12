
#include "texture_locker.hpp"

TextureReader::TextureReader(const Texture &texture, const RECT *rect, bool read_only) {
    result = texture->LockRect(0, &locked_rect, rect, read_only ? D3DLOCK_READONLY : 0);
    if (success()) this->texture = &texture;
}

Color TextureReader::get_pixel(Vec2i pos) const {
    auto p = static_cast<BYTE *>(locked_rect.pBits);
    auto offset = pos.x * sizeof(D3DCOLOR) + pos.y * locked_rect.Pitch;
    auto pixel = reinterpret_cast<D3DCOLOR *>(p + offset);
    
    return Color(*pixel);
}

Color TextureWriter::set_pixel(Vec2i pos, Color color) const {
    auto p = static_cast<BYTE *>(locked_rect.pBits);
    auto offset = pos.x * sizeof(D3DCOLOR) + pos.y * locked_rect.Pitch;
    auto pixel = reinterpret_cast<D3DCOLOR *>(p + offset);
    auto c = Color(*pixel);
    
    *pixel = color.d3d();
    
    return c;
}

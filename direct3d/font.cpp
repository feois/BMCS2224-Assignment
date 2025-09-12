
#include "font.hpp"

DWORD TextConfig::flags() const {
    DWORD flag = 0;
    
    switch (alignment.vertical) {
        case Alignment::Top: flag |= DT_TOP; break;
        case Alignment::VCenter: flag |= DT_VCENTER; break;
        case Alignment::Bottom: flag |= DT_BOTTOM; break;
    }
    
    switch (alignment.horizontal) {
        case Alignment::Left: flag |= DT_LEFT; break;
        case Alignment::Center: flag |= DT_CENTER; break;
        case Alignment::Right: flag |= DT_RIGHT; break;
    }
    
    if (single_line) flag |= DT_SINGLELINE;
    if (!clipping) flag |= DT_NOCLIP;
    if (wrapping) flag |= DT_WORDBREAK;
    
    return flag;
}

Font::Font(const Device &device, LPCTSTR name, Vec2i size, int weight, bool italic): sprite({ device }) {
    LPD3DXFONT p;
    
    result = D3DXCreateFont(
        &*device,
        size.y, size.x,
        weight,
        1,
        italic,
        DEFAULT_CHARSET,
        OUT_TT_ONLY_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        name,
        &p
    );
    font = p;
}

Vec2i Font::get_size(TStr str, Vec2i pos, TextConfig config) {
    if (config.auto_size) {
        auto t = config.transform.d3d();
        RECT rect;
        
        rect.right = (rect.left = pos.x) + config.size.x;
        rect.bottom = (rect.top = pos.y) + config.size.y;
        
        if (config.size.x == 0) rect.right = 0;
        if (config.size.y == 0) rect.bottom = 0;
        
        sprite->SetTransform(&t);
        result = font->DrawText(&*sprite, str.data(), str.size(), &rect, config.flags() | DT_CALCRECT, config.color.d3d());
        
        return Vec2i(rect.right - rect.left, rect.bottom - rect.top);
    }
    else return config.size;
}

Font& Font::write(TStr str, Vec2i pos, TextConfig config) {
    RECT rect;
    auto size = get_size(str, pos, config);
    auto t = config.transform.d3d();
    
    rect.right = (rect.left = pos.x) + size.x;
    rect.bottom = (rect.top = pos.y) + size.y;
    
    sprite->SetTransform(&t);
    result = font->DrawText(&*sprite, str.data(), str.size(), &rect, config.flags(), config.color.d3d());
    
    return *this;
}

void Font::begin() { sprite.begin(); }
void Font::end() { sprite.end(); }
void Font::lost() { result = font->OnLostDevice(); sprite.lost(); }
void Font::reset() { result = font->OnResetDevice(); sprite.reset(); }

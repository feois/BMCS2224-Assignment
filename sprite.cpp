
#include <array>
#include <strsafe.h>
#include "sprite.hpp"

Sprite::Sprite(const Device &device, Rc<Texture> rect_texture): rect_texture(rect_texture) {
    LPD3DXSPRITE p;
    result = D3DXCreateSprite(&*device, &p);
    sprite = p;
}

Sprite &Sprite::draw(const TextureRect &texture, Vec2i pos, Vec2i center, Color modulate) {
    auto posv = pos.to_vec3(), cenv = center.to_vec3();
    RECT rect = texture.rect;
    D3DXMATRIX m;
    D3DXMatrixIdentity(&m);
    sprite->SetTransform(&m);
    result = sprite->Draw(&**texture.texture, &rect, &cenv, &posv, modulate.d3d());
    return *this;
}

Sprite &Sprite::draw(const TextureRect &texture, const Transform &transform, Color modulate) {
    RECT rect = texture.rect;
    auto t = transform.d3d();
    sprite->SetTransform(&t);
    result = sprite->Draw(&**texture.texture, &rect, nullptr, nullptr, modulate.d3d());
    return *this;
}

void Sprite::lost() { result = sprite->OnLostDevice(); }
void Sprite::reset() { result = sprite->OnResetDevice(); }

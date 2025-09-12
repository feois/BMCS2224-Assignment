
#include "texture.hpp"

Texture::Texture(const Device &device, Vec2i size): size(size) {
    LPDIRECT3DTEXTURE9 p;
    
    result = device->CreateTexture(
        size.x,
        size.y,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED,
        &p,
        nullptr
    );
    
    texture = p;
}

Texture::Texture(const Device &device, LPCTSTR file, Color keycode) {
    LPDIRECT3DTEXTURE9 p;
    
    result = D3DXCreateTextureFromFileEx(
        &*device,
        file,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        keycode.d3d(),
        nullptr,
        nullptr,
        &p
    );
    
    texture = p;
    
    if (success()) {
        D3DSURFACE_DESC desc;
        
        result = texture->GetLevelDesc(0, &desc);
        size = Vec2i(desc.Width, desc.Height);
    }
}

#include "device.hpp"
#include "texture.hpp"

Device::Device(const Window &window, Vec2i size): direct3D(Direct3D()) {
    LPDIRECT3DDEVICE9 p;
    
    ZeroMemory(&param, sizeof(param));
    
    param.Windowed = true;
    param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    param.BackBufferFormat = D3DFMT_X8R8G8B8;
    param.BackBufferCount = 1;
    param.BackBufferWidth = size.x;
    param.BackBufferHeight = size.y;
    param.hDeviceWindow = window.get_handle();
    
    this->size = size;
    
    result = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window.get_handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &param, &p);
    device = p;
}

void Device::clear(Color color) const {
    if (device)
        device->Clear(0, nullptr, D3DCLEAR_TARGET, color.d3d(), 1.0f, 0);
}

void Device::present() const {
    if (device)
        device->Present(nullptr, nullptr, nullptr, nullptr);
}

Device& Device::set_fullscreen(bool fullscreen) {
    if (device && static_cast<bool>(param.Windowed) == fullscreen) {
        param.Windowed = !fullscreen;
        result = device->Reset(&param);
    }
    
    return *this;
}

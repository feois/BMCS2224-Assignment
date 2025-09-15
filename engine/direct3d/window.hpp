
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "direct3d.hpp"
#include "window_class.hpp"
#include <engine/core/vec2.hpp>

class Window: WindowCallback {
    HWND handle = nullptr;
    
    void _destroy();
    
    LRESULT window_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    
    virtual void on_destroy() {}
    virtual bool on_keydown(WPARAM, LPARAM) { return false; }
    
public:
    Window(const WindowClass &wc, LPCTSTR name, Vec2i pos, Vec2i size, int show);
    
    HWND get_handle() const { return handle; }
    Vec2i get_size() const;
    
    // convert screen coordinate to window coordinate
    Vec2i from_screen(Vec2i vec) const;
    
    // manually destroy window
    void destroy();
    
    MOVE_ONLY(Window) {
        destroy();
        MOVE(handle);
        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }
    ~Window() { destroy(); }
};

#endif

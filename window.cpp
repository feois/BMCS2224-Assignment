
#include "dependencies.hpp"
#include "window.hpp"
#include "device.hpp"
#include "utils.hpp"

Window::Window(const WindowClass &wc, LPCTSTR name, Vec2i pos, Vec2i size, int show) {
    handle = CreateWindowEx(
        0,
        wc.class_name(),
        name,
        WS_TILEDWINDOW,
        pos.x,
        pos.y,
        size.x,
        size.y,
        nullptr,
        nullptr,
        module_handle(),
        nullptr
    );
    
    SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(handle, show);
    UpdateWindow(handle);
}

LRESULT Window::window_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_KEYDOWN:
        if (on_keydown(wParam, lParam)) return 0;
        break;
    
    case WM_DESTROY:
        on_destroy();
        _destroy();
        break;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::_destroy() {
    SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
    handle = nullptr;
}

void Window::destroy() {
    if (handle) {
        HWND h = handle;
        
        on_destroy();
        _destroy();
        DestroyWindow(h);
    }
}

Vec2i Window::get_size() const {
    RECT rect;
    GetClientRect(handle, &rect);
    return Vec2i(rect.right, rect.bottom);
}

Vec2i Window::from_screen(Vec2i vec) const {
    auto point = POINT { vec.x, vec.y };
    ScreenToClient(handle, &point);
    return Vec2i(point.x, point.y);
}

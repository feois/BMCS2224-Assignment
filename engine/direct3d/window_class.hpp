
#ifndef WINDOW_CLASS_HPP
#define WINDOW_CLASS_HPP

#include "direct3d.hpp"
#include <engine/core/utils.hpp>

// an interface for window callback
class WindowCallback {
public:
    virtual LRESULT window_callback(HWND, UINT, WPARAM, LPARAM) = 0;
};

// a reusable template to create window
class WindowClass {
    WNDCLASS wc;
    
    void reset();
    
public:
    WindowClass(LPCTSTR name);
    
    LPCTSTR class_name() const { return wc.lpszClassName; }
    
    MOVE_ONLY(WindowClass) { reset(); MOVE(wc); }
    ~WindowClass() { reset(); }
};

#endif

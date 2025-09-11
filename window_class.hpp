
#ifndef WINDOW_CLASS_HPP
#define WINDOW_CLASS_HPP

#include "dependencies.hpp"
#include "utils.hpp"

class WindowCallback {
public:
    virtual LRESULT window_callback(HWND, UINT, WPARAM, LPARAM) = 0;
};

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

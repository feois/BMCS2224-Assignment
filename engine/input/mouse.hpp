
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "input.hpp"

inline RawInputDevice Mouse_init();

class Mouse: public InputDevice<Mouse_init> {
    DIMOUSESTATE state;
    ActiveState left {}, right {}, mid {};
    
    friend inline RawInputDevice Mouse_init() { return Mouse::init(); }
    
    static RawInputDevice init() { return RawInputDevice(GUID_SysMouse, &c_dfDIMouse); }
    
    void _update() override;
    
public:
    using InputDevice<Mouse_init>::InputDevice;
    
    enum MouseType {
        Left,
        Middle,
        Right,
    };
    
    // return the mouse movement
    Vec2i delta() const;
    int delta_x() const;
    int delta_y() const;
    int wheel() const; // return the mouse wheel movement
    bool left_click(bool once = true) const; // if left clicked, $once will only return true if just clicked
    bool right_click(bool once = true) const; // if right clicked, $once will only return true if just clicked
    bool middle_click(bool once = true) const; // if middle clicked, $once will only return true if just clicked
};

#endif

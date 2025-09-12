
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
    
    Vec2i delta() const;
    int delta_x() const;
    int delta_y() const;
    int wheel() const;
    bool left_click(bool once = true) const;
    bool right_click(bool once = true) const;
    bool middle_click(bool once = true) const;
};

#endif


#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "input.hpp"

inline RawInputDevice Keyboard_init();

class Keyboard: public InputDevice<Keyboard_init> {
    std::array<BYTE, 256> buffer;
    std::array<ActiveState, 256> state;
    
    friend inline RawInputDevice Keyboard_init() { return Keyboard::init(); }
    
    static RawInputDevice init() { return RawInputDevice(GUID_SysKeyboard, &c_dfDIKeyboard); }
    
    void _update() override;
    
public:
    using InputDevice<Keyboard_init>::InputDevice;
    
    bool key_pressed(BYTE key) const;
    bool key_just_pressed(BYTE key) const;
};

#endif

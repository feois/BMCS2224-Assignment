
#include "keyboard.hpp"

void Keyboard::_update() {
    result = raw_device()->GetDeviceState(buffer.size(), buffer.data());
    
    if (success()) {
        for (size_t i = 0; i < buffer.size(); i++) {
            if (buffer[i] & 0x80) state[i]++;
            else state[i]--;
        }
    }
}

bool Keyboard::key_pressed(BYTE key) const { return state[key] != ActiveState::Inactive; }
bool Keyboard::key_just_pressed(BYTE key) const { return state[key] == ActiveState::JustActive; }

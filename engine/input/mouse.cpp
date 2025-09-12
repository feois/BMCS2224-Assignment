
#include "mouse.hpp"

static bool is_active(const ActiveState &state, bool just) {
    return just ? state == ActiveState::JustActive : state != ActiveState::Inactive;
}

void Mouse::_update() {
    result = raw_device()->GetDeviceState(sizeof(state), &state);
    
    if (state.rgbButtons[0] & 0x80) left++;
    else left--;
    
    if (state.rgbButtons[1] & 0x80) right++;
    else right--;
    
    if (state.rgbButtons[2] & 0x80) mid++;
    else mid--;
}

Vec2i Mouse::delta() const { return Vec2i(delta_x(), delta_y()); }
int Mouse::delta_x() const { return state.lX; }
int Mouse::delta_y() const { return state.lY; }
int Mouse::wheel() const { return state.lZ; }
bool Mouse::left_click(bool once) const { return is_active(left, once); }
bool Mouse::right_click(bool once) const { return is_active(right, once); }
bool Mouse::middle_click(bool once) const { return is_active(mid, once); }

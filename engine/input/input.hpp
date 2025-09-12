
#ifndef INPUT_HPP
#define INPUT_HPP

#include <dinput.h>
#include <engine/core/utils.hpp>
#include <engine/direct3d/window.hpp>

Vec2i get_mouse_pos();

Rc<IDirectInput8> DirectInput();

enum class ActiveState {
    Inactive,
    JustActive,
    Active,
};

inline ActiveState& operator ++(ActiveState &state) {
    switch (state) {
        case ActiveState::Inactive: state = ActiveState::JustActive; break;
        case ActiveState::JustActive: state = ActiveState::Active; break;
        case ActiveState::Active: break;
    }
    
    return state;
}

inline ActiveState& operator --(ActiveState &state) {
    return state = ActiveState::Inactive;
}

inline ActiveState operator ++(ActiveState &state, int) {
    auto s = state;
    ++state;
    return s;
}

inline ActiveState operator --(ActiveState &state, int) {
    auto s = state;
    --state;
    return s;
}

// unacquired input device
class RawInputDevice: public HResult {
    Rc<IDirectInput8> direct_input;
    Box<IDirectInputDevice8, RELEASE(IDirectInputDevice8)> device;
    bool acquired = false;
    
public:
    RawInputDevice(REFGUID guid, LPCDIDATAFORMAT format);
    
    bool acquire(const Window &window, bool background, bool exclusive);
    void unacquire();
    
    WRAP(IDirectInputDevice8, device.get());
    
    virtual ~RawInputDevice() = default;
};

// acquired input device
template<RawInputDevice (&Initializer)()>
class InputDevice: public HResult {
    Rc<RawInputDevice> device;
    const Window *window;
    bool background;
    bool exclusive;
    bool acquired;
    
    static Rc<RawInputDevice> device_rc() {
        SINGLETON(
            RawInputDevice,
            []() { return new RawInputDevice(Initializer()); },
            [](RawInputDevice *device) { delete device; }
        );
    }
    
protected:
    LPDIRECTINPUTDEVICE8 raw_device() const { return acquired ? &**device : nullptr; }
    virtual void _update() = 0;

public:
    InputDevice(const Window &window, bool background, bool exclusive)
    : device(device_rc()), window(&window), background(background), exclusive(exclusive), acquired(false)
    { retry(); }

    InputDevice& retry() {
        if (device && !acquired) acquired = device->acquire(*window, background, exclusive);
        
        return *this;
    }
    
    InputDevice& update() {
        if (acquired) {
            _update();

            if (failed()) {
                device->unacquire();
                acquired = false;
            }
        }

        return *this;
    }
    
    operator bool() const { return acquired; }
    ~InputDevice() { if (acquired) device->unacquire(); acquired = false; }
};

#endif

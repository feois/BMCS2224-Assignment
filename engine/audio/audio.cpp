
#include "audio.hpp"
#include "channel.hpp"
#include "sound.hpp"

bool FmodResult::failed() const { return !success(); }
bool FmodResult::success() const { return result == FMOD_OK; }
void FmodResult::clear_result() { result = FMOD_OK; }
FMOD_RESULT FmodResult::get_result() const { return result; }

Fmod::Fmod(int max_channels, FMOD_INITFLAGS flags, void *extra_driver_data, float virtual_volume) {
    FMOD_ADVANCEDSETTINGS settings = {};
    settings.cbSize = sizeof(settings);
    
    settings.vol0virtualvol = virtual_volume;
    
    FMOD_SYSTEM *p;
    
    result = FMOD_System_Create(&p, FMOD_VERSION);
    system = p;
    if (system && success()) result = FMOD_System_Init(&*system, max_channels, flags, extra_driver_data);
    if (system && success()) result = FMOD_System_SetAdvancedSettings(&*system, &settings);
    if (failed()) system = {};
}

Fmod& Fmod::close() {
    if (system) result = FMOD_System_Close(&*system);
    
    return *this;
}

Fmod& Fmod::update() {
    if (system) result = FMOD_System_Update(&*system);
    
    return *this;
}

Channel Fmod::channel(const Sound &sound) {
    auto c = Channel();
    FMOD_CHANNEL *p;
    
    c.result = FMOD_System_PlaySound(&*system, &*sound.sound, nullptr, true, &p);
    c.channel = p;
    
    return c;
}

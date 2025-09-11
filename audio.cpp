
#include "audio.hpp"

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

Sound::Sound(Fmod &fmod, const char *name_or_data, FMOD_MODE sound_mode, FMOD_MODE open_mode, FMOD_CREATESOUNDEXINFO *info) {
    FMOD_SOUND *p;
    result = FMOD_System_CreateSound(&*fmod.system, name_or_data, open_mode, info, &p);
    sound = p;
    if (sound && success()) result = FMOD_Sound_SetMode(&*sound, sound_mode);
}

Channel& Channel::update() {
    FMOD_BOOL playing = false;
    
    if (channel) result = FMOD_Channel_IsPlaying(&*channel, &playing);
    if (!playing) channel = nullptr;
    
    return *this;
}

Channel& Channel::stop() {
    if (channel) result = FMOD_Channel_Stop(&*channel);
    
    channel = nullptr;
    
    return *this;
}

bool Channel::is_playing() { return !is_paused(); }

bool Channel::is_paused() {
    FMOD_BOOL paused;
    
    if (channel) result = FMOD_Channel_GetPaused(&*channel, &paused);
    
    return paused;
}

Channel& Channel::play() {
    if (channel) result = FMOD_Channel_SetPaused(&*channel, false);
    
    return *this;
}

Channel& Channel::pause() {
    if (channel) result = FMOD_Channel_SetPaused(&*channel, true);
    
    return *this;
}

Channel& Channel::pan(float pan) {
    if (channel) result = FMOD_Channel_SetPan(&*channel, pan);
    
    return *this;
}

float Channel::volume() {
    float volume;
    
    if (channel) result = FMOD_Channel_GetVolume(&*channel, &volume);
    
    return volume;
}

Channel& Channel::set_volume(float volume) {
    if (channel) result = FMOD_Channel_SetVolume(&*channel, volume);
    
    return *this;
}

Channel& Channel::mute() {
    if (channel) result = FMOD_Channel_SetMute(&*channel, true);
    
    return *this;
}

Channel& Channel::unmute() {
    if (channel) result = FMOD_Channel_SetMute(&*channel, false);
    
    return *this;
}

bool Channel::is_muted() {
    FMOD_BOOL muted;
    
    if (channel) result = FMOD_Channel_GetMute(&*channel, &muted);
    
    return muted;
}

float Channel::speed() {
    float speed;
    
    if (channel) result = FMOD_Channel_GetPitch(&*channel, &speed);
    
    return speed;
}

Channel& Channel::set_speed(float speed) {
    if (channel) result = FMOD_Channel_SetPitch(&*channel, speed);
    
    return *this;
}

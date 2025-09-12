
#include "channel.hpp"

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


#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "audio.hpp"

class Channel: public FmodResult {
    Box<FMOD_CHANNEL, []([[maybe_unused]] FMOD_CHANNEL* p) {}> channel;
    bool muted = false;
    
    friend Fmod;
    
    Channel() {}
    
public:
    Channel& update();
    Channel& stop();

    bool is_playing();
    bool is_paused();
    Channel& play();
    Channel& pause();
    
    float volume();
    Channel& set_volume(float volume);
    
    Channel& pan(float pan);
    
    bool is_muted();
    Channel& mute();
    Channel& unmute();
    
    float speed();
    Channel& set_speed(float speed);
    
    template<FMOD_TIMEUNIT S, FMOD_TIMEUNIT E = S>
    Channel& loop(unsigned int start, unsigned int end, int count = -1) {
        if (channel) result = FMOD_Channel_SetLoopPoints(&*channel, start, S, end, E);
        if (channel && success()) result = FMOD_Channel_SetLoopCount(&*channel, count);
        
        return *this;
    }

    operator bool() const { return channel; }
};

#endif

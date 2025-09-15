
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "audio.hpp"

// control an audio, create with Fmod::channel
class Channel: public FmodResult {
    Box<FMOD_CHANNEL, []([[maybe_unused]] FMOD_CHANNEL* p) {}> channel;
    bool muted = false;
    
    friend Fmod;
    
public:
    Channel() {}
    
    // update channel status
    Channel& update();
    // stop playing and destroy this channel
    Channel& stop();

    // return true if unpaused
    bool is_playing();
    // return true if paused
    bool is_paused();
    // unpause if paused
    Channel& play();
    // pause if unpaused
    Channel& pause();
    
    // return volume (0 to 1)
    float volume();
    // set volume (0 to 1)
    Channel& set_volume(float volume);
    
    // set pan (-1 to +1, -1 for left, +1 for right)
    Channel& pan(float pan);
    
    // return true if muted
    bool is_muted();
    // mute if unmuted
    Channel& mute();
    // unmute if muted
    Channel& unmute();
    
    // get the speed the audio is playing
    float speed();
    // set the speed the audio is playing
    Channel& set_speed(float speed);
    
    // loop from a point to another point
    template<FMOD_TIMEUNIT S, FMOD_TIMEUNIT E = S>
    Channel& loop(unsigned int start, unsigned int end, int count = -1) {
        if (channel) result = FMOD_Channel_SetLoopPoints(&*channel, start, S, end, E);
        if (channel && success()) result = FMOD_Channel_SetLoopCount(&*channel, count);
        
        return *this;
    }

    operator bool() const { return channel; }
};

#endif

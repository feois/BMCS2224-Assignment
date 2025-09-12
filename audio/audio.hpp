
#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <fmod.hpp>
#include <engine/result.hpp>
#include <engine/utils.hpp>

class Fmod;
class Sound;
class Channel;

class FmodResult: public Result {
protected:
    FMOD_RESULT result = FMOD_OK;
    
public:
	bool failed() const override;
	bool success() const override;
	void clear_result() override;
    FMOD_RESULT get_result() const;
};

class Fmod: public FmodResult {
    Box<FMOD_SYSTEM, [](FMOD_SYSTEM* p) { FMOD_System_Release(p); }> system;
    
    friend Sound;
    
public:
    Fmod(int max_channels, FMOD_INITFLAGS flags = FMOD_INIT_VOL0_BECOMES_VIRTUAL, void *extra_driver_data = nullptr, float virtual_volume = 0.001f);
    
    Fmod &close();
    
    Fmod &update();
    
    Channel channel(const Sound &sound);
};

class Sound: public FmodResult {
    Box<FMOD_SOUND, [](FMOD_SOUND* p) { FMOD_Sound_Release(p); }> sound = nullptr;
    
    friend Fmod;
    
    Sound(Fmod &fmod, const char *name_or_data, FMOD_MODE sound_mode, FMOD_MODE open_mode, FMOD_CREATESOUNDEXINFO *info);
    
public:
    Sound(Fmod &fmod, const char *name_or_data, FMOD_MODE sound_mode = FMOD_DEFAULT, FMOD_MODE open_mode = FMOD_DEFAULT)
    : Sound(fmod, name_or_data, sound_mode, open_mode, nullptr) {}
    
    Sound(Fmod &fmod, const char *name_or_data, FMOD_MODE open_mode, FMOD_CREATESOUNDEXINFO info, FMOD_MODE sound_mode = FMOD_DEFAULT)
    : Sound(fmod, name_or_data, sound_mode, open_mode, &info) {}
    
    template<FMOD_TIMEUNIT S, FMOD_TIMEUNIT E = S>
    Sound& loop(unsigned int start, unsigned int end, int count = -1) {
        if (sound) result = FMOD_Sound_SetLoopPoints(&*sound, start, S, end, E);
        if (sound && success()) result = FMOD_Sound_SetLoopCount(&*sound, count);
        
        return *this;
    }
};

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

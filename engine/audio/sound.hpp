
#ifndef SOUND_HPP
#define SOUND_HPP

#include "audio.hpp"

class Sound: public FmodResult {
    Box<FMOD_SOUND, [](FMOD_SOUND* p) { FMOD_Sound_Release(p); }> sound;
    
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

#endif

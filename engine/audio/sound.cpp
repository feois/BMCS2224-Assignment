
#include "sound.hpp"

Sound::Sound(Fmod &fmod, const char *name_or_data, FMOD_MODE sound_mode, FMOD_MODE open_mode, FMOD_CREATESOUNDEXINFO *info) {
    FMOD_SOUND *p;
    result = FMOD_System_CreateSound(&*fmod.system, name_or_data, open_mode, info, &p);
    sound = p;
    if (sound && success()) result = FMOD_Sound_SetMode(&*sound, sound_mode);
}

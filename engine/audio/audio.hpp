
#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <fmod.hpp>
#include <engine/core/result.hpp>
#include <engine/core/utils.hpp>

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

#endif


#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "texture_rect.hpp"
#include <functional>

class Animation {
    int interval;
    int index = 0;
    int frame = 0;
    int length;
    std::function<TextureRect(int)> frame_function;
    bool loop;
    
public:
    Animation(int interval, int length, decltype(frame_function) f, bool loop = true): interval(interval), length(length), frame_function(f), loop(loop) {}

    TextureRect get_texture_rect() { return this->frame_function(index); }
    
    bool playing() const { return index < length; }
    bool finished() const { return !playing(); }
    
    void update(int frames) {
        frame += frames;
        index += frame / interval;
        frame %= interval;
        
        if (loop) index %= length;
        else if (index > length) index = length;
    }
    
    void reset() {
        frame = 0;
        index = 0;
    }
};

#endif

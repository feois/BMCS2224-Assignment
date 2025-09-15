
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "texture_rect.hpp"
#include <functional>

// animation frame counter
class Animation {
    int interval;
    int index = 0;
    int frame = 0;
    int length;
    std::function<TextureRect(int)> frame_function;
    
public:
    bool loop; // control if animation is looping
    
    // create an animation that advance a frame every $interval for $length frames
    // pass in a function that returns TextureRect for a given frame index
    Animation(int interval, int length, decltype(frame_function) f, bool loop = true): interval(interval), length(length), frame_function(f), loop(loop) {}

    // get current texture rect
    TextureRect get_texture_rect() { return this->frame_function(index); }
    
    // return true if animation is not finished
    bool playing() const { return index < length; }
    // return true if animation is finished
    bool finished() const { return !playing(); }
    
    // update the animation
    // pass in how many frames since last update
    void update(int frames) {
        frame += frames;
        index += frame / interval;
        frame %= interval;
        
        if (loop) index %= length;
        else if (index > length) index = length;
    }
    
    // reset animation
    void reset() {
        frame = 0;
        index = 0;
    }
};

#endif

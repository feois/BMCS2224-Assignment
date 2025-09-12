
#ifndef TIMER_HPP
#define TIMER_HPP

#include <windows.h>

class Timer {
public:
    using Time = decltype(LARGE_INTEGER::QuadPart);
    
private:
    inline static Time frequency = 0; // how many ticks in a second
    inline static bool frequency_queried = false;
    
    int fps;
    Time frame_interval; // how many ticks in a frame
    
    Time time; // timestamp
    
    Time accumulator; // subsecond ticks
    int frame_accumulator; // subsecond frames
    
    double time_delta; // time delta calculated by update()
    int frame_delta; // frame delta calculated by update()
    
public:
    Timer(int fps);
    
    int get_fps() const { return fps; }
    
    void update();
    
    double delta() const { return time_delta; }
    int frame() const { return frame_delta; }
};

#endif

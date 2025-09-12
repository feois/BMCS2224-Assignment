
#include "timer.hpp"
#include "utils.hpp"

template<typename F>
static Timer::Time convert(F f) {
    LARGE_INTEGER i;
    f(&i);
    return i.QuadPart;
}

Timer::Timer(int fps): fps(fps) {
    if (!frequency_queried) {
        frequency = convert(QueryPerformanceFrequency);
        frequency_queried = true;
    }
    
    frame_interval = frequency / fps;
    time = convert(QueryPerformanceCounter);
}

void Timer::update() {
    auto now = convert(QueryPerformanceCounter);
    auto delta = now - time;
    auto second = delta / frequency;
    auto subsecond_delta = delta % frequency;
    
    accumulator += subsecond_delta;
    
    frame_delta = static_cast<int>(second);
    
    if (accumulator >= frequency) { // accumulator exceeds a second
        accumulator -= frequency;
        frame_delta++;
    }
    
    auto subsecond_frame = static_cast<int>(accumulator / frame_interval);
    
    frame_delta *= fps; // seconds converted into frames
    frame_delta += subsecond_frame;
    frame_delta -= frame_accumulator;
    time_delta = second + (static_cast<double>(subsecond_delta) / second);
    
    frame_accumulator = subsecond_frame;
    time = now;
}

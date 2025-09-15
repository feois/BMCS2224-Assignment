
#ifndef PADDING_HPP
#define PADDING_HPP

#include "wrapper.hpp"

// surrounds child with empty space
struct Padding: public Wrapper {
    Vec2i padding;
        
    Padding(int padding, Box<UI> &&child): Padding({ padding, padding }, std::move(child)) {}
    Padding(Vec2i padding, Box<UI> &&child): Wrapper(std::move(child)), padding(padding) {}
    
    void calc_min_size(Drawer &drawer) override {
        child->calc_min_size(drawer);
        min_size = child->min_size + padding * 2;
    }
    void update(Vec2i pos, Vec2i max_size) override { child->update(pos + padding, max_size - padding * 2); UI::update(pos, max_size); }
};

#endif

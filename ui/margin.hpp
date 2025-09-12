
#ifndef MARGIN_HPP
#define MARGIN_HPP

#include "ui.hpp"

// takes up space
struct Margin: public UI {
    Margin(Vec2i size) { min_size = size; }
    
    void calc_min_size([[maybe_unused]] Drawer &drawer) override {}
    void draw([[maybe_unused]] Drawer &drawer, [[maybe_unused]] Renderer* z_order) override {}
};

#endif

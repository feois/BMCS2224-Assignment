
#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include "ui.hpp"

// wraps a single UI
struct Wrapper: public UI {
    Box<UI> child;
    
    Wrapper(Box<UI> &&child): child(std::move(child)) {}
    
    void draw(Drawer &drawer, Renderer* z_order) override { child->draw(drawer, z_order); }
    bool hover(Vec2i pos) override { return child->hover(pos); }
    bool click(Vec2i pos) override { return child->click(pos); }
};

#endif

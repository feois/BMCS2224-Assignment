
#ifndef PANEL_HPP
#define PANEL_HPP

#include "wrapper.hpp"

// draws border and background
struct Panel: public Wrapper {
    Color background;
    int border_width = 0;
    Color border = Colors::BLACK;
    
    Panel(Color background, int border_width, Color border, Box<UI> &&child)
    : Wrapper(std::move(child)), background(background), border_width(border_width), border(border) {}
    
    Panel(Color background, Box<UI> &&child): Wrapper(std::move(child)), background(background) {}
    
    constexpr Vec2i offset() const { return Vec2i(border_width, border_width); }
    
    void calc_min_size(Drawer &drawer) override {
        child->calc_min_size(drawer);
        min_size = child->min_size + offset() * 2;
    }
    void update(Vec2i pos, Vec2i max_size) override {
        child->update(pos + offset(), max_size - offset() * 2);
        UI::update(pos, max_size);
    }
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.border) {
            if (border_width > 0) drawer.border.draw_rect(get_position().to_f(), get_size().to_f(), border);
        }
        
        if (z_order == &drawer.background) {
            drawer.background.draw_rect((get_position() + offset()).to_f(), get_size().to_f(), background);
        }
        
        child->draw(drawer, z_order);
    }
};

#endif

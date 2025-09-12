
#ifndef TEXT_HPP
#define TEXT_HPP

#include "ui.hpp"

// writes text
struct Text: public UI {
    TStr text;
    TextConfig config;
    
    Text(TStr text, TextConfig config): text(text), config(config) {}
    
    void calc_min_size(Drawer &drawer) override { min_size = drawer.text.get_size(text, Vec2(), config); }
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.text) drawer.text.write(text, get_position(), config);
    }
};

#endif

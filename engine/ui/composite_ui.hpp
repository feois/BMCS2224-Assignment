
#ifndef COMPOSITE_UI_HPP
#define COMPOSITE_UI_HPP

#include "ui.hpp"

// a group of UI acting as a single UI
template<typename T>
struct CompositeUI: public UI {
    T ui;
    
    CompositeUI(T&& ui): ui(std::move(ui)) {}
    
    void calc_min_size(Drawer &drawer) override { ui.calc_min_size(drawer); min_size = ui.min_size; }
    void update(Vec2i pos, Vec2i max_size) override { ui.update(pos, max_size); UI::update(pos, max_size); }
    void draw(Drawer &drawer, Renderer* z_order) override { ui.draw(drawer, z_order); }
    bool hover(Vec2i pos) override { return ui.hover(pos); }
    bool click(Vec2i pos) override { return ui.in_rect(pos) && on_click(); }
    virtual bool on_click() { return false; }
};

#endif

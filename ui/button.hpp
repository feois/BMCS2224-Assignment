
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "composite_ui.hpp"
#include "panel.hpp"
#include "padding.hpp"

// advanced label that allows hovering and clicking, can wrap any UI
struct Button: public CompositeUI<Panel> {
    std::function<void ()> callback;
    bool enabled = true;
    bool hovered = false;
    
    Button(Color background, int border_width, Color border, Vec2i padding, Box<UI>&& ui, std::function<void ()> callback)
    : CompositeUI {
        Panel {
            background,
            border_width,
            border,
            new Padding {
                padding,
                std::move(ui),
            }
        }
    }, callback(callback) {}
    
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.background) {
            auto c = ui.background;
            if (hovered) ui.background = highlight(c);
            ui.draw(drawer, z_order);
            ui.background = c;
        }
        else ui.draw(drawer, z_order);
    }
    
    bool hover(Vec2i pos) override { return hovered = ui.in_rect(pos); }
    
    bool on_click() override {
        if (enabled) callback();
        return enabled;
    }
};

#endif

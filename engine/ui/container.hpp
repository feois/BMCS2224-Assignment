
#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "ui.hpp"

// children ui can choose to take up extra space using stretch
// all stretching children will share the extra space according to their stretch amount
struct Stretch {
    float stretch = 0;
    Box<UI> ui;
    
    Stretch(float stretch, Box<UI>&& ui): stretch(stretch), ui(std::move(ui)) {}
    Stretch(Box<UI>&& ui): ui(std::move(ui)) {}
};

// arranges children automatically
struct Container: public UI {
    float stretch_sum = 0;
    std::vector<Stretch> children {};
    
    Container(std::vector<Stretch> &&children): children(std::move(children)) {}
    
    template<typename... Args>
    Container(Args&&... args) {
        children.reserve(sizeof...(args));
        (children.emplace_back(std::forward<Args>(args)), ...);
    }
    
    void draw(Drawer &drawer, Renderer* z_order) override { for (auto& child : children) child.ui->draw(drawer, z_order); }
    bool hover(Vec2i pos) override {
        for (auto& child : children)
            if (child.ui->hover(pos)) return true;
        return false;
    }
    bool click(Vec2i pos) override {
        for (auto& child : children)
            if (child.ui->click(pos)) return true;
        return false;
    }
};


#endif

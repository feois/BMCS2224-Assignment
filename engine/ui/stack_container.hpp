
#ifndef STACK_CONTAINER_HPP
#define STACK_CONTAINER_HPP

#include "ui.hpp"
#include <ranges>

// stacks children on top of each other
struct StackContainer: public UI {
    std::vector<Box<UI>> children;
    
    StackContainer(std::vector<Box<UI>> &&children): children(std::move(children)) {}
    
    template<typename... Args>
    StackContainer(Args&&... args) {
        children.reserve(sizeof...(args));
        (children.emplace_back(std::forward<Args>(args)), ...);
    }
    
    void calc_min_size(Drawer &drawer) override {
        min_size = Vec2();
        
        for (auto& child : children) {
            child->calc_min_size(drawer);
            min_size.x = std::max(min_size.x, child->min_size.x);
            min_size.y = std::max(min_size.y, child->min_size.y);
        }
    }
    
    void update(Vec2i pos, Vec2i max_size) override {
        for (auto& child : children) child->update(pos, max_size);
        UI::update(pos, max_size);
    }
    
    void draw(Drawer &drawer, Renderer* z_order) override { for (auto& child : children) child->draw(drawer, z_order); }
    bool hover(Vec2i pos) override {
        for (auto& child : std::views::reverse(children))
            if (child->hover(pos)) return true;
        return false;
    }
    bool click(Vec2i pos) override {
        for (auto& child : std::views::reverse(children))
            if (child->click(pos)) return true;
        return false;
    }
};

#endif

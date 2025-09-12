
#ifndef VCONTAINER_HPP
#define VCONTAINER_HPP

#include "container.hpp"

// arranges children vertically
struct VContainer: public Container {
    using Container::Container;

    void calc_min_size(Drawer &drawer) override {
        stretch_sum = 0;
        min_size = Vec2();
        
        for (auto& child : children) {
            child.ui->calc_min_size(drawer);
            min_size.x = std::max(min_size.x, child.ui->min_size.x);
            min_size.y += child.ui->min_size.y;
            stretch_sum += child.stretch;
        }
    }
    void update(Vec2i pos, Vec2i max_size) override {
        auto extra = max_size.y - min_size.y;
        auto p = pos;
        
        for (auto& child : children) {
            auto s = Vec2i(max_size.x, child.ui->min_size.y);
            if (child.stretch > 0) s += Vec2(0., extra * child.stretch / stretch_sum);
            child.ui->update(p, s);
            p.y += s.y;
        }
        
        UI::update(pos, max_size);
    }
};

#endif


#ifndef HCONTAINER_HPP
#define HCONTAINER_HPP

#include "container.hpp"

// arranges children horizontally
struct HContainer: public Container {
    using Container::Container;

    void calc_min_size(Drawer &drawer) override {
        stretch_sum = 0;
        min_size = Vec2();
        
        for (auto& child : children) {
            child.ui->calc_min_size(drawer);
            min_size.y = std::max(min_size.y, child.ui->min_size.y);
            min_size.x += child.ui->min_size.x;
            stretch_sum += child.stretch;
        }
    }
    void update(Vec2i pos, Vec2i max_size) override {
        auto extra = max_size.x - min_size.x;
        auto p = pos;
        
        for (auto& child : children) {
            auto s = Vec2i(child.ui->min_size.x, max_size.y);
            if (child.stretch > 0) s += Vec2(extra * child.stretch / stretch_sum, 0.);
            child.ui->update(p, s);
            p.x += s.x;
        }
        
        UI::update(pos, max_size);
    }
};

#endif

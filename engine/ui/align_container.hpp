
#ifndef ALIGN_CONTAINER_HPP
#define ALIGN_CONTAINER_HPP

#include "wrapper.hpp"

// aligns childs and limits its size
struct AlignContainer: public Wrapper {
    Alignment alignment {};
    bool vertical_align = true;
    bool horizontal_align = true;
    
    // aligns both vertically and horizontally, limiting both width and height
    AlignContainer(Alignment alignment, Box<UI> &&child): Wrapper(std::move(child)), alignment(alignment) {}
    // aligns vertically only and limits width only
    AlignContainer(Alignment::Vertical alignment, Box<UI> &&child): Wrapper(std::move(child)) {
        this->alignment.vertical = alignment;
        horizontal_align = false;
    }
    // aligns horizontally only and limits height only
    AlignContainer(Alignment::Horizontal alignment, Box<UI> &&child): Wrapper(std::move(child)) {
        this->alignment.horizontal = alignment;
        vertical_align = false;
    }
    
    void calc_min_size(Drawer &drawer) override { child->calc_min_size(drawer); min_size = child->min_size; }
    void update(Vec2i pos, Vec2i max_size) override {
        auto p = pos;
        auto s = max_size;
        
        if (vertical_align) {
            switch (alignment.vertical) {
                case Alignment::Top: break;
                case Alignment::VCenter:
                    p.y += max_size.y / 2;
                    p.y -= min_size.y / 2;
                    break;
                case Alignment::Bottom:
                    p.y += max_size.y;
                    p.y -= min_size.y;
                    break;
            }
            
            s.y = min_size.y;
        }
        
        if (horizontal_align) {
            switch (alignment.horizontal) {
                case Alignment::Left: break;
                case Alignment::Center:
                    p.x += max_size.x / 2;
                    p.x -= min_size.x / 2;
                    break;
                case Alignment::Right:
                    p.x += max_size.x;
                    p.x -= min_size.x;
                    break;
            }
            
            s.x = min_size.x;
        }
        
        child->update(p, s);
        
        UI::update(pos, max_size);
    }
};

#endif

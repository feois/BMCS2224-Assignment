
#ifndef TEXTURE_UI_HPP
#define TEXTURE_UI_HPP

#include "ui.hpp"
#include <engine/texture_rect.hpp>

// draws texture
struct TextureUI: public UI {
    TextureRect texture;
    
    TextureUI(TextureRect texture): texture(texture) {}
    
    void calc_min_size([[maybe_unused]] Drawer &drawer) override { min_size = texture.size(); }
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.texture) drawer.texture.draw(texture, get_position());
    }
};

#endif

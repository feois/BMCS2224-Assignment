
#ifndef UI_HPP
#define UI_HPP

#include <direct3d/font.hpp>
#include <functional>

class UI {
    Vec2i position {};
    Vec2i size {};
    
public:
    Vec2i min_size {};
    
    constexpr bool in_rect(Vec2i pos) const {
        return (
            pos.x >= position.x
            && pos.x <= position.x + size.x
            && pos.y >= position.y
            && pos.y <= position.y + size.y
        );
    }
    
    struct Drawer;
    
    constexpr Vec2i get_position() const { return position; }
    constexpr Vec2i get_size() const { return size; }
    
    virtual void calc_min_size(Drawer &drawer) = 0;
    virtual void update(Vec2i pos, Vec2i max_size) { position = pos; size = max_size; }
    virtual void draw(Drawer &drawer, Renderer* z_order) = 0;
    virtual bool hover([[maybe_unused]] Vec2i pos) { return false; }
    virtual bool click([[maybe_unused]] Vec2i pos) { return false; }
    
    constexpr static Color highlight(Color color) { return color.interpolate(Colors::WHITE, 0.35); }
    
    virtual ~UI() = default;
    
    struct Drawer: Renderer {
        Sprite border, background, texture;
        Font text;
        
        Drawer(std::function<Sprite()> sprite_creator, std::function<Font()> font_creator)
        : border(sprite_creator()), background(sprite_creator()), texture(sprite_creator())
        , text(font_creator()) {
            z_order = {
                &border,
                &background,
                &texture,
                &text,
            };
        }
        
        void begin() override {}
        void end() override {}
        void lost() override { for (auto& renderer : z_order) renderer->lost(); }
        void reset() override { for (auto& renderer : z_order) renderer->reset(); }
        
        void draw(UI& ui) {
            for (auto renderer : z_order) {
                renderer->begin();
                ui.draw(*this, renderer);
                renderer->end();
            }
        }
        
    private:
        std::vector<Renderer *> z_order {};
    };
};

#endif

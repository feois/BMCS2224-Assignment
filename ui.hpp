
#ifndef UI_HPP
#define UI_HPP

#include "vec2.hpp"
#include "font.hpp"

class Game;

struct UI {
    bool hover = false;
    
    virtual Vec2i position() = 0;
    virtual Vec2i size() = 0;
    virtual void draw(Game &game) = 0;
    virtual bool on_click() = 0;
    
    constexpr static Color highlight(Color color) { return color.interpolate(Colors::WHITE, 0.35); }
};

struct Label: public UI {
    #define SETTER(type, var) type var; Label& set_##var(type var) { this->var = var; return *this; }
    
    SETTER(Color, background);
    SETTER(Color, border);
    SETTER(int, border_width);
    SETTER(Vec2, pos);
    SETTER(Vec2i, padding);
    SETTER(TStr, text);
    SETTER(TextConfig, config);
    
    #undef SETTER
    
    Vec2i position() override { return pos; }
    Vec2i size() override { return config.size + padding * 2; }
    
    void update_size(Game &game);
    void anchor(Game &game, Vec2i pos) {
        update_size(game);
        set_pos(config.alignment.anchor(pos, size()));
    }
    
    void draw(Game &game) override;
    
    bool on_click() override { return false; }
};

struct Button: public Label {
    std::function<void ()> callback;
    bool enabled = true;
    
    Button(const Label &label, std::function<void ()> callback): Label(label), callback(callback) {}
    
    bool on_click() override {
        if (enabled) callback();
        return enabled;
    }
    
    void draw(Game &game) override {
        auto c = background;
        if (hover) background = highlight(background);
        Label::draw(game);
        background = c;
    }
};

#endif

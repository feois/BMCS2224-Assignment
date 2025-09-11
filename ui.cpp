
#include "ui.hpp"
#include "game.hpp"

void Label::update_size(Game &game) { config.size = game.font.get_size(text, pos); }

void Label::draw(Game &game) {
    Vec2f border_size = Vec2(border_width, border_width);
    
    update_size(game);
    
    auto sizef = size().to_f();
    
    if (border_width > 0) game.draw_rect(Game::ui_border, pos.f() - border_size, sizef + border_size * 2, border).flush();
    game.draw_rect(Game::ui_background, pos, sizef, background).flush();
    game.font.write(text, pos.i() + padding, config);
}

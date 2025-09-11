
#include "texture_rect.hpp"

TextureRect::TextureRect(const Texture &texture): texture(&texture) {
    rect.top = 0;
    rect.left = 0;
    rect.right = texture.get_size().x;
    rect.bottom = texture.get_size().y;
}


#ifndef COLOR_HPP
#define COLOR_HPP

#include "dependencies.hpp"

struct Color {
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE a;
    
    constexpr Color(): Color(0, 0, 0) {}
    constexpr Color(BYTE r, BYTE g, BYTE b, BYTE a = 255): r(r), g(g), b(b), a(a) {}
    constexpr Color(D3DCOLOR color):
        r(static_cast<BYTE>((color >> 16) & 0x0000000FF)),
        g(static_cast<BYTE>((color >> 8) & 0x000000FF)),
        b(static_cast<BYTE>(color & 0x000000FF)),
        a(static_cast<BYTE>((color >> 24) & 0x000000FF))
        {}

    constexpr static BYTE interpolate_byte(BYTE f, BYTE t, double r) {
        int d = static_cast<int>(t) - static_cast<int>(f);
        return static_cast<BYTE>(static_cast<double>(f) + d * r + 0.5);
    }
    
    constexpr Color interpolate(Color target, double f) const {
        return Color {
            interpolate_byte(r, target.r, f),
            interpolate_byte(g, target.g, f),
            interpolate_byte(b, target.b, f),
            interpolate_byte(a, target.a, f),
        };
    }
    
    constexpr D3DCOLOR d3d() const {
        return D3DCOLOR_ARGB(a, r, g, b);
    }
    
    constexpr bool operator ==(const Color &c) const { return r == c.r && g == c.g && b == c.b && a == c.a; }
    constexpr bool operator !=(const Color &c) const { return r != c.r || g != c.g || b != c.b || a != c.a; }
};

struct Colors {
    static constexpr Color ZERO = Color(0, 0, 0, 0);
    static constexpr Color BLACK = Color();
    static constexpr Color WHITE = Color(255, 255, 255);
};

#endif

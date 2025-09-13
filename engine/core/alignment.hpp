
#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

#include "vec2.hpp"

struct Alignment {
    enum Vertical { Top = -1, VCenter = 0, Bottom = 1 };
    enum Horizontal { Left = -1, Center = 0, Right = 1 };
    
    Vertical vertical = Top;
    Horizontal horizontal = Left;
    
    constexpr Alignment() {}
    
    constexpr Alignment(Vertical vertical, Horizontal horizontal): vertical(vertical), horizontal(horizontal) {}
    constexpr Alignment(Horizontal horizontal, Vertical vertical): vertical(vertical), horizontal(horizontal) {}
};

#endif

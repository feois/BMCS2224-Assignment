
#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum class Direction {
    None,
    Left,
    Right,
    Up,
    Down,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight,
};

constexpr void dir_to_vec(Direction d, int &x, int &y) {
    x = 0;
    y = 0;
    
    switch (d)
    {
    case Direction::None: break;
    case Direction::Left: x = -1; break;
    case Direction::UpLeft: x = -1; y = -1; break;
    case Direction::DownLeft: x = -1; y = 1; break;
    case Direction::Right: x = 1; break;
    case Direction::UpRight: x = 1; y = -1; break;
    case Direction::DownRight: x = 1; y = 1; break;
    case Direction::Up: y = -1; break;
    case Direction::Down: y = 1; break;
    }
}

constexpr Direction operator+(Direction a, Direction b) {
    int ax = 0, ay = 0, bx = 0, by = 0;
    
    dir_to_vec(a, ax, ay);
    dir_to_vec(b, bx, by);
    
    int x = ax + bx;
    int y = ay + by;
    
    if (x % 2 == 0) x /= 2;
    if (y % 2 == 0) y /= 2;
    
    if (x == 1 && y == 0) return Direction::Right;
    if (x == 0 && y == 1) return Direction::Down;
    if (x == 1 && y == 1) return Direction::DownRight;
    if (x == -1 && y == 0) return Direction::Left;
    if (x == -1 && y == 1) return Direction::DownLeft;
    if (x == 0 && y == -1) return Direction::Up;
    if (x == 1 && y == -1) return Direction::UpRight;
    if (x == -1 && y == -1) return Direction::UpLeft;
    
    return Direction::None;
}

constexpr Direction &operator +=(Direction &a, const Direction &b) { return a = a + b; }

#endif

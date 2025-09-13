
#ifndef PHYSICS_BODY_HPP
#define PHYSICS_BODY_HPP

#include <engine/core/vec2.hpp>

struct Collision {
    Vec2f normal {};
    float overlap = 0;
    
    constexpr operator bool() const { return overlap != 0; }
};

struct PhysicsBody {
    bool is_static = false;
    bool on_floor = false;
    
    float mass = 1;
    float elasticity = 1;
    float friction = 0;
    
    Vec2f position {};
    Vec2f velocity {};
    
    virtual Collision is_colliding(const PhysicsBody &body) const = 0;
    void collide(PhysicsBody &body);
    bool is_colliding_floor(const PhysicsBody &body, Vec2f gravity, float epsilon = 1.f);
};

struct RectBody: public virtual PhysicsBody {
    Vec2f size {};
    
    Collision is_colliding(const PhysicsBody &body) const override;
    
    enum Side {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
    };
    
    constexpr Side flip(Side side) {
        switch (side) {
            case LEFT: return RIGHT;
            case RIGHT: return LEFT;
            case TOP: return BOTTOM;
            case BOTTOM: return TOP;
        }
        return LEFT; // should not happen
    }
    
    template<Side S>
    float side() const {
        switch (S) {
            case LEFT: return position.x;
            case RIGHT: return position.x + size.x;
            case TOP: return position.y;
            case BOTTOM: return position.y + size.y;
        }
        
        return 0;
    }
    
    Vec2f center() const { return position + size / 2; }
    
    Side colliding_side(const PhysicsBody &body) const;
};

struct CircleBody: public virtual PhysicsBody {
    float radius = 0;
    
    Collision is_colliding(const PhysicsBody &body) const override;
};

#endif

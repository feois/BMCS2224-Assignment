
#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <engine/core/vec2.hpp>

struct PhysicsObject {
    Vec2f position;
    Vec2f velocity;
    
    PhysicsObject(Vec2f pos, Vec2f velocity = Vec2()): position(pos), velocity(velocity) {}
    
    virtual bool check_collision(const PhysicsObject *obj) const = 0;
};

struct Rect: public PhysicsObject {
    Vec2f size;
    
    Rect(Vec2f pos, Vec2f size): PhysicsObject(pos), size(size) {}
    
    bool check_collision(const PhysicsObject *obj) const override;
    Direction push_out(Rect &obj) const;
    
    float left() const { return position.x; }
    float right() const { return position.x + size.x; }
    float top() const { return position.y; }
    float bottom() const { return position.y + size.y; }
    Vec2f center() const { return position + size / 2; }
};

struct Circle: public PhysicsObject {
    float radius;
    
    Circle(Vec2f center, float radius, Vec2f velocity = Vec2()): PhysicsObject(center, velocity), radius(radius) {}
    
    bool check_collision(const PhysicsObject *obj) const override;
    void reflect(const Rect &rect, float elasticity = 1);
    void collide(float mass, Circle &circle, float circle_mass, float elasticity = 1);
};

#endif

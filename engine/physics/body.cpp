
#include "body.hpp"
#include <algorithm>

#undef min
#undef max

using enum RectBody::Side;

static Collision check_collision(const CircleBody &c1, const CircleBody &c2) {
    auto dist = c2.position - c1.position;
    float overlap = c1.radius + c2.radius - dist.length();
    
    return overlap > 0 ? Collision { .normal = dist.normalize(), .overlap = overlap } : Collision();
}

static Collision check_collision(const CircleBody &c, const RectBody &r) {
    Vec2f closest {
        std::clamp(c.position.x, r.side<LEFT>(), r.side<RIGHT>()),
        std::clamp(c.position.y, r.side<TOP>(), r.side<BOTTOM>()),
    };
    auto diff = closest - c.position;
    float dist = diff.length();
    float overlap = c.radius - dist;
    
    return overlap > 0 ? Collision { .normal = diff.normalize(), .overlap = overlap } : Collision();
}

static Collision check_collision(const RectBody &r, const CircleBody &c) {
    auto collision = check_collision(c, r);
    collision.normal = -collision.normal;
    return collision;
}

static Collision check_collision(const RectBody &r1, const RectBody &r2) {
    auto overlap = Vec2f {
        std::min(r1.side<RIGHT>(), r2.side<RIGHT>()) - std::max(r1.side<LEFT>(), r2.side<LEFT>()),
        std::min(r1.side<BOTTOM>(), r2.side<BOTTOM>()) - std::max(r1.side<TOP>(), r2.side<TOP>()),
    };
    
    Vec2f n {};
    
    if (overlap.x > 0 && overlap.y > 0) {
        if (overlap.x <= overlap.y) { // push out in x direction
            n.x = r2.center().x < r1.center().x ? -1.f : 1.f;
        }
        
        if (overlap.x >= overlap.y) { // push out in y direction
            n.y = r2.center().y < r1.center().y ? -1.f : 1.f;
        }
        
        n = n.multiply(overlap);
        
        auto length = n.length();
        
        return { .normal = n / length, .overlap = length };
    }
    else return {};
}

Collision RectBody::is_colliding(const PhysicsBody &body) const {
    if (dynamic_cast<const CircleBody *>(&body))
        return check_collision(*this, dynamic_cast<const CircleBody &>(body));
    
    if (dynamic_cast<const RectBody *>(&body))
        return check_collision(*this, dynamic_cast<const RectBody &>(body));
    
    return {};
}

Collision CircleBody::is_colliding(const PhysicsBody &body) const {
    if (dynamic_cast<const CircleBody *>(&body))
        return check_collision(*this, dynamic_cast<const CircleBody &>(body));
    
    if (dynamic_cast<const RectBody *>(&body))
        return check_collision(*this, dynamic_cast<const RectBody &>(body));
    
    return {};
}

static void process_collision(PhysicsBody &a, PhysicsBody &b) {
    if (a.is_static && b.is_static) return;
    if (b.is_static) {
        process_collision(b, a); // let a be static
        return;
    }
    
    auto collision = a.is_colliding(b);
    
    if (!collision) return;
    
    float total_mass, a_mass_ratio, b_mass_ratio;
    
    if (!a.is_static) {
        total_mass = a.mass + b.mass;
        if (total_mass == 0) return;
        a_mass_ratio = a.mass / total_mass;
        b_mass_ratio = b.mass / total_mass;
    }
    
    auto push = collision.normal * collision.overlap;
    
    if (a.is_static) b.position += push;
    else {
        a.position -= push * b_mass_ratio;
        b.position += push * a_mass_ratio;
    }
    
    auto velocity = b.velocity - a.velocity;
    float dot = velocity.dot(collision.normal);
    
    if (dot > 0) return; // moving apart

    auto impulse = -collision.normal * (1 + std::min(a.elasticity, b.elasticity)) * dot;

    if (a.is_static) b.velocity += impulse;
    else {
        a.velocity -= impulse * b_mass_ratio;
        b.velocity += impulse * a_mass_ratio;
    }
    
    auto tangent = collision.normal.transpose().flip_x();
    auto friction = -tangent * velocity.dot(tangent) * (a.friction + b.friction) / 2;

    if (a.is_static) b.velocity += friction;
    else {
        a.velocity -= friction * b_mass_ratio;
        b.velocity += friction * a_mass_ratio;
    }
}

void PhysicsBody::collide(PhysicsBody &body) {
    process_collision(*this, body);
}

static bool check_floor(const Collision &collision, Vec2f gravity) {
    return -collision.normal.dot(-gravity) > 0.5f;
}

bool PhysicsBody::is_colliding_floor(const PhysicsBody &body, Vec2f gravity, float epsilon) {
    if (is_static) return false;
    
    gravity = gravity.normalize();
    
    auto collision = is_colliding(body);
    
    if (collision) return check_floor(collision, gravity);
    else {
        auto p = position;
        position += gravity * epsilon;
        collision = is_colliding(body);
        position = p;
        return collision && check_floor(collision, gravity);
    }
}

// static void process_collision(RectBody &r1, RectBody &r2) {
//     if (r2.is_static) process_collision(r2, r1); // let r1 be static
    
//     auto overlap = Vec2f {
//         std::min(r1.side<RIGHT>(), r2.side<RIGHT>()) - std::max(r1.side<LEFT>(), r2.side<LEFT>()),
//         std::min(r1.side<BOTTOM>(), r2.side<BOTTOM>()) - std::max(r1.side<TOP>(), r2.side<TOP>()),
//     };
    
//     if (r1.is_static) {
//         Vec2f push_direction {};
        
//         if (overlap.x >= 0 && overlap.y >= 0) {
//             if (overlap.x <= overlap.y) { // push out in x direction
//                 push_direction.x = r2.center().x < r1.center().x ? -1 : 1;
//                 r2.velocity.x *= -r2.elasticity;
//             }
            
//             if (overlap.x >= overlap.y) { // push out in y direction
//                 push_direction.y = r2.center().y < r1.center().y ? -1 : 1;
//                 r2.velocity.y *= -r2.elasticity;
//             }
//         }
        
//         r2.position += overlap.multiply(push_direction);
//     }
//     else {
        
//     }
// }

// static void process_collision(CircleBody &c, RectBody &r) {
//     if (check_collision(c, r)) {
//         auto p = position - closest_point(*this, rect);
//         auto d = p.length();
//         auto n = p.normalize();
//         auto v = velocity.dot(n);
        
//         if (n == Vec2f()) {
//             float dl = std::abs(position.x - rect.left());
//             float dr = std::abs(position.x - rect.right());
//             float dt = std::abs(position.y - rect.top());
//             float db = std::abs(position.y + rect.top());
            
//             n = { dl < dr ? -1.f : 1.f, dt < db ? -1.f : 1.f };
//             n.normalize();
//         }
        
//         position += n * (radius - d);
//         if (v < 0) velocity -= n * (1 + elasticity) * v;
//     }
// }

// static void process_collision(RectBody &r, CircleBody &c) {
//     process_collision(c, r);
// }

// static void process_collision(CircleBody &c1, CircleBody &c2) {
//     if (check_collision(c1, c2)) {
//         auto r = radius + circle.radius;
//         auto v = position - circle.position;
// 	    auto d = v.length_squared() - r * r;
// 		auto n = v.normalize();
// 		auto vel = n.dot(velocity - circle.velocity);
		
// 		if (vel > 0) return;

//         auto f = n * ((1 + elasticity) * vel / (mass + circle_mass));
		
// 		velocity -= f * circle_mass;
// 		circle.velocity += f * mass;
        
//         auto overlap = sqrt(-d) / 2;
		
// 		position -= n * overlap;
// 		circle.position += n * overlap;
//     }
// }

// void RectBody::collide(PhysicsBody &body) {
//     if (is_static && body.is_static) return;
//     if (dynamic_cast<RectBody *>(&body)) process_collision(*this, dynamic_cast<RectBody &>(body));
//     if (dynamic_cast<CircleBody *>(&body)) process_collision(*this, dynamic_cast<CircleBody &>(body));
// }

// void CircleBody::collide(PhysicsBody &body) {
//     if (is_static && body.is_static) return;
//     if (dynamic_cast<RectBody *>(&body)) process_collision(*this, dynamic_cast<RectBody &>(body));
//     if (dynamic_cast<CircleBody *>(&body)) process_collision(*this, dynamic_cast<CircleBody &>(body));
// }

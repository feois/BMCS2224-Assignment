
#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <engine/core/vec2.hpp>
#include <engine/core/utils.hpp>
#include "body.hpp"
#include "mask.hpp"
#include <algorithm>
#include <functional>

template<MaskInt T = DefaultMaskInt>
struct MaskBody: public virtual PhysicsBody {
    Mask<T> layer;
    Mask<T> mask;
};

template<MaskInt T = DefaultMaskInt>
struct PhysicsEngine {
    std::vector<std::weak_ptr<MaskBody<T>>> bodies;
    
    Vec2f gravity_direction { 0, 1 };
    float gravity_speed = 9.8f / 60;
    
    template<typename... Args>
    PhysicsEngine(Args&&... args) {
        bodies.reserve(sizeof...(args));
        (bodies.emplace_back(std::forward<Args>(args)), ...);
    }
    
    constexpr Vec2f gravity() const { return gravity_direction * gravity_speed; }
    
    void process_bodies(std::function<void (MaskBody<T>&, MaskBody<T>&)> f) {
        for (size_t i = 0; i < bodies.size(); i++) {
            auto a = bodies[i].lock();
            
            if (!(a && a->is_active)) continue;
            
            for (size_t j = i + 1; j < bodies.size(); j++) {
                auto b = bodies[j].lock();
                
                if (!(b && b->is_active)) continue;
                
                f(*a, *b);
            }
        }
    }
    
    void process(int frame) {
        float f = static_cast<float>(frame);
        
        // process bodies and remove invalid weak_ptr at the same time
        std::erase_if(bodies, [&](auto& weak_pointer) {
            auto shared = weak_pointer.lock();
            
            if (!shared) return true;
            
            auto& body = *shared;
            
            if (body.is_active && !body.is_static) {
                if (!body.on_floor) body.velocity += gravity() * f;
                body.position += body.velocity * f;
                body.on_floor = false;
            }
            
            return false;
        });
        
        process_bodies([](auto &a, auto& b) {
            if ((a.mask * b.layer) || (a.layer * b.mask)) {
                a.collide(b);
            }
        });
        
        process_bodies([&](auto &a, auto& b) {
            if ((a.mask * b.layer) || (a.layer * b.mask)) {
                if (!a.on_floor) a.on_floor = a.is_colliding_floor(b, gravity_direction);
                if (!b.on_floor) b.on_floor = b.is_colliding_floor(a, gravity_direction);
            }
        });
    }
};

#endif

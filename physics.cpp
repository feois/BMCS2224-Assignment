
#include "physics.hpp"
#include <algorithm>

#undef min
#undef max

static Vec2f closest_point(const Circle &c, const Rect &r) {
    return {
        std::clamp(c.position.x, r.left(), r.right()),
        std::clamp(c.position.y, r.top(), r.bottom()),
    };
}

static bool check_circles(const Circle &x, const Circle &y) {
    float r = x.radius + y.radius;
    return (x.position - y.position).length_squared() <= r * r;
}

static bool check_circle_rect(const Circle &c, const Rect &r) {
    return (c.position - closest_point(c, r)).length_squared() <= c.radius * c.radius;
}

static bool check_rects(const Rect &x, const Rect &y) {
    return !(
        x.left() > y.right()
        || x.right() < y.left()
        || x.top() > y.bottom()
        || x.bottom() < y.top()
    );
}

bool Rect::check_collision(const PhysicsObject *obj) const {
    auto c = dynamic_cast<const Circle *>(obj);
    auto r = dynamic_cast<const Rect *>(obj);
    
    if (c) return check_circle_rect(*c, *this);
    if (r) return check_rects(*this, *r);
    
    return false;
}

bool Circle::check_collision(const PhysicsObject *obj) const {
    auto c = dynamic_cast<const Circle *>(obj);
    auto r = dynamic_cast<const Rect *>(obj);
    
    if (c) return check_circles(*c, *this);
    if (r) return check_circle_rect(*this, *r);
    
    return false;
}

Direction Rect::push_out(Rect &obj) const {
    auto direction = Direction::None;
    auto overlap = Vec2f {
        std::min(right(), obj.right()) - std::max(left(), obj.left()),
        std::min(bottom(), obj.bottom()) - std::max(top(), obj.top()),
    };
    
    if (overlap.x >= 0 && overlap.y >= 0) {
        if (overlap.x <= overlap.y) { // push out in x direction
            direction = obj.center().x < center().x ? Direction::Left : Direction::Right;
            obj.velocity.x = 0;
        }
        
        if (overlap.x >= overlap.y) { // push out in y direction
            direction = obj.center().y < center().y ? Direction::Up : Direction::Down;
            obj.velocity.y = 0;
        }
    }
    
    obj.position += overlap.multiply(Vec2i(direction).to_f());
    
    return direction;
}

void Circle::reflect(const Rect &rect, float elasticity) {
    if (check_collision(&rect)) {
        auto p = position - closest_point(*this, rect);
        auto d = p.length();
        auto n = p.normalize();
        auto v = velocity.dot(n);
        
        if (n == Vec2f()) {
            float dl = std::abs(position.x - rect.left());
            float dr = std::abs(position.x - rect.right());
            float dt = std::abs(position.y - rect.top());
            float db = std::abs(position.y + rect.top());
            
            n = { dl < dr ? -1.f : 1.f, dt < db ? -1.f : 1.f };
            n.normalize();
        }
        
        position += n * (radius - d);
        if (v < 0) velocity -= n * (1 + elasticity) * v;
    }
}

void Circle::collide(float mass, Circle &circle, float circle_mass, float elasticity) {
    if (check_collision(&circle)) {
        auto r = radius + circle.radius;
        auto v = position - circle.position;
	    auto d = v.length_squared() - r * r;
		auto n = v.normalize();
		auto vel = n.dot(velocity - circle.velocity);
		
		if (vel > 0) return;

        auto f = n * ((1 + elasticity) * vel / (mass + circle_mass));
		
		velocity -= f * circle_mass;
		circle.velocity += f * mass;
        
        auto overlap = sqrt(-d) / 2;
		
		position -= n * overlap;
		circle.position += n * overlap;
    }
}

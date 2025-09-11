
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "vec2.hpp"
#include "angle.hpp"

struct Rotate: public Angle {
    enum Direction {
        CLOCKWISE = -1,
        COUNTERCLOCKWISE = 1,
        LEFT = COUNTERCLOCKWISE,
        RIGHT = CLOCKWISE,
    };
    
    constexpr Rotate() {}
    
    template<Direction D>
    constexpr Rotate(Angle a): Angle(a.rad() * static_cast<int>(D)) {}
};
struct Scale: public Vec2f {
    using Vec2f::Vec2f;
    
    constexpr Scale(float scale): Vec2f(scale, scale) {}
    constexpr Scale(): Scale(1) {}
    constexpr Scale(const Vec2f &vec): Vec2f(vec) {}
};
struct Translate: public Vec2f {
    using Vec2f::Vec2f;
    
    constexpr Translate(const Vec2f &vec): Vec2f(vec) {}
};

struct Transform {
    Vec2f scale_center;
    Rotate scale_axis_rotation;
    Scale scale;
    Vec2f rotation_center;
    Rotate rotation;
    Vec2f translation_center;
    Translate translation;
    
    constexpr Transform() {}
    constexpr Transform(Vec2f scale_center, Vec2f rotation_center, Vec2f translation_center, Rotate scale_axis_rotation = Rotate())
    : scale_center(scale_center), scale_axis_rotation(scale_axis_rotation), rotation_center(rotation_center), translation_center(translation_center) {}
    constexpr Transform(Vec2f center, Rotate scale_axis_rotation = Rotate()): Transform(center, center, center, scale_axis_rotation) {}
    
    constexpr Transform operator +(const Scale &scale) const {
        auto transform = *this;
        transform.scale = transform.scale.multiply(scale);
        return transform;
    }
    constexpr Transform operator +(const Rotate &rotation) const {
        auto transform = *this;
        transform.rotation += rotation;
        return transform;
    }
    constexpr Transform operator +(const Translate &translation) const {
        auto transform = *this;
        transform.translation += translation;
        return transform;
    }
    
    constexpr Transform& operator +=(const Scale &scale) { return *this = *this + scale; }
    constexpr Transform& operator +=(const Rotate &rotation) { return *this = *this + rotation; }
    constexpr Transform& operator +=(const Translate &translation) { return *this = *this + translation; }
    
    D3DXMATRIX d3d() const {
        D3DXMATRIX m;
        auto sc = scale_center.to_vec2(),
            s = scale.to_vec2(),
            rc = rotation_center.to_vec2(),
            t = (translation - translation_center).to_vec2();
        
        D3DXMatrixTransformation2D(&m, &sc, scale_axis_rotation.rad(), &s, &rc, rotation.rad(), &t);
        
        return m;
    }
};

#endif

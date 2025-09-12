
#ifndef VEC2_HPP
#define VEC2_HPP

#include "dependencies.hpp"
#include "angle.hpp"
#include "direction.hpp"

struct Vec2i;

struct Vec2f {
    float x, y;
    
    constexpr Vec2f(): Vec2f(0, 0) {}
    constexpr Vec2f(float x, float y): x(x), y(y) {}
    
    constexpr Vec2f multiply(float x, float y) const { return multiply(Vec2f(x, y)); }
    constexpr Vec2f multiply(const Vec2f &v) const { return Vec2f(x * v.x, y * v.y); }
    constexpr Vec2f divide(float x, float y) const { return divide(Vec2f(x, y)); }
    constexpr Vec2f divide(const Vec2f &v) const { return Vec2f(x / v.x, y / v.y); }
    
    constexpr float aspect_xy() const { return x / y; }
    constexpr float aspect_yx() const { return y / x; }
    
    constexpr Vec2f transpose() const { return Vec2f(y, x); }
    
    float length_squared() const { return x * x + y * y; }
    float length() const { return sqrtf(length_squared()); }
    constexpr Vec2f normalize() const { return *this == Vec2f() ? Vec2f() : *this / length(); }
    
    Angle to_angle() const { return Radian(atan2f(y, x)); }
    Angle to_d3d_angle() const { return flip_y().to_angle(); }
    
    constexpr float add_xy() const { return x + y; }
    constexpr float mul_xy() const { return x * y; }
    
    constexpr Vec2f flip_x() const { return Vec2f(-x, y); }
    constexpr Vec2f flip_y() const { return Vec2f(x, -y); }
    
    constexpr Vec2f from_angle(Angle angle) { return Vec2f(cosf(angle.rad()), sinf(angle.rad())); }
    constexpr Vec2f from_d3d_angle(Angle angle) { return from_angle(angle).flip_y(); }
    
    constexpr float dot(const Vec2f &v) const { return x * v.x + y * v.y; }
    
    constexpr bool in_rect(Vec2f pos, Vec2f size) const {
        return x >= pos.x && x <= pos.x + size.x && y >= pos.y && y <= pos.y + size.y;
    }
    
    constexpr bool operator ==(const Vec2f &v) const { return x == v.x && y == v.y; }
    constexpr bool operator !=(const Vec2f &v) const { return x != v.x || y != v.y; }
    
    constexpr Vec2f operator +() const { return *this; }
    constexpr Vec2f operator -() const { return Vec2f(-x, -y); }
    
    constexpr Vec2f operator +(const Vec2f &v) const { return Vec2f(x + v.x, y + v.y); }
    constexpr Vec2f operator -(const Vec2f &v) const { return Vec2f(x - v.x, y - v.y); }
    constexpr Vec2f operator *(float i) const { return Vec2f(x * i, y * i); }
    constexpr Vec2f operator /(float i) const { return Vec2f(x / i, y / i); }
    
    constexpr Vec2f &operator +=(const Vec2f &v) { return *this = *this + v; }
    constexpr Vec2f &operator -=(const Vec2f &v) { return *this = *this - v; }
    constexpr Vec2f &operator *=(float i) { return *this = *this * i; }
    constexpr Vec2f &operator /=(float i) { return *this = *this / i; }
    
    constexpr Vec2i to_i() const;
    D3DXVECTOR2 to_vec2() const { return D3DXVECTOR2(x, y); }
    D3DXVECTOR3 to_vec3() const { return D3DXVECTOR3(x, y, 0); }
    
    operator D3DXVECTOR2() const { return to_vec2(); }
    operator D3DXVECTOR3() const { return to_vec3(); }
};

struct Vec2i {
    int x, y;
    
    constexpr Vec2i(): Vec2i(0, 0) {}
    constexpr Vec2i(int x, int y): x(x), y(y) {}
    constexpr Vec2i(Direction d) { dir_to_vec(d, x, y); }
    
    constexpr Vec2i multiply(int x, int y) const { return multiply(Vec2i(x, y)); }
    constexpr Vec2i multiply(Vec2i v) const { return Vec2i(x * v.x, y * v.y); }
    constexpr Vec2i divide(int x, int y) const { return divide(Vec2i(x, y)); }
    constexpr Vec2i divide(Vec2i v) const { return Vec2i(x / v.x, y / v.y); }
    
    constexpr int add_xy() const { return x + y; }
    constexpr int mul_xy() const { return x * y; }
    
    constexpr Vec2i transpose() const { return Vec2i(y, x); }
    
    constexpr Vec2i flip_x() const { return Vec2i(-x, y); }
    constexpr Vec2i flip_y() const { return Vec2i(x, -y); }
    
    constexpr bool operator ==(const Vec2i &v) const { return x == v.x && y == v.y; }
    constexpr bool operator !=(const Vec2i &v) const { return x != v.x || y != v.y; }
    
    constexpr Vec2i operator +() const { return *this; }
    constexpr Vec2i operator -() const { return Vec2i(-x, -y); }
    
    constexpr Vec2i operator +(const Vec2i &v) const { return Vec2i(x + v.x, y + v.y); }
    constexpr Vec2i operator -(const Vec2i &v) const { return Vec2i(x - v.x, y - v.y); }
    constexpr Vec2i operator *(int i) const { return Vec2i(x * i, y * i); }
    constexpr Vec2i operator /(int i) const { return Vec2i(x / i, y / i); }
    
    constexpr Vec2i &operator +=(const Vec2i &v) { return *this = *this + v; }
    constexpr Vec2i &operator -=(const Vec2i &v) { return *this = *this - v; }
    constexpr Vec2i &operator *=(int i) { return *this = *this * i; }
    constexpr Vec2i &operator /=(int i) { return *this = *this / i; }
    
    constexpr Vec2f to_f() const { return Vec2f(static_cast<float>(x), static_cast<float>(y)); }
    D3DXVECTOR2 to_vec2() const { return to_f().to_vec2(); }
    D3DXVECTOR3 to_vec3() const { return to_f().to_vec3(); }
    
    operator D3DXVECTOR2() const { return to_vec2(); }
    operator D3DXVECTOR3() const { return to_vec3(); }
};

constexpr Vec2i Vec2f::to_i() const { return Vec2i(static_cast<int>(x), static_cast<int>(y)); }

struct Vec2 {
    Vec2i iv;
    Vec2f fv;
    
    constexpr Vec2() {}
    constexpr Vec2(Vec2i v): iv(v), fv(v.to_f()) {}
    constexpr Vec2(Vec2f v): iv(v.to_i()), fv(v) {}
    constexpr Vec2(POINT point): Vec2(Vec2i(point.x, point.y)) {}
    constexpr Vec2(int x, int y): Vec2(Vec2i(x, y)) {}
    constexpr Vec2(float x, float y): Vec2(Vec2f(x, y)) {}
    
    constexpr Vec2i i() const { return iv; }
    constexpr Vec2f f() const { return fv; }
    
    constexpr int ix() const { return iv.x; }
    constexpr int iy() const { return iv.y; }
    constexpr float fx() const { return fv.x; }
    constexpr float fy() const { return fv.y; }
    
    D3DXVECTOR2 to_vec2() const { return fv.to_vec2(); }
    D3DXVECTOR3 to_vec3() const { return fv.to_vec3(); }
    
    constexpr bool operator ==(const Vec2 &v) const { return iv == v.iv && fv == v.fv; }
    constexpr bool operator !=(const Vec2 &v) const { return iv != v.iv || fv != v.fv; }
    
    constexpr operator Vec2i() const { return iv; }
    constexpr operator Vec2f() const { return fv; }
    operator D3DXVECTOR2() const { return to_vec2(); }
    operator D3DXVECTOR3() const { return to_vec3(); }
};

#endif

















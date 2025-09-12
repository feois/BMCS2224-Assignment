
#ifndef ANGLE_HPP
#define ANGLE_HPP

#include <engine/direct3d/direct3d.hpp>

class Angle {
    float radian;
    
protected:
    constexpr Angle(float radian);
    
    static constexpr float fmod(float f, float d) { return f - d * static_cast<int>(f / d); }
    
public:
    constexpr Angle(): radian(0) {}

    static constexpr Angle right_angle() { return Angle(D3DX_PI / 2); }
    
    constexpr float rad() const { return radian; }
    constexpr float deg() const;
    constexpr float signed_rad() const;
    constexpr float signed_deg() const;
    
    constexpr bool operator ==(const Angle &angle) const { return radian == angle.radian; }
    constexpr bool operator !=(const Angle &angle) const { return radian != angle.radian; }
    
    constexpr Angle operator +() const { return *this; }
    constexpr Angle operator -() const;
    
    constexpr Angle operator +(const Angle &angle) const { return Angle(radian + angle.radian); }
    constexpr Angle operator -(const Angle &angle) const { return Angle(radian - angle.radian); }
    
    constexpr Angle& operator +=(const Angle &angle) { return *this = *this + angle; }
    constexpr Angle& operator -=(const Angle &angle) { return *this = *this - angle; }
};

class Degree: public Angle {
public:
    constexpr Degree(float degree): Angle(to_radian(degree)) {}
    
    static constexpr float to_radian(float degree) { return D3DXToRadian(degree); }
    static constexpr float normalize(float d) { return (d = fmod(d, 360)) < 0 ? d + 360 : d; }
    static constexpr float normalize_signed(float d) { return normalize(d + 180) - 180; }
};

class Radian: public Angle {
    static constexpr float PI = D3DX_PI;
    static constexpr float PI2 = PI * 2;
    
public:
    constexpr Radian(float radian): Angle(radian) {}
    
    static constexpr float to_degree(float radian) { return D3DXToDegree(radian); }
    static constexpr float normalize(float r) { return (r = fmod(r, PI2)) < 0 ? r + PI2 : r; }
    static constexpr float normalize_signed(float r) { return normalize(r + PI) - PI; }
};

constexpr Angle::Angle(float radian): radian(Radian::normalize(radian)) {}

constexpr float Angle::deg() const { return Radian::to_degree(radian); }
constexpr float Angle::signed_deg() const { return Degree::normalize_signed(Radian::to_degree(radian)); }
constexpr float Angle::signed_rad() const { return Radian::normalize_signed(radian); }

constexpr Angle Angle::operator -() const { return Radian(-radian); }

#endif


#ifndef MASK_HPP
#define MASK_HPP

#include <concepts>
#include <type_traits>

typedef unsigned int DefaultMaskInt;

template<typename T>
concept MaskInt = std::unsigned_integral<T>;

template<typename T>
concept MaskIndex = std::unsigned_integral<T> || (std::is_enum_v<T> && std::unsigned_integral<std::underlying_type_t<T>>);

template<MaskInt T>
struct Mask {
    T bits = 0;
    
    constexpr Mask() {}
    
    template<typename... Args>
    constexpr Mask(MaskIndex auto i, Args... j) {
        T k = 1;
        Mask m = from_raw(k << i);
        m += Mask(j...);
        bits = m.bits;
    }
    
    static constexpr Mask from_raw(T mask) {
        Mask m;
        m.bits = mask;
        return m;
    }
    
    constexpr Mask operator +() const { return *this; }
    constexpr Mask operator -() const { return from_raw(~bits); }
    
    constexpr operator bool() const { return bits; }
    
    constexpr Mask operator +(Mask mask) const { return from_raw(bits | mask.bits); }
    constexpr Mask operator -(Mask mask) const { return from_raw(bits & ~mask.bits); }
    constexpr Mask operator *(Mask mask) const { return from_raw(bits & mask.bits); }
    
    constexpr Mask operator +=(Mask mask) { return *this = *this + mask; }
    constexpr Mask operator -=(Mask mask) { return *this = *this - mask; }
    constexpr Mask operator *=(Mask mask) { return *this = *this * mask; }
};

#endif

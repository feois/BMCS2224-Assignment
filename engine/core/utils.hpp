
#ifndef UTILS_HPP
#define UTILS_HPP

#include <windows.h>
#include <utility>
#include <new>
#include <array>
#include <vector>
#include <span>
#include <string>
#include <string_view>
#include <memory>
#include <strsafe.h>

template<typename T>
using Rc = std::shared_ptr<T>; // alias for reference-counted pointer

using TString = std::basic_string<TCHAR>; // alias
using TStr = std::basic_string_view<TCHAR>; // alias

// specify that a class cannot be copied
#define MOVE_ONLY(class) \
    class(const class &) = delete; \
    class& operator =(const class &) = delete; \
    class& operator =(class &&other) noexcept { \
        if (this != &other) { \
            this->~class(); \
            new (this) class(std::move(other)); \
        } \
        \
        return *this; \
    } \
    class(class &&other) noexcept 

// a lambda that release a d3d9 resource
#define RELEASE(t) [](t* pointer) { pointer->Release(); }

// move a class member, use in MOVE_ONLY
#define MOVE(member) member = std::move(other.member);

// define a way to get the underlying type of a wrapper
#define WRAP(t, n) \
    t& operator *() const { return *operator->(); } \
    t* operator ->() const { return n; } \

// define a function that return a Rc that automatically destroys itself if there's no more Rc
// if no Rc exist, it creates the Rc first
#define SINGLETON(t, f, d) \
    static std::weak_ptr<t> w; \
    if (auto s = w.lock()) return s; \
    auto s = Rc<t>((f)(), (d)); \
    w = s; \
    return s; \

// format a string like sprintf
// returns a temporary string, therefore it will be destroyed by the end of statement
template<size_t FORMAT_LENGTH, typename... T>
TString format(LPCTSTR f, T&&... t) {
    size_t extra = 32;
    TString s;
    while (true) {
        size_t size = FORMAT_LENGTH + extra;
        s.reserve(size);
        s.resize(size); // resize first so internal length is equal to capacity and resize later will not overwrite
        switch (StringCchPrintf(s.data(), size + 1, f, std::forward<T>(t)...)) {
            case S_OK:
                s.resize(
                    #ifdef UNICODE
                    wcslen(s.data())
                    #else
                    strlen(s.data())
                    #endif
                );
                return s;
            case STRSAFE_E_INSUFFICIENT_BUFFER:
                extra += extra / 2;
                break;
            case STRSAFE_E_INVALID_PARAMETER: // string too long
            default: // unknown error
                exit(999);
        }
    }
}

// create an error window
template<size_t FORMAT_LENGTH, typename... T>
void error(LPCTSTR f, T&&... t) {
	MessageBox(nullptr, format<FORMAT_LENGTH>(f, std::forward<T>(t)...).data(), TEXT("Error"), MB_OK | MB_ICONERROR);
}

// create an error window and forcefully exit with the code
template<size_t FORMAT_LENGTH, typename... T>
void panic(int code, LPCTSTR f, T&&... t) {
    error<FORMAT_LENGTH>(f, std::forward<T>(t)...);
    exit(code);
}

// return the value if Result::success() returns true, otherwise panic
template<size_t FORMAT_LENGTH, typename T, typename... U>
static T panic_if_failed(T t, int code, LPCTSTR s, U&&... u) {
    if (t.failed()) panic<FORMAT_LENGTH>(code, s, std::forward<U>(u)...);
    return t;
}

#define format(f, ...) format<std::char_traits<TCHAR>::length(TEXT(f))>(TEXT(f) __VA_OPT__(,) __VA_ARGS__)
#define error(f, ...) error<std::char_traits<TCHAR>::length(TEXT(f))>(TEXT(f) __VA_OPT__(,) __VA_ARGS__)
#define panic(c, f, ...) panic<std::char_traits<TCHAR>::length(TEXT(f))>(c, TEXT(f) __VA_OPT__(,) __VA_ARGS__)
#define panic_if_failed(t, c, f, ...) panic_if_failed<sizeof(TEXT(f))>(t, c, TEXT(f) __VA_OPT__(,) __VA_ARGS__)

// early release an object
template<typename T>
void drop(T& t) { T temp(std::move(t)); }

// wraps a unique_ptr
template<typename T, void (*F)(T*) = nullptr>
class Box {
    std::unique_ptr<T, decltype(F)> object;
    
    static void default_deleter([[maybe_unused]] T* pointer) { if constexpr (!F) delete pointer; }
    static constexpr decltype(F) deleter() {
        if constexpr (F) return F;
        else return default_deleter;
    }
    
public:
    constexpr Box(): object { nullptr, deleter() } {}
    
    constexpr Box(T* object): object { object, deleter() } {}
    constexpr Box(T&& object): Box(new T(std::move(object))) {}
    
    constexpr T* get() const { return object.get(); }
    
    constexpr operator bool() const { return static_cast<bool>(object); }

    WRAP(T, object.get());
};

#endif

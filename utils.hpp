
#ifndef UTILS_HPP
#define UTILS_HPP

#include "dependencies.hpp"
#include "vec2.hpp"
#include <utility>
#include <new>
#include <array>
#include <strsafe.h>
#include <vector>
#include <span>
#include <string>
#include <string_view>

#define NO_MOVE(class) \
    class(const class &) = delete; \
    class& operator =(const class &) = delete; \
    class(class &&other) = delete; \
    class& operator =(class &&other) = delete; \

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

#define RELEASE(t) [](t* pointer) { pointer->Release(); }

#define MOVE(member) member = other.member;

#define WRAP(t, n) \
    t& operator *() const { return *operator->(); } \
    t* operator ->() const { return n; } \

#define SINGLETON(t, f, d) \
    static std::weak_ptr<t> w; \
    if (auto s = w.lock()) return s; \
    auto s = std::shared_ptr<t>((f)(), (d)); \
    w = s; \
    return s; \

constexpr size_t DEFAULT_FORMAT_BUFFER_SIZE = 256;

template<size_t BUFFER_SIZE = DEFAULT_FORMAT_BUFFER_SIZE, typename... T>
std::array<TCHAR, BUFFER_SIZE> format(LPCTSTR f, T&&... t) {
    std::array<TCHAR, BUFFER_SIZE> s {};
    StringCchPrintf(s.data(), s.size(), f, std::forward<T>(t)...);
    return s;
}

template<size_t BUFFER_SIZE = DEFAULT_FORMAT_BUFFER_SIZE, typename... T>
void error(LPCTSTR f, T&&... t) {
	MessageBox(nullptr, format(f, std::forward<T>(t)...).data(), TEXT("Error"), MB_OK | MB_ICONERROR);
}

template<size_t BUFFER_SIZE = DEFAULT_FORMAT_BUFFER_SIZE, typename... T>
void panic(int code, LPCTSTR f, T&&... t) {
    error(f, std::forward<T>(t)...);
    exit(code);
}

template<typename T, typename... U>
static T panic_if_failed(T t, int code, LPCTSTR s, U&&... u) {
    if (t.failed()) (panic)(code, s, std::forward<U>(u)...);
    return t;
}

#define format(f, ...) format(TEXT(f) __VA_OPT__(,) __VA_ARGS__)
#define error(f, ...) error(TEXT(f) __VA_OPT__(,) __VA_ARGS__)
#define panic(c, f, ...) panic(c, TEXT(f) __VA_OPT__(,) __VA_ARGS__)
#define panic_if_failed(t, c, f, ...) panic_if_failed(t, c, TEXT(f) __VA_OPT__(,) __VA_ARGS__)

template<typename T>
void drop(T& t) { T temp(std::move(t)); }

template<typename T>
using Rc = std::shared_ptr<T>;

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
    constexpr Box(T&& object): object { std::make_unique(std::move(object)), deleter() } {}
    
    constexpr T* get() const { return object.get(); }
    
    constexpr operator bool() const { return static_cast<bool>(object); }

    WRAP(T, object.get());
};

template <typename T, std::size_t N, typename F, std::size_t... I>
constexpr std::array<T, N> array_from_fn_impl(F&& f, std::index_sequence<I...>) {
    return { { (static_cast<void>(I), f())... } };
}

template <typename T, std::size_t N, typename F>
constexpr std::array<T, N> array_from_fn(F&& f) {
    return array_from_fn_impl<T, N>(std::forward<F>(f), std::make_index_sequence<N>{});
}

using TString = std::basic_string<TCHAR>;
using TStr = std::basic_string_view<TCHAR>;

template<typename T, size_t N>
std::vector<T> own(const std::span<T> array) { return { array.begin(), array.end() }; }

inline TString own_str(const std::span<TCHAR> str) { return { str.data() }; }

#endif

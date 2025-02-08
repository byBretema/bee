#pragma once

#include "timer.hpp"

#include <cstdint>
#include <limits>
#include <chrono>

//=========================================================
//== CONCAT
//=========================================================

#define __AC_CONCAT2(l, r) l##r
#define __AC_CONCAT1(l, r) __AC_CONCAT2(l, r)
#define AC_CONCAT(l, r)    __AC_CONCAT1(l, r)

//=========================================================
//== DEFER
//=========================================================

#define __detail_ac_defer_ref(fn)  const auto AC_CONCAT(defer__, __LINE__) = ac::detail::Defer([&]() { fn; });
#define __detail_ac_defer_copy(fn) const auto AC_CONCAT(defer__, __LINE__) = ac::detail::Defer([=]() { fn; });

#define ac_defer(fn) __detail_ac_defer_ref(fn)
#ifndef defer
#    define defer(fn) __detail_ac_defer_ref(fn)
#endif

#define ac_deferc(fn) __detail_ac_defer_copy(fn)
#ifndef deferc
#    define deferc(fn) __detail_ac_defer_copy(fn)
#endif

namespace ac::detail
{
template<typename T>  // <- Shout-out to https://github.com/javiersalcedopuyo
class Defer
{
public:
    Defer() = delete;
    Defer(T fn) : fn(fn) {}
    ~Defer() { fn(); }

private:
    const T fn;
};

//=========================================================
//== OTHER MACROS
//=========================================================

#define ac_bind(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define ac_bit(x)   (1 << x)

//=========================================================
//== ELAPSED TIMER
//=========================================================

class ElapsedTimer
{
    using Clock = std::chrono::high_resolution_clock;

public:
    inline void reset()
    {
        m_ref   = Clock::now();
        m_valid = true;
    }

    inline i64  elapsed_s() const { return elapsed<std::chrono::seconds>(); }
    inline i64  elapsed_ms() const { return elapsed<std::chrono::milliseconds>(); }
    inline i64  elapsed_us() const { return elapsed<std::chrono::microseconds>(); }
    inline i64  elapsed_ns() const { return elapsed<std::chrono::nanoseconds>(); }
    inline bool is_valid() const { return m_valid; }

private:
    Clock::time_point m_ref   = Clock::now();
    bool              m_valid = false;

    template<typename TimeUnit>
    inline i64 elapsed() const
    {
        return std::chrono::duration_cast<TimeUnit>(Clock::now() - m_ref).count();
    }
};
using ETimer = ElapsedTimer;


//=========================================================
//== CONSTANTS
//=========================================================

namespace ac
{

    //-------------------------------------
    // ... Time Consts
    //-------------------------------------

    inline constexpr u64 s_to_ns = 1e+9;
    inline constexpr u64 ns_to_s = 1e-9;

    inline constexpr u64 s_to_us  = 1e+6;
    inline constexpr u64 us_to_ss = 1e-6;

    inline constexpr u64 s_to_ms = 1e+3;
    inline constexpr u64 ms_to_s = 1e-3;

    inline constexpr u64 ms_to_ns = 1e+6;
    inline constexpr u64 ns_to_ms = 1e-6;

    inline constexpr u64 ms_to_us = 1e+3;
    inline constexpr u64 us_to_ms = 1e-3;

    inline constexpr u64 us_to_ns = 1e+3;
    inline constexpr u64 ns_to_us = 1e-3;


    //-------------------------------------
    // ... Numeric Aliases
    //-------------------------------------

    namespace NumericAliases
    {
        using b8 = bool;

        using i8  = int8_t;
        using i16 = int16_t;
        using i32 = int32_t;
        using i64 = int64_t;

        using u8  = uint8_t;
        using u16 = uint16_t;
        using u32 = uint32_t;
        using u64 = uint64_t;

        using f32 = float;
        using f64 = double;

        using usize = size_t;
        using isize = ptrdiff_t;

        inline constexpr i8  i8_max  = std::numeric_limits<i8>::max();
        inline constexpr i16 i16_max = std::numeric_limits<i16>::max();
        inline constexpr i32 i32_max = std::numeric_limits<i32>::max();
        inline constexpr i64 i64_max = std::numeric_limits<i64>::max();

        inline constexpr u8  u8_max  = std::numeric_limits<u8>::max();
        inline constexpr u16 u16_max = std::numeric_limits<u16>::max();
        inline constexpr u32 u32_max = std::numeric_limits<u32>::max();
        inline constexpr u64 u64_max = std::numeric_limits<u64>::max();

        inline constexpr f32 f32_max = std::numeric_limits<f32>::max();
        inline constexpr f64 f64_max = std::numeric_limits<f64>::max();

        inline constexpr usize usize_max = std::numeric_limits<usize>::max();
        inline constexpr isize isize_max = std::numeric_limits<isize>::max();

        inline constexpr i8  i8_min  = std::numeric_limits<i8>::min();
        inline constexpr i16 i16_min = std::numeric_limits<i16>::min();
        inline constexpr i32 i32_min = std::numeric_limits<i32>::min();
        inline constexpr i64 i64_min = std::numeric_limits<i64>::min();

        inline constexpr u8  u8_min  = std::numeric_limits<u8>::min();
        inline constexpr u16 u16_min = std::numeric_limits<u16>::min();
        inline constexpr u32 u32_min = std::numeric_limits<u32>::min();
        inline constexpr u64 u64_min = std::numeric_limits<u64>::min();

        inline constexpr f32 f32_min = std::numeric_limits<f32>::min();
        inline constexpr f64 f64_min = std::numeric_limits<f64>::min();

        inline constexpr usize usize_min = std::numeric_limits<usize>::min();
        inline constexpr isize isize_min = std::numeric_limits<isize>::min();

        inline constexpr f32 f32_epsilon = std::numeric_limits<f32>::epsilon();
        inline constexpr f64 f64_epsilon = std::numeric_limits<f64>::epsilon();
    }  // namespace NumericAliases
    using namespace NumericAliases;

}  // namespace ac

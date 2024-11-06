#pragma once

#include <cstdint>
#include <limits>

namespace ac
{

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

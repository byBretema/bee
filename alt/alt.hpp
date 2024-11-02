#pragma once

#include "defer.hpp"
#include "numeric_aliases.hpp"
#include "timer.hpp"

#define ac_bind(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define ac_bit(x)   (1 << x)

namespace ac
{
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// ... Time Consts

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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}  // namespace ac

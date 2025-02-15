#pragma once

/* alt_cpp (ac) - v0.01

    An alternative take on cpp.
    Basically some aliases and helpers.

    Also a bunch of code that I found myself repeating between project.

    No warranty implied, use at your own risk.

    =============================================
    ! How to include
    =============================================

    -- Classic header-only stuff, add this:

    #define ALT_CPP_IMPLEMENTATION

    -- Before you include this file in *one* C++ file to create the
    implementation, something like this:

    #include ...
    #include ...
    #define ALT_CPP_IMPLEMENTATION
    #include "alt.hpp"

    =============================================
    ! Define-Based options:
    =============================================

    -- If you use fmt-lib, 'alt' will include basic fmt header files and
    expose, basic log methods: ac_info/warn/err/debug("", ...)

    #define ALT_CPP_INCLUDE_FMT

    -- If you use glm-lib, 'alt' will include basic glm header files

    #define ALT_CPP_INCLUDE_GLM

*/


//=========================================================
//== INCLUDES
//=========================================================

#include <chrono>
#include <cmath>
#include <cstdint>
#include <limits>

#include <array>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <memory>
// #include <functional>

#ifdef ALT_CPP_INCLUDE_FMT
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#endif

#ifdef ALT_CPP_INCLUDE_GLM
// #define GLM_FORCE_SSE
// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtx/string_cast.hpp> // Before of 'GLM_FORCE_INLINE'
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vec_swizzle.hpp>
#include <glm/gtx/vector_angle.hpp>
#define glmstr(x) glm::to_string(x)
#endif


//=========================================================
//== CONCAT
//=========================================================

#define __AC_CONCAT2(l, r) l##r
#define __AC_CONCAT1(l, r) __AC_CONCAT2(l, r)
#define AC_CONCAT(l, r) __AC_CONCAT1(l, r)


//=========================================================
//== DEFER
//=========================================================


#define ac_defer(fn) const auto AC_CONCAT(defer__, __LINE__) = ac::detail::Defer([&]() { fn; })
#ifndef defer
#define defer(fn) ac_defer(fn)
#else
#warning "MACRO_WARN: 'defer' is already defined using it might end in a missbehave"
#endif

#define ac_deferc(fn) const auto AC_CONCAT(defer__, __LINE__) = ac::detail::Defer([=]() { fn; })
#ifndef deferc
#define deferc(fn) ac_deferc(fn)
#else
#warning "MACRO_WARN: 'deferc' is already defined using it might end in a missbehave"
#endif

namespace ac::detail {
template <typename T> // <- Shout-Out to https://github.com/javiersalcedopuyo
class Defer {
public:
    Defer() = delete;
    Defer(T fn) : fn(fn) {}
    ~Defer() { fn(); }

private:
    const T fn;
};
} // namespace ac::detail

//=========================================================
//== LOGGING
//=========================================================

#ifdef ALT_CPP_INCLUDE_FMT
// String Builder
#define ac_fmt(msg, ...) fmt::format(msg, __VA_ARGS__)

// Logging helpers
#define ac_info(msg, ...) fmt::println("[INFO] | {}:{} | {}", __FILE__, __LINE__, ac_fmt(msg, __VA_ARGS__))
#define ac_warn(msg, ...) fmt::println("[WARN] | {}:{} | {}", __FILE__, __LINE__, ac_fmt(msg, __VA_ARGS__))
#define ac_err(msg, ...) fmt::println("[ERRO] | {}:{} | {}", __FILE__, __LINE__, ac_fmt(msg, __VA_ARGS__))
#define ac_debug(msg, ...) fmt::println("[DBUG] | {}:{} | {}", __FILE__, __LINE__, ac_fmt(msg, __VA_ARGS__))
#else
#warning "[alt_cpp] :: Using fmt-lib will improve experience (and performance) of ac_fmt/info/err/.. methods a lot."
#include <iostream>
namespace ac::detail {
std::string format(std::string msg, std::vector<std::string> const &args);
std::vector<std::string> to_stringlist();

template <typename T, typename... Args>
std::vector<std::string> to_stringlist(T &&first, Args &&...args) {
    std::ostringstream oss;
    oss << std::boolalpha << first;
    std::vector<std::string> result { oss.str() };
    std::vector<std::string> rest = to_stringlist(std::forward<Args>(args)...);
    result.insert(result.end(), rest.begin(), rest.end());
    return result;
}
} // namespace ac::detail

// String Builder
#define ac_fmt(msg, ...) ac::detail::format(msg, ac::detail::to_stringlist(__VA_ARGS__))

// Logging helpers
#define ac_info(msg, ...)                                                                                              \
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_fmt(msg, __VA_ARGS__) << "\n"
#define ac_warn(msg, ...)                                                                                              \
    std::cout << "[WARN] | " << __FILE__ << ":" << __LINE__ << " | " << ac_fmt(msg, __VA_ARGS__) << "\n"
#define ac_err(msg, ...)                                                                                               \
    std::cout << "[ERRO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_fmt(msg, __VA_ARGS__) << "\n"
#define ac_debug(msg, ...)                                                                                             \
    std::cout << "[DBUG] | " << __FILE__ << ":" << __LINE__ << " | " << ac_fmt(msg, __VA_ARGS__) << "\n"
#endif

//=========================================================
//== OTHER MACROS
//=========================================================

#define ac_bind(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define ac_bit(x) (1 << x)

#define ac_nocopy(T)                                                                                                   \
public:                                                                                                                \
    T(T const &) = delete;                                                                                             \
    T &operator=(T const &) = delete;

#define ac_nomove(T)                                                                                                   \
public:                                                                                                                \
    T(T &&) noexcept = delete;                                                                                         \
    T &operator=(T &&) noexcept = delete;

#define ac_nocopy_nomove(T) ac_nocopy(T) ac_nomove(T)

#define ac_as(T, x) static_cast<T>(x)
#ifndef as
#define as(T, x) ac_as(T, x)
#else
#warning "MACRO_WARN: 'as' is already defined using it might end in a missbehave"
#endif


//=========================================================
//== NAMESPACE
//=========================================================

namespace ac {

//-------------------------------------
// ... Numbers Aliases
//-------------------------------------

namespace TypeAlias_Numbers {
// Bool
using b8 = bool;

// Unsigned
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = size_t;
inline constexpr u8 u8_min = std::numeric_limits<u8>::min();
inline constexpr u8 u8_max = std::numeric_limits<u8>::max();
inline constexpr u16 u16_min = std::numeric_limits<u16>::min();
inline constexpr u16 u16_max = std::numeric_limits<u16>::max();
inline constexpr u32 u32_min = std::numeric_limits<u32>::min();
inline constexpr u32 u32_max = std::numeric_limits<u32>::max();
inline constexpr u64 u64_min = std::numeric_limits<u64>::min();
inline constexpr u64 u64_max = std::numeric_limits<u64>::max();
inline constexpr usize usize_min = std::numeric_limits<usize>::min();
inline constexpr usize usize_max = std::numeric_limits<usize>::max();

// Signed
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using isize = ptrdiff_t;
inline constexpr i8 i8_min = std::numeric_limits<i8>::min();
inline constexpr i8 i8_max = std::numeric_limits<i8>::max();
inline constexpr i16 i16_min = std::numeric_limits<i16>::min();
inline constexpr i16 i16_max = std::numeric_limits<i16>::max();
inline constexpr i32 i32_min = std::numeric_limits<i32>::min();
inline constexpr i32 i32_max = std::numeric_limits<i32>::max();
inline constexpr i64 i64_min = std::numeric_limits<i64>::min();
inline constexpr i64 i64_max = std::numeric_limits<i64>::max();
inline constexpr isize isize_min = std::numeric_limits<isize>::min();
inline constexpr isize isize_max = std::numeric_limits<isize>::max();

// Floating point
using f32 = float;
using f64 = double;
inline constexpr f32 f32_min = std::numeric_limits<f32>::min();
inline constexpr f32 f32_max = std::numeric_limits<f32>::max();
inline constexpr f32 f32_epsilon = std::numeric_limits<f32>::epsilon();
inline constexpr f64 f64_min = std::numeric_limits<f64>::min();
inline constexpr f64 f64_max = std::numeric_limits<f64>::max();
inline constexpr f64 f64_epsilon = std::numeric_limits<f64>::epsilon();

} // namespace TypeAlias_Numbers
using namespace TypeAlias_Numbers;


//-------------------------------------
// ... Pointers Aliases
//-------------------------------------

namespace TypeAlias_Pointers {
// Unique pointer
template <typename T>
using uptr = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr uptr<T> unew(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Shared pointer
template <typename T>
using sptr = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr sptr<T> snew(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace TypeAlias_Pointers
using namespace TypeAlias_Pointers;


//-------------------------------------
// ... Containers Aliases
//-------------------------------------

namespace TypeAlias_Containers {
// Maps
template <typename K, typename V>
using umap = std::unordered_map<K, V>;
template <typename K, typename V>
using omap = std::map<K, V>;

// Sets
template <typename T>
using uset = std::unordered_set<T>;
template <typename T>
using oset = std::set<T>;

// Arrays
template <typename T>
using vec = std::vector<T>;
template <typename T, size_t S>
using arr = std::array<T, S>;

// Optional
template <typename T>
using opt = std::optional<T>;

// // String
// using str = std::string;
} // namespace TypeAlias_Containers
static inline constexpr auto none = std::nullopt;
using namespace TypeAlias_Containers;


//-------------------------------------
// ... GLM Aliases
//-------------------------------------

#ifdef ALT_CPP_INCLUDE_GLM

namespace TypeAlias_GLM {
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
} // namespace TypeAlias_GLM
using namespace TypeAlias_GLM;
#else
namespace TypeAlias_GLM {} // namespace TypeAlias_GLM
#endif


//-------------------------------------
// ... Time Consts
//-------------------------------------

inline constexpr f64 s_to_ms = 1e+3;
inline constexpr f64 s_to_us = 1e+6;
inline constexpr f64 s_to_ns = 1e+9;

inline constexpr f64 ms_to_s = 1e-3;
inline constexpr f64 ms_to_us = 1e+3;
inline constexpr f64 ms_to_ns = 1e+6;

inline constexpr f64 us_to_s = 1e-6;
inline constexpr f64 us_to_ms = 1e-3;
inline constexpr f64 us_to_ns = 1e+3;

inline constexpr f64 ns_to_s = 1e-9;
inline constexpr f64 ns_to_ms = 1e-6;
inline constexpr f64 ns_to_us = 1e-3;


//-------------------------------------
// ... Elapsed Timer
//-------------------------------------

class ElapsedTimer {
public:
    void reset();
    f64 elapsed_s() const;
    f64 elapsed_ms() const;
    f64 elapsed_us() const;
    f64 elapsed_ns() const;
    bool is_valid() const;

private:
    i64 elapsed() const;

private:
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point m_ref = Clock::now();
    bool m_valid = false;
};
using ETimer = ElapsedTimer;


//-------------------------------------
// ... String Utils
//-------------------------------------

std::string str_replace(std::string to_replace, std::string const &from, std::string const &to,
                        bool onlyFirstMatch = false);
vec<std::string> str_split(std::string const &to_split, std::string const &delimeter);


//-------------------------------------
// ... Binary Utils
//-------------------------------------

vec<u8> bin_read(std::string const &path);
bool bin_check_magic(std::span<const u8> bin, std::span<const u8> magic);


//-------------------------------------
// ... Files Utils
//-------------------------------------

std::string file_read(std::string const &input_file);

bool file_write_append(std::string const &output_file, std::string const &to_write);
bool file_write_trunc(std::string const &output_file, std::string const &to_write);

bool file_write_append(std::string const &output_file, const char *data, usize data_size);
bool file_write_trunc(std::string const &output_file, const char *data, usize data_size);

bool file_check_extension(std::string const &input_file, std::string ext);


//-------------------------------------
// ... Math Utils
//-------------------------------------

f32 map(f32 value, f32 srcMin, f32 srcMax, f32 dstMin, f32 dstMax);
f32 map_100(f32 value, f32 dstMin, f32 dstMax);

bool fuzzyEq(f32 f1, f32 f2, f32 threshold = 0.01f);

f32 clampAngle(f32 angle);

#ifdef ALT_CPP_INCLUDE_GLM
bool fuzzyEq(vec2 const &v1, vec2 const &v2, f32 t = 0.01f);
bool fuzzyEq(vec3 const &v1, vec3 const &v2, f32 t = 0.01f);
bool fuzzyEq(vec4 const &v1, vec4 const &v2, f32 t = 0.01f);

template <typename T>
inline bool isAligned(T const &a, T const &b, f32 margin = 0.f) {
    return abs(glm::dot(glm::normalize(a), glm::normalize(b))) >= (1.f - f32_epsilon - margin);
}
#endif

} // namespace ac


//=============================================================================
//=============================================================================
// ALT_CPP IMPLEMENTATIONs
//=============================================================================
//=============================================================================

#ifdef ALT_CPP_IMPLEMENTATION

#ifndef _ALT_CPP_IMPLEMENTATION
#define _ALT_CPP_IMPLEMENTATION

#include <fstream>

namespace ac {

//-------------------------------------
// ... Logger
//-------------------------------------

#ifndef ALT_CPP_INCLUDE_FMT
namespace detail {
std::string format(std::string msg, std::vector<std::string> const &args) {
    msg += " | <== ";
    for (size_t i = 0; i < args.size() - 1; ++i) {
        msg += "{ " + args[i] + " } : ";
    }
    msg += "{ " + args[args.size() - 1] + " }";
    return msg;
}
std::vector<std::string> to_stringlist() { return {}; }
} // namespace detail
#endif


//-------------------------------------
// ... Elapsed Timer
//-------------------------------------

void ElapsedTimer::reset() {
    m_ref = Clock::now();
    m_valid = true;
}
f64 ElapsedTimer::elapsed_s() const { return as(f64, elapsed()) * ns_to_s; }
f64 ElapsedTimer::elapsed_ms() const { return as(f64, elapsed()) * ns_to_ms; }
f64 ElapsedTimer::elapsed_us() const { return as(f64, elapsed()) * ns_to_us; }
f64 ElapsedTimer::elapsed_ns() const { return as(f64, elapsed()); }
bool ElapsedTimer::is_valid() const { return m_valid; }
i64 ElapsedTimer::elapsed() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - m_ref).count();
}


//-------------------------------------
// ... String Utils
//-------------------------------------

std::string str_replace(std::string to_replace, std::string const &from, std::string const &to, bool only_first_match) {
    usize pos = 0;
    while ((pos = to_replace.find(from)) < to_replace.size()) {
        to_replace.replace(pos, from.length(), to);

        if (only_first_match) {
            break;
        }
    }

    return to_replace;
}

vec<std::string> str_split(std::string const &to_split, std::string const &delimeter) {
    std::string token;
    vec<std::string> splitted;
    usize ini = 0;
    usize end = 0;

    // Split and store the string body
    while ((end = to_split.find(delimeter, ini)) < to_split.size()) {
        token = to_split.substr(ini, end - ini);
        ini = end + delimeter.size();
        splitted.push_back(token);
    }

    // Store the string tail
    if (ini < to_split.size()) {
        token = to_split.substr(ini);
        splitted.push_back(token);
    }

    return splitted;
}


//-------------------------------------
// ... Binary Utils
//-------------------------------------

vec<u8> bin_read(std::string const &path) {
    std::ifstream file { path, std::ios::binary };
    auto fileBegin = std::istreambuf_iterator<char>(file);
    auto fileEnd = std::istreambuf_iterator<char>();
    return { fileBegin, fileEnd };
}

bool bin_check_magic(std::span<const u8> bin, std::span<const u8> magic) {
    // Validation
    if (magic.empty() || bin.size() < magic.size()) {
        return false;
    }
    // Iteration
    bool match = true;
    for (size_t i = 0; i < magic.size(); ++i) {
        match &= (bin[i] == magic[i]);
    }
    // Result
    return match;
}


//-------------------------------------
// ...Files Utils
//-------------------------------------

std::string file_read(std::string const &input_file) {
    std::ifstream file(input_file, std::ios::ate | std::ios::binary);
    ac_defer(file.close());

    if (!file.is_open()) {
        return "";
        ac_err("Issues opening file [r]: {}", input_file);
    }

    std::string content;
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());

    return content;
}

bool file_write(std::string const &output_file, char const *data, usize data_size, std::ios_base::openmode mode) {
    if (!data || data_size < 1) {
        return false;
        ac_err("[file_write] Invalid data: {}", output_file);
    }

    std::ofstream file(output_file, std::ios::out | std::ios::binary | mode);
    ac_defer(file.close());

    if (!file.is_open()) {
        return false;
        ac_err("[file_write] Opening file: {}", output_file);
    }

    file.write(data, data_size);

    return true;
}
bool file_write_append(std::string const &output_file, std::string const &to_write) {
    return file_write(output_file, to_write.data(), to_write.size(), std::ios::app);
}
bool file_write_trunc(std::string const &output_file, std::string const &to_write) {
    return file_write(output_file, to_write.data(), to_write.size(), std::ios::trunc);
}
bool file_write_append(std::string const &output_file, const char *data, usize data_size) {
    return file_write(output_file, data, data_size, std::ios::app);
}
bool file_write_trunc(std::string const &output_file, const char *data, usize data_size) {
    return file_write(output_file, data, data_size, std::ios::trunc);
}

bool file_check_extension(std::string const &input_file, std::string ext) {
    auto to_check = input_file.substr(input_file.find_last_of('.') + 1);
    std::transform(to_check.begin(), to_check.end(), to_check.begin(), ::tolower);

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return to_check == ext;
}


//-------------------------------------
// ... Math Utils
//-------------------------------------

f32 map(f32 value, f32 srcMin, f32 srcMax, f32 dstMin, f32 dstMax) {
    return dstMin + (dstMax - dstMin) * (value - srcMin) / (srcMax - srcMin);
}
f32 map_100(f32 value, f32 dstMin, f32 dstMax) { return map(value, 0, 100, dstMin, dstMax); }
bool fuzzyEq(f32 f1, f32 f2, f32 threshold) {
    auto const diff = abs(f1 - f2);
    auto const isEq = diff <= threshold;
    return isEq;
}
f32 clampAngle(f32 angle) {
    auto const turns = floorf(angle / 360.f);
    return angle - 360.f * turns;
}

#ifdef ALT_CPP_INCLUDE_GLM
bool fuzzyEq(vec2 const &v1, vec2 const &v2, f32 t) { return fuzzyEq(v1.x, v2.x, t) && fuzzyEq(v1.y, v2.y, t); }
bool fuzzyEq(vec3 const &v1, vec3 const &v2, f32 t) {
    return fuzzyEq(v1.x, v2.x, t) && fuzzyEq(v1.y, v2.y, t) && fuzzyEq(v1.z, v2.z, t);
}
bool fuzzyEq(vec4 const &v1, vec4 const &v2, f32 t) {
    return fuzzyEq(v1.x, v2.x, t) && fuzzyEq(v1.y, v2.y, t) && fuzzyEq(v1.z, v2.z, t) && fuzzyEq(v1.w, v2.w, t);
}
#endif


//-------------------------------------
// ...
//-------------------------------------

} // namespace ac

#endif // _ALT_CPP_IMPLEMENTATION

#endif // ALT_CPP_IMPLEMENTATION

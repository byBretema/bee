#pragma once

/* alt_bench - v0.01

    Micro framework for bechmarking

    No warranty implied, use at your own risk.

    =============================================
    ! How to include
    =============================================

    -- Classic header-only stuff, add this:

    #define ALT_BENCH_IMPLEMENTATION

    -- Before you include this file in *one* C++ file to create the
    implementation, something like this:

    #include ...
    #include ...
    #define ALT_BENCH_IMPLEMENTATION
    #include "alt_bench.hpp"

    =============================================
    ! Define-Based options:
    =============================================

    -- By default we redirect the output of the benchmarked code during its execution
    so you don't see the same log hundreds or thousands of times, you can avoid that behaviour
    using below flag before including this file.

    #define ALT_BENCH_ENABLE_STDOUT

*/

#include <cstdio>
#include <iostream>

#include <chrono>

#include <functional>
#include <tuple>
#include <vector>

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace ac::bench {

namespace detail {
void add(std::string const &name, int32_t times, std::function<void()> const &fn);
}

void run();

} // namespace ac::bench

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define __AC_BENCH_CONCAT2(l, r) l##r
#define __AC_BENCH_CONCAT1(l, r) __AC_BENCH_CONCAT2(l, r)
#define AC_BENCH_CONCAT(l, r) __AC_BENCH_CONCAT1(l, r)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define BENCH_CASE(name, times, code)                                                                                  \
    static inline int AC_BENCH_CONCAT(bench_case__, __LINE__) = [] {                                                   \
        ac::bench::detail::add(name, times, [] { code; });                                                             \
        return 0;                                                                                                      \
    }();

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//=============================================================================
//=============================================================================
// ALT_BENCH_IMPLEMENTATION
//=============================================================================
//=============================================================================

#ifdef ALT_BENCH_IMPLEMENTATION

#ifndef __ALT_BENCH_IMPLEMENTATION_GUARD
#define __ALT_BENCH_IMPLEMENTATION_GUARD

namespace ac::bench {

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace detail {

inline FILE *stdout_file = nullptr;

void stdout_off() {
    std::fflush(stdout);
#ifdef _WIN32
    stdout_file = freopen("NUL", "w", stdout);
#else
    stdout_file = freopen("/dev/null", "w", stdout);
#endif
}

void stdout_on() {
    std::fflush(stdout);
    if (stdout_file) {
#ifdef _WIN32
        freopen("CON", "w", stdout); // Windows console device
#else
        freopen("/dev/tty", "w", stdout); // Linux/macOS console device
#endif
    }
}

struct Item {
    std::string name = "";
    int32_t times = 1;
    std::function<void()> fn = nullptr;
};
inline std::vector<Item> gList;


void add(std::string const &name, int32_t times, std::function<void()> const &fn) {
    gList.emplace_back(name, times, fn);
}

} // namespace detail

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void run() {

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    for (auto const &[name, times, fn] : detail::gList) {

#if !defined(ALT_BENCH_ENABLE_STDOUT)
        detail::stdout_off();
#endif

        uint32_t i = 0;
        start = std::chrono::high_resolution_clock::now();
        for (; i < times; ++i) {
            fn();
        }
        end = std::chrono::high_resolution_clock::now();

#if !defined(ALT_BENCH_ENABLE_STDOUT)
        detail::stdout_on();
#endif

        using ns = std::chrono::nanoseconds;
        auto const elapsed = std::chrono::duration_cast<ns>(end - start).count();

        std::cout << "[alt_bench] | " << name << " | " << double(elapsed) * 1e-6 << " ms\n";
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


} // namespace ac::bench

#endif // __ALT_BENCH_IMPLEMENTATION_GUARD
#endif // ALT_BENCH_IMPLEMENTATION

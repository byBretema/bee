#pragma once

/* alt_bench - v0.01

    Nano framework for bechmarking

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
    using below flag before including this file. (Do you really want that?)

    #define ALT_BENCH_STDOUT_ENABLE

    -- By default we redirect the output of the benchmarked code during its execution
    so you don't see the same log hundreds or thousands of times, you can use below flag to run
    the funtion ONCE with the stdout enabled, the rest of 'times' will be disabled.

    #define ALT_BENCH_STDOUT_ONCE

*/

#include <cstdint>
#include <cstdio>
#include <iostream>

#include <chrono>

#include <format>
#include <functional>
#include <vector>

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace ac::bench {

namespace detail {
void add(std::string const &name, int32_t times, std::function<void()> const &fn, const char *file, int line);
}

void run();

} // namespace ac::bench

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define __AC_BENCH_CONCAT2(l, r) l##r
#define __AC_BENCH_CONCAT1(l, r) __AC_BENCH_CONCAT2(l, r)
#define __AC_BENCH_CONCAT(l, r) __AC_BENCH_CONCAT1(l, r)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define BENCH(name, times, code)                                                                                       \
    static inline int __AC_BENCH_CONCAT(bench_case__, __LINE__) = [] {                                                 \
        ac::bench::detail::add(name, times, [] { code; }, __FILE__, __LINE__);                                         \
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

#ifdef _WIN32
#include <windows.h>
static const int ___ALT_BENCH_COUT_SETUP = []() {
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}();
#endif

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

struct Benchmark {
    std::string name = "";
    int32_t times = 1;
    std::function<void()> fn = nullptr;
    const char *file = "";
    int32_t line = -1;
};
inline std::vector<Benchmark> g_benchmarks;


void add(std::string const &name, int32_t times, std::function<void()> const &fn, const char *file, int line) {
    g_benchmarks.push_back({ name, times, fn, file, line });
}

} // namespace detail

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void run() {

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                   😴 RUNNING BENCHMARKs\n";
    std::cout << "============================================================\n";
    std::cout << "\n";

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    for (auto const &[name, times, fn, file, line] : detail::g_benchmarks) {

#if !defined(ALT_BENCH_STDOUT_ENABLE)
        detail::stdout_off();
#endif

        uint32_t i = 0;
        start = std::chrono::high_resolution_clock::now();
        for (; i < times; ++i) {
            fn();
        }
        end = std::chrono::high_resolution_clock::now();

#if !defined(ALT_BENCH_STDOUT_ENABLE)
        detail::stdout_on();
#endif

        using ns = std::chrono::nanoseconds;
        auto const elapsed = std::chrono::duration_cast<ns>(end - start).count();

        std::cout << "⌚ " << name << " :: Executed " << times << " times in " //
                  << double(elapsed) * 1e-6 << " ms  ( " << file << ":" << line << " )\n";
#if defined(ALT_BENCH_STDOUT_ONCE)
        fn();
        std::cout << "\n";
#endif
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


} // namespace ac::bench

#endif // __ALT_BENCH_IMPLEMENTATION_GUARD
#endif // ALT_BENCH_IMPLEMENTATION

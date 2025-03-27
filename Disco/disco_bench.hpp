#pragma once

/* disco_bench - v0.01

    Nano framework for bechmarking

    No warranty implied, use at your own risk.

    =============================================
    ! How to include
    =============================================

    -- Classic header-only stuff, add this:

    #define DISCO_BENCH_IMPLEMENTATION

    -- Before you include this file in *one* C++ file to create the
    implementation, something like this:

    #include ...
    #include ...
    #define DISCO_BENCH_IMPLEMENTATION
    #include "disco_bench.hpp"

    =============================================
    ! Define-Based options:
    =============================================

    -- By default we redirect the output of the benchmarked code during its execution
    so you don't see the same log hundreds or thousands of times, you can use below flag to run
    the funtion ONCE with the stdout enabled, the rest of 'times' will be disabled.

    #define DISCO_BENCH_STDOUT_ONCE

    -- By default we redirect the output of the benchmarked code during its execution
    so you don't see the same log hundreds or thousands of times, you can avoid that behaviour
    using below flag before including this file. (Do you really want that?)

    #define DISCO_BENCH_STDOUT_ENABLE

*/


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                INCLUDES                                  #
// #                                                                          #
// #                                                                          #
// ############################################################################

#include <cstdint>
#include <cstdio>
#include <iostream>

#include <chrono>

#include <format>
#include <functional>
#include <vector>


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                NAMESPACE                                 #
// #                                                                          #
// #                                                                          #
// ############################################################################

namespace dc::bench {

namespace detail {

void add(std::string const &name, int32_t times, std::function<void()> const &fn, const char *file, int line);

} // namespace detail

void run();

} // namespace dc::bench


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                  CONCAT                                  #
// #                                                                          #
// #           // ############################################################################
// #                                                                          #
// #                                                                          #
// #                            IMPLEMENTATION                                #
// #                                                                          #
// #                                                                          #
// ############################################################################ #
// ############################################################################

#ifndef DISCO_CONCAT
#define __DISCO_CONCAT2(l, r) l##r
#define __DISCO_CONCAT1(l, r) __DISCO_CONCAT2(l, r)
#define DISCO_CONCAT(l, r) __DISCO_CONCAT1(l, r)
#endif


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                 MACROS                                   #
// #                                                                          #
// #                                                                          #
// ############################################################################

#define BENCH(name, times, ...)                                                                                        \
    static inline int DISCO_CONCAT(bench_case__, __LINE__) = [] {                                                      \
        dc::bench::detail::add(name, times, [] { __VA_ARGS__; }, __FILE__, __LINE__);                                  \
        return 0;                                                                                                      \
    }();


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                            IMPLEMENTATION                                #
// #                                                                          #
// #                                                                          #
// ############################################################################

#define DISCO_BENCH_IMPLEMENTATION
#ifdef DISCO_BENCH_IMPLEMENTATION

#ifndef __DISCO_BENCH_IMPLEMENTATION_GUARD
#define __DISCO_BENCH_IMPLEMENTATION_GUARD

#ifdef _WIN32
#include <windows.h>
static const int ___DISCO_BENCH_COUT_SETUP = []() {
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}();
#endif

namespace dc::bench {


// ==============================================
// ========== Internal helpers

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


// ==============================================
// ========== Public API

void run() {

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                   😴 RUNNING BENCHMARKs\n";
    std::cout << "============================================================\n";
    std::cout << "\n";

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    for (auto const &[name, times, fn, file, line] : detail::g_benchmarks) {

#ifndef DISCO_BENCH_STDOUT_ENABLE
        detail::stdout_off();
#endif

        uint32_t i = 0;
        start = std::chrono::high_resolution_clock::now();
        for (; i < times; ++i) {
            fn();
        }
        end = std::chrono::high_resolution_clock::now();

#ifndef DISCO_BENCH_STDOUT_ENABLE
        detail::stdout_on();
#endif

        using ns = std::chrono::nanoseconds;
        auto const elapsed = std::chrono::duration_cast<ns>(end - start).count();

        std::cout << "⌚ " << name << " :: Executed " << times << " times in " //
                  << double(elapsed) * 1e-6 << " ms  ( " << file << ":" << line << " )\n";

#ifdef DISCO_BENCH_STDOUT_ONCE
        fn();
        std::cout << "\n";
#endif
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


} // namespace dc::bench

#endif // __DISCO_BENCH_IMPLEMENTATION_GUARD
#endif // DISCO_BENCH_IMPLEMENTATION

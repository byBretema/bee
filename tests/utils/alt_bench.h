#pragma once

/* alt_bench (part of alt_cpp) - v0.01

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

namespace ac {

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

using BenchFn = std::function<void()>;
using BenchData = std::tuple<std::string, int32_t, BenchFn>;
inline std::vector<BenchData> gBenchList;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace StdOut {

inline FILE *null_file = nullptr;

void off() {
    std::fflush(stdout);
#ifdef _WIN32
    null_file = freopen("NUL", "w", stdout);
#else
    null_file = freopen("/dev/null", "w", stdout);
#endif
}

void on() {
    std::fflush(stdout);
    if (null_file) {
#ifdef _WIN32
        freopen("CON", "w", stdout); // Windows console device
#else
        freopen("/dev/tty", "w", stdout); // Linux/macOS console device
#endif
    }
}

} // namespace StdOut

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void RunBenchmarks() {

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    for (auto const &[name, times, fn] : gBenchList) {

#if !defined(ALT_BENCH_ENABLE_STDOUT)
        StdOut::off();
#endif

        uint32_t i = 0;
        start = std::chrono::high_resolution_clock::now();
        for (; i < times; ++i) {
            fn();
        }
        end = std::chrono::high_resolution_clock::now();

#if !defined(ALT_BENCH_ENABLE_STDOUT)
        StdOut::on();
#endif

        using ns = std::chrono::nanoseconds;
        auto const elapsed = std::chrono::duration_cast<ns>(end - start).count();

        std::cout << "[alt_bench] | " << name << " | " << double(elapsed) * 1e-6 << " ms\n";
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

} // namespace ac

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define __AC_TEST_CONCAT2(l, r) l##r
#define __AC_TEST_CONCAT1(l, r) __AC_TEST_CONCAT2(l, r)
#define AC_TEST_CONCAT(l, r) __AC_TEST_CONCAT1(l, r)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define BENCH_CASE(name, times, code)                                                                                  \
    static inline int AC_TEST_CONCAT(bench_case__, __LINE__) = [] {                                                    \
        ac::gBenchList.push_back({ name, times, [] { code; } });                                                       \
        return 0;                                                                                                      \
    }()

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

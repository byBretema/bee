#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define ALT_CPP_IMPLEMENTATION
#define ALT_CPP_INCLUDE_FMT
// #define ALT_CPP_INCLUDE_GLM
#include "../alt.hpp"

#include <iostream>
#include <array>
#include <vector>

#include <cstdio>

using namespace ac::TypeAlias_Numbers;
using namespace ac::TypeAlias_GLM;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct DISABLE_OUTPUT
{
    DISABLE_OUTPUT()
    {
        std::fflush(stdout);
#ifdef _WIN32
        null_file = freopen("NUL", "w", stdout);
#else
        null_file = freopen("/dev/null", "w", stdout);
#endif
    }
    ~DISABLE_OUTPUT()
    {
        std::fflush(stdout);
        if (null_file)
        {
#ifdef _WIN32
            freopen("CON", "w", stdout);  // Windows console device
#else
            freopen("/dev/tty", "w", stdout);  // Linux/macOS console device
#endif
        }
    }

private:
    FILE *null_file = nullptr;
};

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void run_tests()
{
    doctest::Context ctx;
    ctx.setOption("no-version", true);
    ctx.setOption("no-intro", true);

    int res = ctx.run();
    if (ctx.shouldExit())
        exit(res);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<typename T>
void run_benchmark(const char* name, uint32_t times, T fn)
{
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    {
        DISABLE_OUTPUT scoped;

        start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < times; ++i)
        {
            fn();
        }
        end = std::chrono::high_resolution_clock::now();
    }

    using ns           = std::chrono::nanoseconds;
    auto const elapsed = std::chrono::duration_cast<ns>(end - start).count();

    std::cout << "[docbench] - " << name << ": " <<  double(elapsed) * 1e-6 << " ms\n";
}

void run_benchmarks()
{
    run_benchmark("cout", 5, [](){
        std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 1 << " is " << ac_bit(1) << "\n";
    });
    run_benchmark("ac_info", 5, [](){
        ac_info("2 elevated to {} is {} = {}", 1, ac_bit(1), true);
    });

#ifdef ALT_CPP_INCLUDE_GLM
    run_benchmark("ac_info_glm_vec3", 5, [](){
        ac_info("glm vec3 {}", glmstr(vec3(2.f)));
    });
#endif
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv)
{
    std::cout << "\n";
    std::cout << "===========================================================\n";
    std::cout << "== RUNNING TESTs\n";
    std::cout << "===========================================================\n";
    run_tests();

    std::cout << "\n";
    std::cout << "===========================================================\n";
    std::cout << "== RUNNING \"BENCHMARKs\"\n";
    std::cout << "===========================================================\n";
    run_benchmarks();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TEST_CASE("testing the factorial function")
{
    CHECK(ac::u8_min == std::numeric_limits<uint8_t>::min());
}
TEST_CASE("testing the factorial function")
{
    CHECK(ac::u8_max == std::numeric_limits<uint8_t>::max());
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

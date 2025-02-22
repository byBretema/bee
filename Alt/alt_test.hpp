#pragma once

/* alt_test - v0.01

    Nano framework for testing

    No warranty implied, use at your own risk.

    =============================================
    ! How to include
    =============================================

    -- Classic header-only stuff, add this:

    #define ALT_TEST_IMPLEMENTATION

    -- Before you include this file in *one* C++ file to create the
    implementation, something like this:

    #include ...
    #include ...
    #define ALT_TEST_IMPLEMENTATION
    #include "alt_test.hpp"

*/

#include <cstdint>
#include <cstdio>
#include <iostream>

#include <functional>
#include <vector>

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace ac::test {

namespace detail {
struct Error {
    const char *name = "";
    const char *file = "";
    int line = -1;
};
struct Test {
    const char *name = "";
    std::function<void(Test *test)> fn = nullptr;
    std::vector<Error> errors {};
    int32_t total_count = 0;
    int32_t fail_count = 0;
    int32_t pass_count = 0;
};

void add(Test const &test);
} // namespace detail

void run();

} // namespace ac::test

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define __AC_TEST_CONCAT2(l, r) l##r
#define __AC_TEST_CONCAT1(l, r) __AC_TEST_CONCAT2(l, r)
#define __AC_TEST_CONCAT(l, r) __AC_TEST_CONCAT1(l, r)

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


#define TEST(name, code)                                                                                               \
    static inline int __AC_TEST_CONCAT(test_case__, __LINE__) = [] {                                                   \
        ac::test::detail::Test test { name };                                                                          \
        test.fn = [](Test *test) { code; };                                                                            \
        ac::test::detail::add(test);                                                                                   \
        return 0;                                                                                                      \
    }();

#define CHECK(name, condition)                                                                                         \
    ++(*test).total_count;                                                                                             \
    if ((condition)) {                                                                                                 \
        ++(*test).pass_count;                                                                                          \
    } else {                                                                                                           \
        (*test).errors.emplace_back(name, __FILE__, __LINE__);                                                         \
        ++(*test).fail_count;                                                                                          \
    }

#define TEST_CHECK(name, code) TEST(name, CHECK(name, code));

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//=============================================================================
//=============================================================================
// ALT_TEST_IMPLEMENTATION
//=============================================================================
//=============================================================================

#define ALT_TEST_IMPLEMENTATION

#ifdef ALT_TEST_IMPLEMENTATION

#ifndef __ALT_TEST_IMPLEMENTATION_GUARD
#define __ALT_TEST_IMPLEMENTATION_GUARD

#ifdef _WIN32
#include <windows.h>
static const int ___ALT_TEST_COUT_SETUP = []() {
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}();
#endif

namespace ac::test {

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace detail {

inline std::vector<Test> g_tests;
void add(Test const &test) { g_tests.push_back(test); }

} // namespace detail

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void run() {

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                      👍 RUNNING TESTs\n";
    std::cout << "============================================================\n";
    std::cout << "\n";

    int32_t total_count = 0;
    int32_t pass_count = 0;
    int32_t fail_count = 0;

    for (auto const &test : detail::g_tests) {

        if (test.fn) {
            test.fn(&test);
        }

        total_count += test.total_count;
        total_count += test.pass_count;
        total_count += test.fail_count;

        for (auto const &[name, file, line] : test.errors) {

            auto const pname = test.total_count < 2 ? "" : test.name;
            auto const psep = test.total_count < 2 ? "" : " :: ";
            std::cout << "🚩 " << pname << psep << name << " ( " << file << ":" << line << " )\n";

            continue;
        }
    }

    if (fail_count > 0) {
        std::cout << "\n-----------------------------\n\n";
    }

    std::cout << "📦 Total -> " << total_count << "\n";
    std::cout << "✅ Pass  -> " << pass_count << "\n";
    std::cout << "❌ Fail  -> " << fail_count << "\n";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


} // namespace ac::test

#endif // __ALT_TEST_IMPLEMENTATION_GUARD
#endif // ALT_TEST_IMPLEMENTATION

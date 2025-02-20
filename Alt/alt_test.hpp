#pragma once

/* alt_test - v0.01

    Micro framework for testing

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
struct SubTest {
    const char *name = "";
    std::function<bool()> fn = nullptr;
    const char *file = "";
    int line = -1;
};
struct Test {
    const char *name = "";
    std::vector<SubTest> subtests {};
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

#define TEST_CHECK(testName, code)                                                                                     \
    static inline int __AC_TEST_CONCAT(test_case__, __LINE__) = [] {                                                   \
        ac::test::detail::SubTest subTest;                                                                             \
        subTest.name = "·";                                                                                            \
        subTest.fn = [] { return code; };                                                                              \
        subTest.file = __FILE__;                                                                                       \
        subTest.line = __LINE__;                                                                                       \
        ac::test::detail::add(ac::test::detail::Test { testName, { subTest } });                                       \
        return 0;                                                                                                      \
    }();

#define TEST(name, code)                                                                                               \
    static inline int __AC_TEST_CONCAT(test_case__, __LINE__) = [] {                                                   \
        ac::test::detail::Test test { name, {} };                                                                      \
        test.subtests.reserve(4);                                                                                      \
        code;                                                                                                          \
        ac::test::detail::add(test);                                                                                   \
        return 0;                                                                                                      \
    }();

#define CHECK(name, code) test.subtests.emplace_back(name, [&] { return code; }, __FILE__, __LINE__)

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

namespace ac::test {

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace detail {

inline std::vector<Test> g_tests;
void add(Test const &test) { g_tests.push_back(test); }

} // namespace detail


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void run() {

    std::cout << "\n";
    std::cout << "===========================================================\n";
    std::cout << "👍 RUNNING TESTs\n";
    std::cout << "===========================================================\n";

    int32_t fail_count = 0;
    int32_t pass_count = 0;
    int32_t total_count = 0;

    for (auto const &[test_name, sub_tests] : detail::g_tests) {

        bool show_header = true;
        for (auto const &[name, fn, file, line] : sub_tests) {

            ++total_count;

            if (fn && !fn()) {

                if (show_header) {
                    if (fail_count > 0) {
                        std::cout << "---------------------------------------\n";
                    }
                    std::cout << "\n";
                    std::cout << "❌ " << test_name << "\n";
                    std::cout << "---------------------------------------\n";
                    show_header = false;
                }

                std::cout << "🔺 " << name << " | " << file << ":" << line << "\n";

                ++fail_count;
                continue;
            }

            ++pass_count;
        }
    }

    if (fail_count > 0) {
        std::cout << "---------------------------------------\n\n";
    }

    std::cout << "📦 Total | " << total_count << "\n";
    std::cout << "✅ Pass  | " << pass_count << "\n";
    std::cout << "❌ Fail  | " << fail_count << "\n";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


} // namespace ac::test

#endif // __ALT_TEST_IMPLEMENTATION_GUARD
#endif // ALT_TEST_IMPLEMENTATION

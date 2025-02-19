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
    std::string name = "";
    std::function<bool()> fn = nullptr;
};
struct Test {
    std::string name = "";
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

#define TEST_WRAP(name, code)                                                                                          \
    static inline int __AC_TEST_CONCAT(test_case__, __LINE__) = [] {                                                   \
        ac::test::detail::Test test { name, {} };                                                                      \
        code;                                                                                                          \
        ac::test::detail::add(test);                                                                                   \
        return 0;                                                                                                      \
    }();

#define TEST_CASE(name, code) test.subtests.emplace_back(name, [] { code })

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

    int32_t fail_count = 0;
    int32_t pass_count = 0;
    int32_t total_count = 0;

    for (auto const &[test_name, sub_tests] : detail::g_tests) {
        for (auto const &[subtest_name, subtest_fn] : sub_tests) {
            ++total_count;
            if (!subtest_fn()) {
                std::cout << "[alt_test] | " << test_name << " > " << subtest_name << " | FAILED\n";
                std::cout << "\n-------------------------------------------------------------------------------\n";
                ++fail_count;
                continue;
            }
            ++pass_count;
        }
    }

    std::cout << "[alt_test] | PASS COUNT | " << pass_count << " of " << total_count << "\n";
    std::cout << "[alt_test] | FAIL COUNT | " << fail_count << " of " << total_count << "\n";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


} // namespace ac::test

#endif // __ALT_TEST_IMPLEMENTATION_GUARD
#endif // ALT_TEST_IMPLEMENTATION

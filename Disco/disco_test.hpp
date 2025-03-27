#pragma once

/* disco_test - v0.01

    Nano framework for testing

    No warranty implied, use at your own risk.

    =============================================
    ! How to include
    =============================================

    -- Classic header-only stuff, add this:

    #define DISCO_TEST_IMPLEMENTATION

    -- Before you include this file in *one* C++ file to create the
    implementation, something like this:

    #include ...
    #include ...
    #define DISCO_TEST_IMPLEMENTATION
    #include "disco_test.hpp"

*/


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                INCLUDES                                  #
// #                                                                          #
// #                                                                          #
// ############################################################################

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                NAMESPACE                                 #
// #                                                                          #
// #                                                                          #
// ############################################################################

namespace dc::test {

namespace detail {

struct Error {
    const char *name = "";
    const char *file = "";
    int line = -1;
};

struct Result {
    std::vector<Error> errors {};
    int32_t total_count = 0;
    int32_t fail_count = 0;
    int32_t pass_count = 0;
};

struct Test {
    const char *name = "";
    Result (*fn)(void) = nullptr;
};

void add(Test const &test);

} // namespace detail

void run();

} // namespace dc::test


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                  CONCAT                                  #
// #                                                                          #
// #                                                                          #
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

#define TEST(name, ...)                                                                                                \
    static inline int DISCO_CONCAT(test_case__, __LINE__) = [] {                                                       \
        dc::test::detail::Test test { name };                                                                          \
        test.fn = []() {                                                                                               \
            dc::test::detail::Result result;                                                                           \
            __VA_ARGS__;                                                                                               \
            return result;                                                                                             \
        };                                                                                                             \
        dc::test::detail::add(test);                                                                                   \
        return 0;                                                                                                      \
    }();

#define CHECK(name, ...)                                                                                               \
    result.total_count++;                                                                                              \
    if ((__VA_ARGS__)) {                                                                                               \
        result.pass_count++;                                                                                           \
    } else {                                                                                                           \
        result.errors.push_back({ name, __FILE__, __LINE__ });                                                         \
        result.fail_count++;                                                                                           \
    }


#define TEST_CHECK(name, code) TEST(name, CHECK(name, code));


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                            IMPLEMENTATION                                #
// #                                                                          #
// #                                                                          #
// ############################################################################

#ifdef DISCO_TEST_IMPLEMENTATION

#ifndef __DISCO_TEST_IMPLEMENTATION_GUARD
#define __DISCO_TEST_IMPLEMENTATION_GUARD

#ifdef _WIN32
#include <windows.h>
static const int ___DISCO_TEST_COUT_SETUP = []() {
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}();
#endif

namespace dc::test {

// ==============================================
// ========== Internal helpers

namespace detail {

inline std::vector<Test> g_tests;
void add(Test const &test) { g_tests.push_back(test); }

} // namespace detail

// ==============================================
// ========== Public API

void run() {

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                      👍 RUNNING TESTs\n";
    std::cout << "============================================================\n";
    std::cout << "\n";

    int32_t total_count = 0;
    int32_t pass_count = 0;
    int32_t fail_count = 0;

    for (auto &test : detail::g_tests) {

        if (!test.fn) {
            assert(0);
            continue;
        }

        detail::Result const result = test.fn();

        total_count += result.total_count;
        pass_count += result.pass_count;
        fail_count += result.fail_count;

        for (auto const &[name, file, line] : result.errors) {

            auto const pname = result.total_count < 2 ? "" : test.name;
            auto const psep = result.total_count < 2 ? "" : " :: ";
            std::cout << "🚩 " << pname << psep << name << "  ( " << file << ":" << line << " )\n";

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

} // namespace dc::test

#endif // __DISCO_TEST_IMPLEMENTATION_GUARD
#endif // DISCO_TEST_IMPLEMENTATION

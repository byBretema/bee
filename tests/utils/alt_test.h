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

namespace ac::test {

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct Item {
    std::string name = "";
    std::function<void()> fn = nullptr;
};
inline std::vector<Item> gList;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void run() {
    // doctest::Context ctx;
    // ctx.setOption("no-version", true);
    // ctx.setOption("no-intro", true);

    // int res = ctx.run();
    // if (ctx.shouldExit()) {
    //     exit(res);
    // }
}

} // namespace ac::test

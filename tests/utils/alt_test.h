#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

namespace ac {

void RunTests() {
    doctest::Context ctx;
    ctx.setOption("no-version", true);
    ctx.setOption("no-intro", true);

    int res = ctx.run();
    if (ctx.shouldExit()) {
        exit(res);
    }
}

} // namespace ac


#define ALT_TEST_IMPLEMENTATION
#include "../Alt/alt_test.hpp"

#define ALT_BENCH_IMPLEMENTATION
#include "../Alt/alt_bench.hpp"

#define ALT_CPP_IMPLEMENTATION
// #define ALT_CPP_INCLUDE_FMT
// #define ALT_CPP_INCLUDE_GLM
#include "../Alt/alt_cpp.hpp"

using namespace ac::TypeAlias_Numbers;
using namespace ac::TypeAlias_GLM;

#include <iostream>

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: TESTS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TEST("Numeric Aliases", {
    CHECK("u8 min", ac::u8_min == std::numeric_limits<uint8_t>::min());
    CHECK("u8 max", ac::u8_max == std::numeric_limits<uint8_t>::max());
    CHECK("u16 min", ac::u16_min == std::numeric_limits<uint16_t>::min());
    CHECK("u16 max", ac::u16_max == std::numeric_limits<uint16_t>::max());
    CHECK("u32 min", ac::u32_min == std::numeric_limits<uint32_t>::min());
    CHECK("u32 max", ac::u32_max == std::numeric_limits<uint32_t>::max());
    CHECK("u64 min", ac::u64_min == std::numeric_limits<uint64_t>::min());
    CHECK("u64 max", ac::u64_max == std::numeric_limits<uint64_t>::max());
    CHECK("usize min", ac::usize_min == std::numeric_limits<size_t>::min());
    CHECK("usize max", ac::usize_max == std::numeric_limits<size_t>::max());

    CHECK("i8 min", ac::i8_min == std::numeric_limits<int8_t>::min());
    CHECK("i8 max", ac::i8_max == std::numeric_limits<int8_t>::max());
    CHECK("i16 min", ac::i16_min == std::numeric_limits<int16_t>::min());
    CHECK("i16 max", ac::i16_max == std::numeric_limits<int16_t>::max());
    CHECK("i32 min", ac::i32_min == std::numeric_limits<int32_t>::min());
    CHECK("i32 max", ac::i32_max == std::numeric_limits<int32_t>::max());
    CHECK("i64 min", ac::i64_min == std::numeric_limits<int64_t>::min());
    CHECK("i64 max", ac::i64_max == std::numeric_limits<int64_t>::max());
    CHECK("isize min", ac::isize_min == std::numeric_limits<ptrdiff_t>::min());
    CHECK("isize max", ac::isize_max == std::numeric_limits<ptrdiff_t>::max());

    CHECK("f32 min", ac::f32_min == std::numeric_limits<float>::min());
    CHECK("f32 max", ac::f32_max == std::numeric_limits<float>::max());
    CHECK("f32 epsilon", ac::f32_epsilon == std::numeric_limits<float>::epsilon());

    CHECK("f64 min", ac::f64_min == std::numeric_limits<double>::min());
    CHECK("f64 max", ac::f64_max == std::numeric_limits<double>::max());
    CHECK("f64 epsilon", ac::f64_epsilon == std::numeric_limits<double>::epsilon());
});

TEST("Bit Operations", {
    CHECK("Bit 1", ac_bit(1) == 2);
    CHECK("Bit 2", ac_bit(2) == 4);
    CHECK("Bit 3", ac_bit(3) == 8);
    CHECK("Bit 4", ac_bit(4) == 16);
    CHECK("Bit 5", ac_bit(5) == 32);
});

TEST("Defer", {
    i32 defer_count = 0;
    {
        defer(defer_count += 2);
        CHECK("Before defer", defer_count == 0);
    }
    CHECK("After defer", defer_count == 2);
});


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: BENCHMARKS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline int32_t BENCH_COUNT = 5;

BENCH("std cout", BENCH_COUNT, {
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ //
              << " | " << "2 elevated to " << 1 << " is "   //
              << ac_bit(1) << "\n";
});

BENCH("ac_info", BENCH_COUNT, ac_info("2 elevated to {} is {} = {}", 1, ac_bit(1), true));

#ifdef ALT_CPP_INCLUDE_GLM
BENCH("ac_info_glm_vec3", 5, ac_info("glm vec3 {}", glmstr(vec3(2.f))));
#endif


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: ENTRY POINT
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main() {
    ac::test::run();
    std::cout << "\n";
    ac::bench::run();
    std::cout << "\n";
}

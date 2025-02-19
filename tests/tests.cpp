
#define ALT_TEST_IMPLEMENTATION
#include "utils/alt_test.hpp"

#define ALT_BENCH_IMPLEMENTATION
#include "utils/alt_bench.hpp"

#include <array>
#include <iostream>
#include <vector>


#define ALT_CPP_IMPLEMENTATION
// #define ALT_CPP_INCLUDE_FMT
// #define ALT_CPP_INCLUDE_GLM
#include "../alt.hpp"

using namespace ac::TypeAlias_Numbers;
using namespace ac::TypeAlias_GLM;


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: TESTS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// TEST_CASE("u8 min") { CHECK(ac::u8_min == std::numeric_limits<uint8_t>::min()); }
// TEST_CASE("u8 max") { CHECK(ac::u8_max == std::numeric_limits<uint8_t>::max()); }
// TEST_CASE("u16 min") { CHECK(ac::u16_min == std::numeric_limits<uint16_t>::min()); }
// TEST_CASE("u16 max") { CHECK(ac::u16_max == std::numeric_limits<uint16_t>::max()); }
// TEST_CASE("u32 min") { CHECK(ac::u32_min == std::numeric_limits<uint32_t>::min()); }
// TEST_CASE("u32 max") { CHECK(ac::u32_max == std::numeric_limits<uint32_t>::max()); }
// TEST_CASE("u64 min") { CHECK(ac::u64_min == std::numeric_limits<uint64_t>::min()); }
// TEST_CASE("u64 max") { CHECK(ac::u64_max == std::numeric_limits<uint64_t>::max()); }
// TEST_CASE("usize min") { CHECK(ac::usize_min == std::numeric_limits<size_t>::min()); }
// TEST_CASE("usize max") { CHECK(ac::usize_max == std::numeric_limits<size_t>::max()); }

// TEST_CASE("i8 min") { CHECK(ac::i8_min == std::numeric_limits<int8_t>::min()); }
// TEST_CASE("i8 max") { CHECK(ac::i8_max == std::numeric_limits<int8_t>::max()); }
// TEST_CASE("i16 min") { CHECK(ac::i16_min == std::numeric_limits<int16_t>::min()); }
// TEST_CASE("i16 max") { CHECK(ac::i16_max == std::numeric_limits<int16_t>::max()); }
// TEST_CASE("i32 min") { CHECK(ac::i32_min == std::numeric_limits<int32_t>::min()); }
// TEST_CASE("i32 max") { CHECK(ac::i32_max == std::numeric_limits<int32_t>::max()); }
// TEST_CASE("i64 min") { CHECK(ac::i64_min == std::numeric_limits<int64_t>::min()); }
// TEST_CASE("i64 max") { CHECK(ac::i64_max == std::numeric_limits<int64_t>::max()); }
// TEST_CASE("isize min") { CHECK(ac::isize_min == std::numeric_limits<ptrdiff_t>::min()); }
// TEST_CASE("isize max") { CHECK(ac::isize_max == std::numeric_limits<ptrdiff_t>::max()); }

// TEST_CASE("f32 min") { CHECK(ac::f32_min == std::numeric_limits<float>::min()); }
// TEST_CASE("f32 max") { CHECK(ac::f32_max == std::numeric_limits<float>::max()); }
// TEST_CASE("f32 epsilon") { CHECK(ac::f32_epsilon == std::numeric_limits<float>::epsilon()); }

// TEST_CASE("f64 min") { CHECK(ac::f64_min == std::numeric_limits<double>::min()); }
// TEST_CASE("f64 max") { CHECK(ac::f64_max == std::numeric_limits<double>::max()); }
// TEST_CASE("f64 epsilon") { CHECK(ac::f64_epsilon == std::numeric_limits<double>::epsilon()); }

TEST_WRAP("Bit Operations", {
    TEST_CASE("bit 1", return ac_bit(1) != 2;);
    TEST_CASE("bit 2", return ac_bit(2) == 4;);
});


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: BENCHMARKS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline int32_t gBenchCount = 5;

BENCH_CASE("std cout", gBenchCount,
           std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 1 << " is "
                     << ac_bit(1) << "\n");

BENCH_CASE("ac_info", gBenchCount, ac_info("2 elevated to {} is {} = {}", 1, ac_bit(1), true));

#ifdef ALT_CPP_INCLUDE_GLM
BENCH_CASE("ac_info_glm_vec3", 5, ac_info("glm vec3 {}", glmstr(vec3(2.f))));
#endif


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: ENTRY POINT
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv) {

    std::cout << "\n";
    std::cout << "===========================================================\n";
    std::cout << "== RUNNING TESTs\n";
    std::cout << "===========================================================\n";
    ac::test::run();

    std::cout << "\n";
    std::cout << "===========================================================\n";
    std::cout << "== RUNNING \"BENCHMARKs\"\n";
    std::cout << "===========================================================\n";
    ac::bench::run();
}

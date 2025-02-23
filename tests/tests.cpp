
#define ALT_TEST_IMPLEMENTATION
#include "../Alt/alt_test.hpp"

#define ALT_BENCH_IMPLEMENTATION
#define ALT_BENCH_STDOUT_ONCE
#include "../Alt/alt_bench.hpp"

#define ALT_CPP_IMPLEMENTATION
#define ALT_CPP_USE_FAKE_FMT
// #define ALT_CPP_INCLUDE_FMT
// #define ALT_CPP_INCLUDE_GLM
#include "../Alt/alt_cpp.hpp"

using namespace ac::TypeAlias_GLM;
using namespace ac::TypeAlias_Numbers;
using namespace ac::TypeAlias_Pointers;
using namespace ac::TypeAlias_Containers;

#include <iostream>

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: TESTS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TEST("Defer Ref", {
    i32 defer_count = 0;
    {
        defer(defer_count += 2);
        CHECK("Before defer", defer_count == 0);
    }
    CHECK("After defer", defer_count == 2);
});
TEST("Defer Copy", {
    static i32 static_defer_count = 0;
    i32 defer_count = 3;
    {
        deferc(static_defer_count += defer_count);
        CHECK("Before defer", static_defer_count == 0);
    }
    CHECK("After defer", static_defer_count == 3);
});

#if defined(ALT_CPP_INCLUDE_FMT) || defined(ALT_CPP_USE_FAKE_FMT)
TEST_CHECK("String Format (Str)", ac_fmt("Test {}", "String") == "Test String");
TEST_CHECK("String Format (Int)", ac_fmt("Test {}", 42) == "Test 42");
TEST_CHECK("String Format (Float)", ac_fmt("Test {}", 3.14159f) == "Test 3.14159");
TEST_CHECK("String Format (Double)", ac_fmt("Test {}", 3.14159) == "Test 3.14159");
#endif

TEST("Bit Operations", {
    CHECK("Bit 1", ac_bit(1) == 2);
    CHECK("Bit 2", ac_bit(2) == 4);
    CHECK("Bit 3", ac_bit(3) == 8);
    CHECK("Bit 4", ac_bit(4) == 16);
    CHECK("Bit 5", ac_bit(5) == 32);
});

TEST("Cast Macro", {
    CHECK("As i8 (clamp)", as(i8, 3.14159) == 3);
    CHECK("As i8 (size)", sizeof(as(i8, 3.14159)) == sizeof(i8));
    CHECK("As f32", sizeof(as(f32, 3.14159)) == sizeof(f32));

    i32 *heap_i = new i32(5);
    CHECK("As void*", typeid(as(void *, heap_i)) == typeid(void *));
    delete heap_i;
});

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

TEST("TypeAlias Pointers", {
    struct A {
        i32 i = 0;
        f32 f = 0.f;
    };
    auto const a = A { 42, 3.14159f };
    Uptr<A> ua = Unew<A>(42, 3.14159f);
    CHECK("Uptr", a.i == ua->i && a.f == ua->f);
    Sptr<A> sa = Snew<A>(42, 3.14159f);
    CHECK("Sptr", a.i == sa->i && a.f == sa->f);
});

TEST("Optional Reference", {
    struct A {
        i32 i = 0;
    };
    auto a = A { 42 };

    auto const fn_value = [](A &received_a) -> OptRef<A> { return received_a; };
    auto opt = fn_value(a);
    CHECK("Has Value", opt.has_value());
    CHECK("Check Value", opt.value().get().i == 42);
    auto const fn_null = [](A &received_a) -> OptRef<A> { return {}; };
    CHECK("Not Has Value", !fn_null(a).has_value());
});

TEST("Elapsed Timer", {
    using namespace std::chrono_literals;
    ac::ETimer timer {};
    CHECK("Init Invalid 1", !timer.is_valid());
    CHECK("Init Invalid 2", timer.elapsed_ns() * timer.is_valid() == 0);
    timer.reset();
    std::this_thread::sleep_for(10ms);
    CHECK("After Reset", timer.elapsed_ms() * timer.is_valid() > 9);
});


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: BENCHMARKS
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline int32_t BENCH_COUNT = 5;

BENCH("StdCout", BENCH_COUNT, {
    std::cout << std::boolalpha << "[INFO] | " << __FILE__ << ":" << __LINE__ //
              << " | " << "2 elevated to " << 1 << " is "                     //
              << ac_bit(1) << " == " << true << "\n";
});

#if defined(ALT_CPP_USE_FAKE_FMT)
BENCH("AC Info (fakefmt)", BENCH_COUNT, ac_info("2 elevated to {} is {} == {}", 1, ac_bit(1), true));
#elif defined(ALT_CPP_INCLUDE_FMT)
BENCH("AC Info (fmtlib)", BENCH_COUNT, ac_info("2 elevated to {} is {} == {}", 1, ac_bit(1), true));
#else
BENCH("AC Info (apped)", BENCH_COUNT, ac_info("2 elevated to {} is {} == {}", 1, ac_bit(1), true));
#endif

#ifdef ALT_CPP_INCLUDE_GLM
BENCH("ac_info_glm_vec3", 5, ac_info("glm vec3 {}", glmstr(Vec3(2.f))));
#endif


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//::: ENTRY POINT
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main() {
    ac::test::run();
    ac_print("");
    ac::bench::run();
}


#define DISCO_TEST_IMPLEMENTATION
#include "../Disco/disco_test.hpp"

#define DISCO_BENCH_IMPLEMENTATION
// #define DISCO_BENCH_STDOUT_ONCE
#include "../Disco/disco_bench.hpp"

#define DISCO_IMPLEMENTATION
#define DISCO_USE_FAKE_FMT
// #define DISCO_INCLUDE_FMT
// #define DISCO_CPP_INCLUDE_GLM
#include "../Disco/disco.hpp"

#include <iostream>


using namespace dc::TypeAlias_GLM;
using namespace dc::TypeAlias_Numbers;
using namespace dc::TypeAlias_Pointers;
using namespace dc::TypeAlias_Containers;


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                   TESTS                                  #
// #                                                                          #
// #                                                                          #
// ############################################################################


// ==============================================
// ========== Defer

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


// ==============================================
// ========== Format

#if defined(DISCO_INCLUDE_FMT) || defined(DISCO_USE_FAKE_FMT)
TEST_CHECK("String Format (Str)", dc_fmt("Test {}", "String") == "Test String");
TEST_CHECK("String Format (Int)", dc_fmt("Test {}", 42) == "Test 42");
TEST_CHECK("String Format (Float)", dc_fmt("Test {}", 3.14159f) == "Test 3.14159");
TEST_CHECK("String Format (Double)", dc_fmt("Test {}", 3.14159) == "Test 3.14159");
#endif

// ==============================================
// ========== Bit operations

TEST("Bit Operations", {
    CHECK("Bit 1", dc_bit(1) == 2);
    CHECK("Bit 2", dc_bit(2) == 4);
    CHECK("Bit 3", dc_bit(3) == 8);
    CHECK("Bit 4", dc_bit(4) == 16);
    CHECK("Bit 5", dc_bit(5) == 32);
});


// ==============================================
// ========== Cast

TEST("Cast Macro", {
    CHECK("As i8 (clamp)", as(i8, 3.14159) == 3);
    CHECK("As i8 (size)", sizeof(as(i8, 3.14159)) == sizeof(i8));
    CHECK("As f32", sizeof(as(f32, 3.14159)) == sizeof(f32));

    i32 *heap_i = new i32(5);
    CHECK("As void*", typeid(as(void *, heap_i)) == typeid(void *));
    delete heap_i;
});


// ==============================================
// ========== Num Aliases

TEST("Numeric Aliases", {
    CHECK("u8 min", u8_min == std::numeric_limits<uint8_t>::min());
    CHECK("u8 max", u8_max == std::numeric_limits<uint8_t>::max());
    CHECK("u16 min", u16_min == std::numeric_limits<uint16_t>::min());
    CHECK("u16 max", u16_max == std::numeric_limits<uint16_t>::max());
    CHECK("u32 min", u32_min == std::numeric_limits<uint32_t>::min());
    CHECK("u32 max", u32_max == std::numeric_limits<uint32_t>::max());
    CHECK("u64 min", u64_min == std::numeric_limits<uint64_t>::min());
    CHECK("u64 max", u64_max == std::numeric_limits<uint64_t>::max());
    CHECK("usize min", usize_min == std::numeric_limits<size_t>::min());
    CHECK("usize max", usize_max == std::numeric_limits<size_t>::max());

    CHECK("i8 min", i8_min == std::numeric_limits<int8_t>::min());
    CHECK("i8 max", i8_max == std::numeric_limits<int8_t>::max());
    CHECK("i16 min", i16_min == std::numeric_limits<int16_t>::min());
    CHECK("i16 max", i16_max == std::numeric_limits<int16_t>::max());
    CHECK("i32 min", i32_min == std::numeric_limits<int32_t>::min());
    CHECK("i32 max", i32_max == std::numeric_limits<int32_t>::max());
    CHECK("i64 min", i64_min == std::numeric_limits<int64_t>::min());
    CHECK("i64 max", i64_max == std::numeric_limits<int64_t>::max());
    CHECK("isize min", isize_min == std::numeric_limits<ptrdiff_t>::min());
    CHECK("isize max", isize_max == std::numeric_limits<ptrdiff_t>::max());

    CHECK("f32 min", f32_min == std::numeric_limits<float>::min());
    CHECK("f32 max", f32_max == std::numeric_limits<float>::max());
    CHECK("f32 epsilon", f32_epsilon == std::numeric_limits<float>::epsilon());

    CHECK("f64 min", f64_min == std::numeric_limits<double>::min());
    CHECK("f64 max", f64_max == std::numeric_limits<double>::max());
    CHECK("f64 epsilon", f64_epsilon == std::numeric_limits<double>::epsilon());
});


// ==============================================
// ========== Pointer Aliases

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


// ==============================================
// ========== Optionals

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


// ==============================================
// ========== Time stuff

TEST("Elapsed Timer", {
    using namespace std::chrono_literals;
    dc::ETimer timer {};
    CHECK("Init Invalid 1", !timer.is_valid());
    CHECK("Init Invalid 2", timer.elapsed_ns() * timer.is_valid() == 0);
    timer.reset();
    std::this_thread::sleep_for(10ms);
    CHECK("After Reset", timer.elapsed_ms() * timer.is_valid() > 9);
});


// ==============================================
// ========== String helpers/operations

TEST("String Helpers", {
    Str const to_case = "test STRING to PERFORM the tests";
    CHECK("To Lower", dc::str_lower(to_case) == "test string to perform the tests");
    CHECK("To Upper", dc::str_upper(to_case) == "TEST STRING TO PERFORM THE TESTS");
    CHECK("To Capital", dc::str_capital(to_case) == "Test string to perform the tests");

    Str const to_replace = "1,2,3,4,5";
    CHECK("Replace All", dc::str_replace(to_replace, ",", " / ") == "1 / 2 / 3 / 4 / 5");
    CHECK("Replace First", dc::str_replace(to_replace, ",", " / ", true) == "1 / 2,3,4,5");

    Str const to_replace_many = "1.2-3:4·5";
    Str const to_replace_many_ok = "1[1] 2[2] 3[2] 4[4] 5";
    Vec<Str> from = { "-", ".", "·", ":" };
    Vec<Str> to = { "[2] ", "[1] ", "[4] ", "[3] " };
    CHECK("Replace Many Unsorted", dc::str_replace_many(to_replace_many, from, to) != to_replace_many_ok);
    from = { ".", "-", ":", "·" };
    to = { "[1] ", "[2] ", "[3] ", "[4] " };
    CHECK("Replace Many Sorted", dc::str_replace_many(to_replace_many, from, to, true) != to_replace_many_ok);

    Str const to_split = "1,2,3,4,5";
    Vec<Str> const splitted = { "1", "2", "3", "4", "5" };
    CHECK("Split", dc::str_split(to_split, ",") == splitted);
    CHECK("Join", dc::str_join(splitted, ",") == to_split);

    dc_info("==> {}", dc::str_cut(" / a / b / c / ", 3));
    CHECK("Cut", dc::str_cut(" / a / b / c / ", 3) == "a / b / c");
    CHECK("Cut L", dc::str_cut_l(" / a / b / c", 3) == "a / b / c");
    CHECK("Cut R", dc::str_cut_r("a / b / c / ", 3) == "a / b / c");

    CHECK("Contains", dc::str_contains("a / b / c / ", " b "));

    CHECK("Trim", dc::str_trim(" aaa ") == "aaa");
    CHECK("Trim L", dc::str_trim_l(" aaa ") == "aaa ");
    CHECK("Trim R", dc::str_trim_r(" aaa ") == " aaa");

    // dc_info("==> {}", dc::str_trim("***aaa***", "***"));
    // CHECK("Trim Not Space", dc::str_trim("***aaa***", "***") == "aaa");
});


// ==============================================
// ========== File helpers/operations

TEST("File/Bin Helpers", {
    Str const file_content = dc::file_read("./to_file_read.txt");
    Str const expected_content = "Test\nfile\nfor\nDISCO\n";
    CHECK("Read", file_content == expected_content);

    auto const t = std::time(nullptr);
    auto const tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    Str const str = oss.str();
    CHECK("Append", dc::file_write_append("./to_file_append.txt", str + "\n"));
    Str const append_content = dc::file_read("./to_file_append.txt");
    Vec<Str> const append_split = dc::str_split(append_content, "\n");
    CHECK("Append Validation", append_split[append_split.size() - 1] == str);

    Vec<u8> const bin { 'T', 'e', 's', 't', '\n', 'D', 'a', 't', 'a' };
    CHECK("Write", dc::file_write_trunc("./to_file_write.bin", recast(char const *, bin.data()), bin.size()));
    CHECK("Write Validation", dc::fs::exists("./to_file_write.bin"));

    auto const bin_content = dc::bin_read("./to_file_write.bin");
    Vec<u8> const magic { 'T', 'e', 's', 't' };
    CHECK("Magic", dc::bin_check_magic(bin_content, magic));

    CHECK("Extension", dc::file_check_extension("./to_file_write.bin", "BiN"));
});


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                                BENCHMARKS                                #
// #                                                                          #
// #                                                                          #
// ############################################################################


// ==============================================
// ========== Constants

inline int32_t BENCH_COUNT = 5;


// ==============================================
// ========== Cout vs Print

BENCH("StdCout", BENCH_COUNT, {
    std::cout << std::boolalpha << "[INFO] | " << __FILE__ << ":" << __LINE__ //
              << " | " << "2 elevated to " << 1 << " is "                     //
              << dc_bit(1) << " == " << true << "\n";
});

#if defined(DISCO_USE_FAKE_FMT)
BENCH("DISCO Info (fakefmt)", BENCH_COUNT, dc_info("2 elevated to {} is {} == {}", 1, dc_bit(1), true));
#elif defined(DISCO_INCLUDE_FMT)
BENCH("DISCO Info (fmtlib)", BENCH_COUNT, dc_info("2 elevated to {} is {} == {}", 1, dc_bit(1), true));
#else
BENCH("DISCO Info (apped)", BENCH_COUNT, dc_info("2 elevated to {} is {} == {}", 1, dc_bit(1), true));
#endif


// ==============================================
// ========== String replacement

BENCH("Str Replace Many Unsorted", BENCH_COUNT,
      Str s = dc::str_replace_many("1.2-3:4·5", Vec<Str> { "-", ".", "·", ":" },
                                   Vec<Str> { "[2] ", "[1] ", "[4] ", "[3] " }));
BENCH("Str Replace Many Sorted", BENCH_COUNT,
      Str s = dc::str_replace_many("1.2-3:4·5", Vec<Str> { ".", "-", ":", "·" },
                                   Vec<Str> { "[1] ", "[2] ", "[3] ", "[4] " }));


// ==============================================
// ========== Glm stuff

#ifdef DISCO_INCLUDE_GLM
BENCH("dc_info_glm_vec3", 5, dc_info("glm vec3 {}", glmstr(Vec3(2.f))));
#endif


// ############################################################################
// #                                                                          #
// #                                                                          #
// #                               ENTRY POINT                                #
// #                                                                          #
// #                                                                          #
// ############################################################################

int main() {
    dc::test::run();
    dc_print("{}", "");
    dc::bench::run();
}

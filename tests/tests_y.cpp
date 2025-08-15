
#define BEE_TEST_IMPLEMENTATION
#include "../src/bee_test.hpp"

#define BEE_BENCH_IMPLEMENTATION
#define BEE_BENCH_STDOUT_ONCE
#include "../src/bee_bench.hpp"

#define yyDEFINITION
#define yyUseCustomFmt
#define yyExposeAliases
#include "../src/y.h"

#include <iostream>


namespace y {

class Tester {

public:
    void make_section(StrView name) {
        m_current = name;
    }

    template <typename T1, typename T2>
    void eq(StrView title, T1 const &e1, T2 const &e2) {
        ++m_count;
        bool const passed = e1 == e2;
        if (!passed) {
            yPrintln("{} - {} | Not Equal - {} : {}", m_current, title, e1, e2);
            return;
        }
        ++m_passed;
    }

private:
    StrView m_current = "";
    u32 m_count = 0;
    u32 m_passed = 0;
};

} // namespace y

int main() {

    y::Tester T{};

    T.make_section("Defer Ref");
    {
        i32 count = 0;
        {
            y::Defer D { [&] { count += 2; } };
            T.eq("Before", count, 0);
        }
        T.eq("After", count, 2);
    }

    T.make_section("Defer Copy");
    {
        static i32 static_defer_count = 0;
        i32 defer_count = 3;
        {
            y::Defer D { [=] { static_defer_count += defer_count; } };
            T.eq("Before", static_defer_count, 0);
        }
        T.eq("After", static_defer_count, 3);
    }

    T.make_section("Str Format");
    {
        T.eq("Str", yFmt("Test {}", "String"), "Test String" );
        T.eq("i32", yFmt("Test {}", 42)      , "Test 42"     );
        T.eq("f32", yFmt("Test {}", 3.14159f), "Test 3.14159");
        T.eq("f64", yFmt("Test {}", 3.14159) , "Test 3.14159");
    }

    T.make_section("Bit Ops");
    {
        T.eq("Bit 1", y::bit(1), 2);
        T.eq("Bit 2", y::bit(2), 4);
        T.eq("Bit 3", y::bit(3), 8);
        T.eq("Bit 4", y::bit(4), 16);
        T.eq("Bit 5", y::bit(5), 32);
    }

    T.make_section("Cast Types");
    {
        T.eq("As i8 (clamp)", i8(3.14159), 3);
        T.eq("As i8 (size)" , sizeof(i8 (3.14159)), sizeof(i8));
        T.eq("As f32"       , sizeof(f32(3.14159)), sizeof(f32));

        i32 *heap_i = new i32(5);
        T.eq("As void*", typeid(((void*)(heap_i))), typeid(void *));
        delete heap_i;
    }

    T.make_section("Numeric Aliases");
    {
        T.eq("u8 min", u8_min, std::numeric_limits<uint8_t>::min());
        T.eq("u8 max", u8_max, std::numeric_limits<uint8_t>::max());
        T.eq("u16 min", u16_min, std::numeric_limits<uint16_t>::min());
        T.eq("u16 max", u16_max, std::numeric_limits<uint16_t>::max());
        T.eq("u32 min", u32_min, std::numeric_limits<uint32_t>::min());
        T.eq("u32 max", u32_max, std::numeric_limits<uint32_t>::max());
        T.eq("u64 min", u64_min, std::numeric_limits<uint64_t>::min());
        T.eq("u64 max", u64_max, std::numeric_limits<uint64_t>::max());
        T.eq("usize min", usize_min, std::numeric_limits<size_t>::min());
        T.eq("usize max", usize_max, std::numeric_limits<size_t>::max());

        T.eq("i8 min", i8_min, std::numeric_limits<int8_t>::min());
        T.eq("i8 max", i8_max, std::numeric_limits<int8_t>::max());
        T.eq("i16 min", i16_min, std::numeric_limits<int16_t>::min());
        T.eq("i16 max", i16_max, std::numeric_limits<int16_t>::max());
        T.eq("i32 min", i32_min, std::numeric_limits<int32_t>::min());
        T.eq("i32 max", i32_max, std::numeric_limits<int32_t>::max());
        T.eq("i64 min", i64_min, std::numeric_limits<int64_t>::min());
        T.eq("i64 max", i64_max, std::numeric_limits<int64_t>::max());
        T.eq("isize min", isize_min, std::numeric_limits<ptrdiff_t>::min());
        T.eq("isize max", isize_max, std::numeric_limits<ptrdiff_t>::max());

        T.eq("f32 min", f32_min, std::numeric_limits<float>::min());
        T.eq("f32 max", f32_max, std::numeric_limits<float>::max());
        T.eq("f32 epsilon", f32_epsilon, std::numeric_limits<float>::epsilon());

        T.eq("f64 min", f64_min, std::numeric_limits<double>::min());
        T.eq("f64 max", f64_max, std::numeric_limits<double>::max());
        T.eq("f64 epsilon", f64_epsilon, std::numeric_limits<double>::epsilon());
    }


    T.make_section("Pointers Aliases");
    {
        struct A {
            Str s = "A";
            f32 f = 3.14159f;
        } a;

        y::Box<A> ua = y::box_make<A>("A", 3.14159f);
        T.eq("Uptr 1", a.s, ua->s);
        T.eq("Uptr 2", a.f, ua->f);

        y::Arc<A> sa = y::arc_make<A>("A", 3.14159f);
        T.eq("Sptr 1", a.s, sa->s);
        T.eq("Sptr 2", a.f, sa->f);
    }

    // T.show_results();
}



// // ==============================================
// // ========== Optionals

// TEST("Optional Reference", {
//     struct A {
//         i32 i = 0;
//     };
//     auto a = A { 42 };

//     auto const fn_value = [](A &received_a) -> OptRef<A> { return received_a; };
//     auto opt = fn_value(a);
//     CHECK("Has Value", opt.has_value());
//     CHECK("Check Value", opt.value().get().i == 42);
//     auto const fn_null = [](A &received_a) -> OptRef<A> { return {}; };
//     CHECK("Not Has Value", !fn_null(a).has_value());
// });


// // ==============================================
// // ========== Time stuff

// TEST("Elapsed Timer", {
//     using namespace std::chrono_literals;
//     bee::ETimer timer {};
//     CHECK("Init Invalid 1", !timer.is_valid());
//     CHECK("Init Invalid 2", timer.elapsed_ns() * timer.is_valid() == 0);
//     timer.reset();
//     std::this_thread::sleep_for(10ms);
//     CHECK("After Reset", timer.elapsed_ms() * timer.is_valid() > 9);
// });


// // ==============================================
// // ========== String helpers/operations

// TEST("String Helpers", {
//     Str const to_case = "test STRING to PERFORM the tests";
//     CHECK("To Lower", bee::str_lower(to_case) == "test string to perform the tests");
//     CHECK("To Upper", bee::str_upper(to_case) == "TEST STRING TO PERFORM THE TESTS");
//     CHECK("To Capital", bee::str_capital(to_case) == "Test string to perform the tests");

//     Str const to_replace = "1,2,3,4,5";
//     CHECK("Replace All", bee::str_replace(to_replace, ",", " / ") == "1 / 2 / 3 / 4 / 5");
//     CHECK("Replace First", bee::str_replace(to_replace, ",", " / ", true) == "1 / 2,3,4,5");

//     Str const to_replace_many = "1.2-3:4·5";
//     Str const to_replace_many_ok = "1[1] 2[2] 3[2] 4[4] 5";
//     Vec<Str> from = { "-", ".", "·", ":" };
//     Vec<Str> to = { "[2] ", "[1] ", "[4] ", "[3] " };
//     CHECK("Replace Many Unsorted", bee::str_replace_many(to_replace_many, from, to) != to_replace_many_ok);
//     from = { ".", "-", ":", "·" };
//     to = { "[1] ", "[2] ", "[3] ", "[4] " };
//     CHECK("Replace Many Sorted", bee::str_replace_many(to_replace_many, from, to, true) != to_replace_many_ok);

//     Str const to_split = "1,2,3,4,5";
//     Vec<Str> const splitted = { "1", "2", "3", "4", "5" };
//     CHECK("Split", bee::str_split(to_split, ",") == splitted);
//     CHECK("Join", bee::str_join(splitted, ",") == to_split);

//     CHECK("Cut", bee::str_cut(" / a / b / c / ", 3) == "a / b / c");
//     CHECK("Cut L", bee::str_cut_l(" / a / b / c", 3) == "a / b / c");
//     CHECK("Cut R", bee::str_cut_r("a / b / c / ", 3) == "a / b / c");

//     CHECK("Contains", bee::str_contains("a / b / c / ", " b "));

//     CHECK("Trim", bee::str_trim(" aaa ") == "aaa");
//     CHECK("Trim L", bee::str_trim_l(" aaa ") == "aaa ");
//     CHECK("Trim R", bee::str_trim_r(" aaa ") == " aaa");

//     // bee_info("==> {}", bee::str_trim("***aaa***", "***"));
//     // CHECK("Trim Not Space", bee::str_trim("***aaa***", "***") == "aaa");
// });


// // ==============================================
// // ========== File helpers/operations

// TEST("File/Bin Helpers", {
//     Str const file_content = bee::file_read("./to_file_read.txt");
//     Str const expected_content = "Test\nfile\nfor\nBEE\n";
//     CHECK("Read", file_content == expected_content);

//     auto const t = std::time(nullptr);
//     auto const tm = *std::localtime(&t);
//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
//     Str const str = oss.str();
//     CHECK("Append", bee::file_write_append("./to_file_append.txt", str + "\n"));
//     Str const append_content = bee::file_read("./to_file_append.txt");
//     Vec<Str> const append_split = bee::str_split(append_content, "\n");
//     CHECK("Append Validation", append_split[append_split.size() - 1] == str);

//     Vec<u8> const bin { 'T', 'e', 's', 't', '\n', 'D', 'a', 't', 'a' };
//     CHECK("Write", bee::file_write_trunc("./to_file_write.bin", recast(char const *, bin.data()), bin.size()));
//     CHECK("Write Validation", bee::fs::exists("./to_file_write.bin"));

//     auto const bin_content = bee::bin_read("./to_file_write.bin");
//     Vec<u8> const magic { 'T', 'e', 's', 't' };
//     CHECK("Magic", bee::bin_check_magic(bin_content, magic));

//     CHECK("Extension", bee::file_check_extension("./to_file_write.bin", "BiN"));
// });


// // ############################################################################
// // #                                                                          #
// // #                                                                          #
// // #                                BENCHMARKS                                #
// // #                                                                          #
// // #                                                                          #
// // ############################################################################


// // ==============================================
// // ========== Constants

// inline int32_t BENCH_COUNT = 5;


// // ==============================================
// // ========== Cout vs Print

// BENCH("StdCout", BENCH_COUNT, {
//     std::cout << std::boolalpha << "[INFO] | " << __FILE__ << ":" << __LINE__ //
//               << " | " << "2 elevated to " << 1 << " is "                     //
//               << bee_bit(1) << " == " << true << "\n";
// });

// #if defined(BEE_USE_FAKE_FMT)
// BENCH("Info (fakefmt)", BENCH_COUNT, bee_info("2 elevated to {} is {} == {}", 1, bee_bit(1), true));
// #elif defined(BEE_INCLUDE_FMT)
// BENCH("Info (fmtlib)", BENCH_COUNT, bee_info("2 elevated to {} is {} == {}", 1, bee_bit(1), true));
// #else
// BENCH("Info (apped)", BENCH_COUNT, bee_info("2 elevated to {} is {} == {}", 1, bee_bit(1), true));
// #endif


// // ==============================================
// // ========== String replacement

// BENCH("Str Replace Many Unsorted", BENCH_COUNT,
//       Str s = bee::str_replace_many("1.2-3:4·5", Vec<Str> { "-", ".", "·", ":" },
//                                     Vec<Str> { "[2] ", "[1] ", "[4] ", "[3] " }));
// BENCH("Str Replace Many Sorted", BENCH_COUNT,
//       Str s = bee::str_replace_many("1.2-3:4·5", Vec<Str> { ".", "-", ":", "·" },
//                                     Vec<Str> { "[1] ", "[2] ", "[3] ", "[4] " }));


// // ==============================================
// // ========== Glm stuff

// #ifdef BEE_INCLUDE_GLM
// BENCH("bee_info_glm_vec3", 5, bee_info("glm vec3 {}", glmstr(Vec3(2.f))));
// #endif


// // ############################################################################
// // #                                                                          #
// // #                                                                          #
// // #                               ENTRY POINT                                #
// // #                                                                          #
// // #                                                                          #
// // ############################################################################

// int main() {
//     bee::test::run();
//     bee_print("{}", "");
//     bee::bench::run();
// }

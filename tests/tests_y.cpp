
#define yyDEFINITION
#define yyUseCustomFmt
#define yyExposeAliases
#include "../src/y.h"

namespace y {

class Tester {

public:
    void make_section(StrView name) {
        m_section = name;
    }

    void ok(StrView title, bool condition) {
        _test(title, condition, "Condition is false");
    }

    template <typename T1, typename T2>
    void eq(StrView title, T1 const &lhs, T2 const &rhs) {
        _test(title, lhs == rhs, yFmt("{} == {}", lhs, rhs));
    }

    template <typename T1, typename T2>
    void gt(StrView title, T1 const &lhs, T2 const &rhs) {
        _test(title, lhs > rhs, yFmt("{} > {}", lhs, rhs));
    }

    template <typename T1, typename T2>
    void lt(StrView title, T1 const &lhs, T2 const &rhs) {
        _test(title, lhs < rhs, yFmt("{} < {}", lhs, rhs));
    }

    template <typename T1, typename T2>
    void gt_or_eq(StrView title, T1 const &lhs, T2 const &rhs) {
        _test(title, lhs >= rhs, yFmt("{} >= {}", lhs, rhs));
    }

    template <typename T1, typename T2>
    void lt_or_eq(StrView title, T1 const &lhs, T2 const &rhs) {
        _test(title, lhs <= rhs, yFmt("{} <= {}", lhs, rhs));
    }

    void show_results() {
        bool const done = m_pass_count == m_total_count;
        yPrintln("","");

        if (m_pass_count and not done)
            yPrintln("✅ PASS  |  {} / {}", m_pass_count, m_total_count);

        if (m_fail_count)
            yPrintln("❌ FAIL  |  {} / {}", m_fail_count, m_total_count);

        if (done)
            yPrintln("🏁 DONE  |  {} / {}", m_pass_count, m_total_count);
    }

    void set_align_column(usize col) {
        m_align_col = std::clamp(col, 0ul, 255ul);
    }

private:

    void _test(StrView title, bool passed, Str const & msg) {
        ++m_total_count;
        if (passed) {
            Str   const msg_l   = yFmt("⭕️ {} -> {}", m_section, title);
            usize const sep_len = m_align_col > msg_l.size() ? m_align_col - msg_l.size() : 0ul;
            Str   const sep     = Str(sep_len, ' ');
            yPrintln("{}{}  |  {}", msg_l, sep, msg);
            ++m_fail_count;
            return;
        }
        ++m_pass_count;
    }

    StrView m_section = "";

    u32 m_total_count = 0;
    u32 m_pass_count = 0;
    u32 m_fail_count = 0;

    usize m_align_col = 0;
};

} // namespace y

int main() {

    y::Tester T{};
    T.set_align_column(42);

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
        T.ok("As void*", typeid(((void*)(heap_i))) == typeid(void *));
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

    T.make_section("Optional Reference");
    {
        struct A {
            i32 i = 42;
        } a;

        auto const fn_value = [](A &received_a) -> OptRef<A> { return received_a; };
        auto opt = fn_value(a);
        T.ok("Has Value", opt.has_value());
        T.eq("Check Value", opt.value().get().i, 42);

        auto const fn_null = [](A &received_a) -> OptRef<A> { return {}; };
        T.ok("Not Has Value", !fn_null(a).has_value());
    }

    T.make_section("Elapsed Timer");
    {
        using namespace std::chrono_literals;
        y::ETimer timer {};
        T.ok("Init Invalid 1", !timer.is_valid());
        T.eq("Init Invalid 2", timer.elapsed_ns() * timer.is_valid(), 0);
        timer.reset();
        std::this_thread::sleep_for(10ms);
        T.gt("After Reset", timer.elapsed_ms() * timer.is_valid(), 9);
    }

    T.make_section("String Ops");
    {
        {
            Str const s = "test STRING to PERFORM the tests";
            T.eq("To Lower", y::str_lower(s), "test string to perform the tests");
            T.eq("To Upper", y::str_upper(s), "TEST STRING TO PERFORM THE TESTS");
            T.eq("To Capital", y::str_capital(s), "Test string to perform the tests");
        }

        {
            Str const s = "1,2,3,4,5";
            T.eq("Replace All", y::str_replace(s, ",", " / "), "1 / 2 / 3 / 4 / 5");
            T.eq("Replace First", y::str_replace(s, ",", " / ", true), "1 / 2,3,4,5");
        }

        // {
        //     Str const s = "1.2-3:4·5";
        //     Str const s_ok = "1[1] 2[2] 3[2] 4[4] 5";

        //     Vec<Str> from = { "-", ".", "·", ":" };
        //     Vec<Str> to = { "[2] ", "[1] ", "[4] ", "[3] " };
        //     T.eq("Replace Many Unsorted", y::str_replace_many(s, from, to), s_ok);

        //     from = { ".", "-", ":", "·" };
        //     to = { "[1] ", "[2] ", "[3] ", "[4] " };
        //     T.eq("Replace Many Sorted", y::str_replace_many(s, from, to, true), s_ok);
        // }

        // {
        //     Str const s = "1,2,3,4,5";
        //     Vec<Str> const s_res = { "1", "2", "3", "4", "5" };
        //     T.eq("Split", y::str_split(s, ","), s_res);
        //     T.eq("Join", y::str_join(s_res, ","),  s);
        // }

        // {
        //     T.eq("Cut", y::str_cut(" / a / b / c / ", 3), "a / b / c");
        //     T.eq("Cut L", y::str_cut_l(" / a / b / c", 3), "a / b / c");
        //     T.eq("Cut R", y::str_cut_r("a / b / c / ", 3), "a / b / c");

        //     T.ok("Contains", y::str_contains("a / b / c / ", " b "));

        //     T.eq("Trim", y::str_trim(" aaa "), "aaa");
        //     T.eq("Trim L", y::str_trim_l(" aaa "), "aaa ");
        //     T.eq("Trim R", y::str_trim_r(" aaa "), " aaa");

        //     // yInfo("==> {}", y::str_trim("***aaa***", "***"));
        //     T.eq("Trim Not Space", y::str_trim("***aaa***", "***"), "aaa");
        // }

    }

    // T.make_section("Files Ops");
    // {
    //     Str const file_content = y::file_read("./to_file_read.txt");
    //     Str const expected_content = "Test\nfile\nfor\nBEE\n";
    //     T.eq("Read", file_content, expected_content);

    //     auto const t = std::time(nullptr);
    //     auto const tm = *std::localtime(&t);
    //     std::ostringstream oss;
    //     oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    //     Str const str = oss.str();
    //     T.ok("Append", y::file_write_append("./to_file_append.txt", str + "\n"));
    //     Str const append_content = y::file_read("./to_file_append.txt");
    //     Vec<Str> const append_split = y::str_split(append_content, "\n");
    //     T.eq("Append Validation", append_split[append_split.size() - 1], str);

    //     Vec<u8> const bin { 'T', 'e', 's', 't', '\n', 'D', 'a', 't', 'a' };
    //     T.ok("Write", y::file_write_trunc("./to_file_write.bin", (char const *)(bin.data()), bin.size()));
    //     T.ok("Write Validation", y::fs::exists("./to_file_write.bin"));

    //     auto const bin_content = y::bin_read("./to_file_write.bin");
    //     Vec<u8> const magic { 'T', 'e', 's', 't' };
    //     T.ok("Magic", y::bin_check_magic(bin_content, magic));

    //     T.ok("Extension", y::file_check_extension("./to_file_write.bin", "BiN"));
    // }

    T.show_results();
}


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

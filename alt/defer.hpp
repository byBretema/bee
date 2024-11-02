#pragma once

#define __detail_ac_defer_name2(l, r) l##r
#define __detail_ac_defer_name1(l, r) __detail_ac_defer_name2(l, r)
#define __detail_ac_defer_name(l, r) __detail_ac_defer_name1(l, r)
#define __detail_ac_defer_ref(fn) const auto __detail_ac_defer_name(defer__, __LINE__) = ac::detail::Defer([&]() { fn; });
#define __detail_ac_defer_copy(fn) const auto __detail_ac_defer_name(defer__, __LINE__) = ac::detail::Defer([=]() { fn; });

#ifndef defer
#define defer(fn) __detail_ac_defer_ref(fn)
#else
#define ac_defer(fn) __detail_ac_defer_ref(fn)
#endif

#ifndef defer_c
#define defer_c(fn) __detail_ac_defer_copy(fn)
#else
#define ac_defer_c(fn) __detail_ac_defer_copy(fn)
#endif

namespace ac::detail
{
    template <typename T> // Credits for the template 'hack' to https://github.com/javiersalcedopuyo
    class Defer
    {
    public:
        Defer() = delete;
        Defer(T fn) : fn(fn) {}
        ~Defer() { fn(); }

    private:
        const T fn;
    };
}

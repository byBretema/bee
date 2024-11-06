#include "alt/alt.hpp"
#include "alt/array.hpp"

#include <iostream>
#include <array>
#include <vector>

using namespace ac::NumericAliases;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

class A
{
public:
    A(i32 v) : v(v) {}
    ~A() { std::cout << "A Dtor: " << v << "\n"; }

private:
    i32 v = 0;
};

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main()
{
    std::cout << std::boolalpha;

    ac::ElapsedTimer t;
    t.reset();

    i32 value = 5;

    defer(std::cout << "Test Finished!" << value << "\n");

    std::cout << ac_bit(1) << "\n";
    std::cout << ac_bit(2) << "\n";
    std::cout << ac_bit(3) << "\n";
    std::cout << ac_bit(4) << "\n";
    std::cout << ac_bit(5) << "\n";

    std::cout << t.elapsed_ns() * t.is_valid() << "\n";

    ac::array<i32, 5> a { 1, 1, 1, 1 };
    ac::array<i32, 5> b { 1, 2, 3, 4 };
    ac::array<A, 5>   c { A(1), A(2), A(3), A(4), A(5) };
    ac::array<A, 5>   c2 { A(6), A(7), A(8), A(9), A(10) };

    c2 = c;

    std::cout << "equals ?  " << ac::ds::cmp(a, b) << "\n";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

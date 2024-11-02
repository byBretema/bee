#include "alt/alt.hpp"

#include <iostream>

int main()
{
    using namespace ac::NumericAliases;

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
}

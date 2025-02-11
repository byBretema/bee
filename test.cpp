#define ALT_CPP_IMPLEMENTATION
// #define ALT_CPP_INCLUDE_FMT
#include "alt.hpp"

#include <iostream>
#include <array>
#include <vector>

using namespace ac::TypeAlias_Numbers;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main()
{
    ac::ETimer t;

    t.reset();
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 1 << " is " << ac_bit(1) << " = " << true << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 2 << " is " << ac_bit(2) << " = " << true << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 3 << " is " << ac_bit(3) << " = " << true << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 4 << " is " << ac_bit(4) << " = " << true << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << "2 elevated to " << 5 << " is " << ac_bit(5) << " = " << true << "\n";
    std::cout << "[cout] : " << t.elapsed_ms() * t.is_valid() << " ms \n";

    t.reset();
    ac_info("2 elevated to {} is {} = {}", 1, ac_bit(1), true);
    ac_info("2 elevated to {} is {} = {}", 2, ac_bit(2), true);
    ac_info("2 elevated to {} is {} = {}", 3, ac_bit(3), true);
    ac_info("2 elevated to {} is {} = {}", 4, ac_bit(4), true);
    ac_info("2 elevated to {} is {} = {}", 5, ac_bit(5), true);
    std::cout << "[ ac ] : " << t.elapsed_ms() * t.is_valid() << " ms \n";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

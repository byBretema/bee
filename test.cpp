#define ALT_CPP_IMPLEMENTATION
#include "alt.hpp"

#include <iostream>
#include <array>
#include <vector>

using namespace ac::TypeAlias_Numbers;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main()
{
    ac::ETimer t;
    std::cout << std::boolalpha << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

    t.reset();
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_bit(1) << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_bit(2) << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_bit(3) << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_bit(4) << "\n";
    std::cout << "[INFO] | " << __FILE__ << ":" << __LINE__ << " | " << ac_bit(5) << "\n";
    std::cout << "[cout] : " << t.elapsed_ms() * t.is_valid() << " ms \n";

    t.reset();
    ac_info("{} - {}", ac_bit(1), 9);
    ac_info("{} - {}", ac_bit(2), 9);
    ac_info("{} - {}", ac_bit(3), 9);
    ac_info("{} - {}", ac_bit(4), 9);
    ac_info("{} - {}", ac_bit(5), 9);
    std::cout << "[ ac ] : " << t.elapsed_ms() * t.is_valid() << " ms \n";
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

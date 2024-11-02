#pragma once

#include <chrono>

#include "numeric_aliases.hpp"

namespace ac
{

class ElapsedTimer
{
    using Clock = std::chrono::high_resolution_clock;

public:
    inline void reset()
    {
        m_ref   = Clock::now();
        m_valid = true;
    }

    inline i64 elapsed_s() const
    {
        return elapsed<std::chrono::seconds>();
    }

    inline i64 elapsed_ms() const
    {
        return elapsed<std::chrono::milliseconds>();
    }

    inline i64 elapsed_us() const
    {
        return elapsed<std::chrono::microseconds>();
    }

    inline i64 elapsed_ns() const
    {
        return elapsed<std::chrono::nanoseconds>();
    }

    inline bool is_valid() const
    {
        return m_valid;
    }

private:
    Clock::time_point m_ref   = Clock::now();
    bool              m_valid = false;

    template<typename TimeUnit>
    inline i64 elapsed() const
    {
        return std::chrono::duration_cast<TimeUnit>(Clock::now() - m_ref).count();
    }
};

}  // namespace ac

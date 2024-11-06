#pragma once

#include "num_types.hpp"

namespace ac
{

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template<typename T>
class span
{
public:
    span(T *data, usize size) : m_data(data), m_size(size) {}

    T const *begin() const { return data; }
    T const *end() const { return data + S; }

    usize size() const { return S; }

    T const *data() const { return m_data; }

    T const &operator[](size_t pos) const { return data[pos]; }

private:
    T    *m_data = nullptr;
    usize m_size = 0;
};

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// Compare 2 blocks of contiguous memory
template<typename T>
bool cmp(span<T const> s1, span<T const> s2)
{
    if (s1.size() != s2.size())
    {
        return false;
    }
    T const *it1 = s1.data();
    T const *it2 = s2.data();

    while (it1 != s1.end() && it2 != s2.end())
    {
        if (*it1 != *it2)
        {
            return false;
        }
        it1++;
        it2++;
    }

    return true;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

}  // namespace ac

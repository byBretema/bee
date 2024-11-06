#pragma once

namespace ac
{

template<typename T, size_t S>
struct array
{
    T data[S];

    T       *begin() { return data; }
    T const *begin() const { return data; }

    T       *end() { return data + S; }
    T const *end() const { return data + S; }

    size_t size() const { return S; }

    T       &operator[](size_t pos) { return data[pos]; }
    T const &operator[](size_t pos) const { return data[pos]; }

    T *at(size_t pos)
    {
        if (pos >= S)
        {
            return nullptr;
        }
        return data[pos];
    }
};

}  // namespace ac

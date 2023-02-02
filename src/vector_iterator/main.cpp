#include <algorithm>
#include <numeric>

#include "fmt/core.h"

#include "custom_vector.hpp"

int main()
{
    CustomVector<int> vec(8);

    // "1 2 3 4 5 6 7 8"
    for (std::size_t i = 0; i < vec.size(); ++i)
        fmt::print("{} ", vec[i]);

    fmt::print("\n");

    // "1 2 3 4 5 6 7 8"
    for (auto it = vec.cbegin(); it != vec.cend(); ++it)
        fmt::print("{} ", *it);

    fmt::print("\n");

    // "8 7 6 5 4 3 2 1"
    for (auto it = vec.crbegin(); it != vec.crend(); ++it)
        fmt::print("{} ", *it);

    fmt::print("\n");

    // sort in reverse order
    std::sort(vec.begin(), vec.end(), std::greater{});

    // "8 7 6 5 4 3 2 1"
    for (const auto& n : vec)
        fmt::print("{} ", n);

    fmt::print("\n");
}

#include <algorithm>
#include <numeric>

#include "catch2/catch_test_macros.hpp"
#include "fmt/core.h"
#include "fmt/format.h"

#include "custom_vector.hpp"

std::ostream& operator<<(std::ostream& os, const CustomVector<int>::CustomVectorIterator& iter)
{
    return os << fmt::format("{} ({})", fmt::ptr(std::addressof(iter)), *iter);
}

TEST_CASE("CustomVector")
{
    SECTION("a CustomVector of size 0 contains no elements")
    {
        const CustomVector<int> vec1;
        const CustomVector<int> vec2{};
        const CustomVector<int> vec3{0};
        const CustomVector<int> vec4(0);

        CHECK(vec1.size() == 0);
        CHECK(vec2.size() == 0);
        CHECK(vec3.size() == 0);
        CHECK(vec4.size() == 0);
    }

    SECTION("can construct CustomVector with number of default elements")
    {
        const CustomVector<int> vec{8};

        CHECK(vec.size() == 8);

        for (std::size_t i = 0; i < vec.size(); ++i)
            CHECK(vec[i] == static_cast<int>(i + 1));
    }
}

TEST_CASE("CustomVectorIterator")
{
    CustomVector<int> vec{8};

    SECTION("iterate values")
    {
        SECTION("traditional for loop")
        {
            int sum = 0;

            for (auto it = vec.begin(); it != vec.end(); ++it)
                sum += *it;

            CHECK(sum == 36);
        }

        SECTION("range-based for loop")
        {
            int sum = 0;

            for (const int i : vec)
                sum += i;

            CHECK(sum == 36);
        }
    }

    SECTION("distance between elements")
    {
        auto first = vec.begin();
        auto last = vec.end();

        CHECK(first - first == 0);
        CHECK((first + 1) - first == 1);
        CHECK((first + 2) - first == 2);
        CHECK(first - (first + 1) == -1);
        CHECK(first - (first + 2) == -2);
        CHECK(first - last == -8);
        CHECK(last - first == 8);
    }

    SECTION("bidirectional_iterator requirements")
    {
        auto iter = vec.begin();

        // addressof(--a) == addressof(a)
        {
            auto a = iter;
            CHECK(std::addressof(--a) == std::addressof(a));
        }

        // bool(a-- == b)
        {
            auto a = iter;
            auto b = iter;
            CHECK(a-- == b);
        }

        // after evaluating both a-- and --b, bool(a == b) is still true
        {
            auto a = iter;
            auto b = iter;
            a--;
            --b;
            CHECK(a == b);
        }

        // ++(--a) == b
        {
            auto a = iter;
            auto b = iter;
            CHECK(++(--a) == b);
        }

        // --(++a) == b
        {
            auto a = iter;
            auto b = iter;
            CHECK(--(++a) == b);
        }
    }

    SECTION("random_access_iterator requirements")
    {
        auto iter1 = vec.begin() + 1;
        auto iter2 = vec.end() - 2;

        const auto n = iter2 - iter1;

        // (a += n) is equal to b
        {
            auto a = iter1;
            auto b = iter2;
            CHECK((a += n) == b);
        }

        // std::addressof(a += n) is equal to std::addressof(a)
        {
            auto a = iter1;
            CHECK(std::addressof(a += n) == std::addressof(a));
        }

        // (a + n) is equal to (a += n)
        {
            auto a = iter1;
            auto a2 = iter1;
            CHECK((a + n) == (a2 += n));
        }

        // (a + n) is equal to (n + a)
        {
            auto a = iter1;
            CHECK((a + n) == (n + a));
        }

        // for any two positive integers x and y, if a + (x + y) is valid, then a + (x + y) is equal to (a + x) + y
        {
            auto a = iter1;
            const int x = 3;
            const int y = 7;
            CHECK((a + (x + y)) == ((a + x) + y));
        }

        // a + 0 is equal to a
        {
            auto a = iter1;
            CHECK((a + 0) == a);
        }

        // if (a + (n - 1)) is valid, then --b is equal to (a + (n - 1))
        {
            auto a = iter1;
            auto b = iter2;
            CHECK((a + (n - 1)) == --b);
        }

        // (b += -n) and (b -= n) are both equal to a
        {
            auto a = iter1;
            auto b = iter2;
            CHECK((b += -n) == a);
        }

        {
            auto a = iter1;
            auto b = iter2;
            CHECK((b -= n) == a);
        }

        // std::addressof(b -= n) is equal to std::addressof(b)
        {
            auto b = iter2;
            CHECK(std::addressof(b -= n) == std::addressof(b));
        }

        // (b - n) is equal to (b -= n)
        {
            auto b = iter2;
            auto b2 = iter2;
            CHECK((b - n) == (b2 -= n));
        }

        // if b is dereferenceable, then a[n] is valid and is equal to *b
        {
            auto a = iter1;
            auto b = iter2;
            CHECK(a[n] == *b);
        }

        // bool(a <= b) is true
        {
            auto a = iter1;
            auto b = iter2;
            CHECK(a <= b);
        }
    }

    SECTION("operator++")
    {
        auto it = vec.begin();

        CHECK(*(++it) == 2);
        CHECK(*(it++) == 2);
        CHECK(*(it++) == 3);
    }

    SECTION("operator--")
    {
        auto it = vec.end();

        CHECK(*(--it) == 8);
        CHECK(*(it--) == 8);
        CHECK(*(it--) == 7);
    }

    SECTION("operator+=")
    {
        auto it = vec.begin();
        it += 2;

        CHECK(*it == 3);
    }

    SECTION("operator-=")
    {
        auto it = vec.end();
        it -= 3;

        CHECK(*it == 6);
    }

    SECTION("operator+")
    {
        CHECK(*(vec.begin() + 2) == 3);
        CHECK(*(2 + vec.begin()) == 3);
    }

    SECTION("operator-")
    {
        CHECK(*(vec.end() - 3) == 6);
    }

    SECTION("operator[]")
    {
        auto it = vec.begin();

        CHECK(*it == 1);
        CHECK(*(it + 1) == 2);
        CHECK(*(it + 2) == 3);
        CHECK(it[0] == 1);
        CHECK(it[1] == 2);
        CHECK(it[2] == 3);
    }

    SECTION("operator<")
    {
        CHECK(vec.begin() < vec.end());
        CHECK((vec.begin() + 1) < (vec.begin() + 2));
    }

    SECTION("operator==")
    {
        CHECK(vec.begin() == vec.begin());
        CHECK((vec.begin() + 1) == (vec.begin() + 1));
    }

    SECTION("operator!=")
    {
        CHECK(vec.begin() != vec.end());
        CHECK(vec.begin() != (vec.begin() + 1));
    }

    SECTION("operator<=")
    {
        CHECK(vec.begin() <= vec.begin());
        CHECK(vec.begin() <= vec.end());
        CHECK((vec.begin() + 1) <= (vec.begin() + 2));
        CHECK((vec.begin() + 2) <= (vec.begin() + 2));
    }

    SECTION("operator>")
    {
        CHECK(vec.end() > vec.begin());
        CHECK((vec.begin() + 2) > (vec.begin() + 1));
    }

    SECTION("operator>=")
    {
        CHECK(vec.begin() >= vec.begin());
        CHECK(vec.end() >= vec.begin());
        CHECK((vec.begin() + 2) >= (vec.begin() + 1));
        CHECK((vec.begin() + 2) >= (vec.begin() + 2));
    }

    SECTION("std::next")
    {
        auto it = vec.begin();

        CHECK(*std::next(it) == 2);
        CHECK(*std::next(it, 3) == 4);
    }

    SECTION("std::prev")
    {
        auto it = vec.end();

        CHECK(*std::prev(it) == 8);
        CHECK(*std::prev(it, 3) == 6);
    }

    SECTION("std::advance")
    {
        auto it = vec.begin();

        std::advance(it, 1);
        CHECK(*it == 2);

        std::advance(it, 2);
        CHECK(*it == 4);
    }

    SECTION("std::distance")
    {
        CHECK(std::distance(vec.begin(), vec.end()) == 8);
        CHECK(std::distance(vec.end(), vec.begin()) == -8);
    }

    SECTION("algorithms")
    {
        SECTION("std::accumulate")
        {
            CHECK(std::accumulate(vec.begin(), vec.end(), 0) == 36);
        }

        SECTION("std::find")
        {
            CHECK(std::find(vec.begin(), vec.end(), 5) != vec.end());
            CHECK(std::find(vec.begin(), vec.end(), 99) == vec.end());
        }

        SECTION("std::sort")
        {
            std::sort(vec.begin(), vec.end(), std::greater{});  // sort in reverse order

            CHECK(vec[0] == 8);
            CHECK(vec[1] == 7);
            CHECK(vec[2] == 6);
            CHECK(vec[3] == 5);

            std::sort(vec.begin(), vec.end());  // sort back to normal order

            CHECK(vec[0] == 1);
            CHECK(vec[1] == 2);
            CHECK(vec[2] == 3);
            CHECK(vec[3] == 4);
        }
    }
}

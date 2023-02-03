#include <algorithm>
#include <numeric>

#include "catch2/catch_test_macros.hpp"
#include "fmt/core.h"
#include "fmt/format.h"

#include "custom_vector.hpp"

std::ostream& operator<<(std::ostream& os, const CustomVector<int>::iterator& iter)
{
    return os << fmt::format("{} ({})", fmt::ptr(std::addressof(iter)), *iter);
}

std::ostream& operator<<(std::ostream& os, const CustomVector<int>::reverse_iterator& iter)
{
    return os << fmt::format("{} ({})", fmt::ptr(std::addressof(iter)), *iter);
}

TEST_CASE("CustomVector")
{
    SECTION("a default constructed CustomVector is empty")
    {
        const CustomVector<int> vec1;
        const CustomVector<int> vec2{};

        CHECK(vec1.size() == 0);
        CHECK(vec2.size() == 0);
    }

    SECTION("can construct CustomVector with initializer list")
    {
        const CustomVector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

        CHECK(vec.size() == 8);

        for (std::size_t i = 0; i < vec.size(); ++i)
            CHECK(vec[i] == static_cast<int>(i + 1));
    }

    SECTION("iterators")
    {
        SECTION("const")
        {
            const CustomVector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

            SECTION("begin")
            {
                auto it1 = vec.begin();
                auto it2 = vec.cbegin();
                auto it3 = vec.rbegin();
                auto it4 = vec.crbegin();

                CHECK(it1 == it2);
                CHECK(it3 == it4);
                CHECK(*it1 == *it2);
                CHECK(*it3 == *it4);
            }

            SECTION("end")
            {
                auto it1 = vec.end();
                auto it2 = vec.cend();
                auto it3 = vec.rend();
                auto it4 = vec.crend();

                CHECK(it1 == it2);
                CHECK(it3 == it4);
                CHECK(*(it1 - 1) == *(it2 - 1));
                CHECK(*(it3 - 1) == *(it4 - 1));
            }
        }

        SECTION("non-const")
        {
            CustomVector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

            SECTION("begin")
            {
                auto it1 = vec.begin();
                auto it2 = vec.rbegin();

                *it1 = 100;
                *it2 = 200;

                CHECK(vec[0] == 100);
                CHECK(vec[vec.size() - 1] == 200);
            }

            SECTION("end")
            {
                auto it1 = vec.end();
                auto it2 = vec.rend();

                *(it1 - 1) = 100;
                *(it2 - 1) = 200;

                CHECK(vec[0] == 200);
                CHECK(vec[vec.size() - 1] == 100);
            }
        }
    }
}

TEST_CASE("CustomVector::iterator")
{
    const CustomVector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

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
            const int x = 2;
            const int y = 3;
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

    SECTION("operators")
    {
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

        SECTION("operator<")
        {
            CHECK(vec.begin() < vec.end());
            CHECK((vec.begin() + 1) < (vec.begin() + 2));
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

        SECTION("operator[]")
        {
            SECTION("const")
            {
                auto it = vec.begin();

                CHECK(*it == 1);
                CHECK(*(it + 1) == 2);
                CHECK(*(it + 2) == 3);
                CHECK(it[0] == 1);
                CHECK(it[1] == 2);
                CHECK(it[2] == 3);
            }

            SECTION("non-const")
            {
                CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};

                numbers[0] = 100;
                numbers[numbers.size() - 1] = 200;

                CHECK(*numbers.begin() == 100);
                CHECK(*(numbers.end() - 1) == 200);
            }
        }
    }

    SECTION("iterator operations")
    {
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
            auto it1 = vec.begin();

            std::advance(it1, 1);
            CHECK(*it1 == 2);

            std::advance(it1, 2);
            CHECK(*it1 == 4);

            auto it2 = vec.end();

            std::advance(it2, -1);
            CHECK(*it2 == 8);

            std::advance(it2, -2);
            CHECK(*it2 == 6);
        }

        SECTION("std::distance")
        {
            CHECK(std::distance(vec.begin(), vec.end()) == 8);
            CHECK(std::distance(vec.end(), vec.begin()) == -8);
        }
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
            CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};

            std::sort(numbers.begin(), numbers.end(), std::greater{});  // sort in reverse order

            CHECK(numbers[0] == 8);
            CHECK(numbers[1] == 7);
            CHECK(numbers[2] == 6);

            std::sort(numbers.begin(), numbers.end());  // sort back to normal order

            CHECK(numbers[0] == 1);
            CHECK(numbers[1] == 2);
            CHECK(numbers[2] == 3);
        }
    }
}

TEST_CASE("CustomVector::reverse_iterator")
{
    const CustomVector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

    SECTION("iterate values")
    {
        SECTION("traditional for loop")
        {
            int sum = 0;

            for (auto it = vec.rbegin(); it != vec.rend(); ++it)
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
        auto first = vec.rbegin();
        auto last = vec.rend();

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
        auto iter = vec.rbegin();

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
        auto iter1 = vec.rbegin() + 1;
        auto iter2 = vec.rend() - 2;

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
            const int x = 2;
            const int y = 3;
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

    SECTION("operators")
    {
        SECTION("operator++")
        {
            auto it = vec.rbegin();

            CHECK(*(++it) == 7);
            CHECK(*(it++) == 7);
            CHECK(*(it++) == 6);
        }

        SECTION("operator--")
        {
            auto it = vec.rend();

            CHECK(*(--it) == 1);
            CHECK(*(it--) == 1);
            CHECK(*(it--) == 2);
        }

        SECTION("operator+=")
        {
            auto it = vec.rbegin();
            it += 2;

            CHECK(*it == 6);
        }

        SECTION("operator-=")
        {
            auto it = vec.rend();
            it -= 3;

            CHECK(*it == 3);
        }

        SECTION("operator+")
        {
            CHECK(*(vec.rbegin() + 2) == 6);
            CHECK(*(2 + vec.rbegin()) == 6);
        }

        SECTION("operator-")
        {
            CHECK(*(vec.rend() - 3) == 3);
        }

        SECTION("operator==")
        {
            CHECK(vec.rbegin() == vec.rbegin());
            CHECK((vec.rbegin() + 1) == (vec.rbegin() + 1));
        }

        SECTION("operator!=")
        {
            CHECK(vec.rbegin() != vec.rend());
            CHECK(vec.rbegin() != (vec.rbegin() + 1));
        }

        SECTION("operator<")
        {
            CHECK(vec.rbegin() < vec.rend());
            CHECK((vec.rbegin() + 1) < (vec.rbegin() + 2));
        }

        SECTION("operator<=")
        {
            CHECK(vec.rbegin() <= vec.rbegin());
            CHECK(vec.rbegin() <= vec.rend());
            CHECK((vec.rbegin() + 1) <= (vec.rbegin() + 2));
            CHECK((vec.rbegin() + 2) <= (vec.rbegin() + 2));
        }

        SECTION("operator>")
        {
            CHECK(vec.rend() > vec.rbegin());
            CHECK((vec.rbegin() + 2) > (vec.rbegin() + 1));
        }

        SECTION("operator>=")
        {
            CHECK(vec.rbegin() >= vec.rbegin());
            CHECK(vec.rend() >= vec.rbegin());
            CHECK((vec.rbegin() + 2) >= (vec.rbegin() + 1));
            CHECK((vec.rbegin() + 2) >= (vec.rbegin() + 2));
        }

        SECTION("operator[]")
        {
            SECTION("const")
            {
                auto it = vec.rbegin();

                CHECK(*it == 8);
                CHECK(*(it + 1) == 7);
                CHECK(*(it + 2) == 6);
                CHECK(it[0] == 8);
                CHECK(it[1] == 7);
                CHECK(it[2] == 6);
            }

            SECTION("non-const")
            {
                CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};

                numbers[0] = 100;
                numbers[numbers.size() - 1] = 200;

                CHECK(*numbers.rbegin() == 200);
                CHECK(*(numbers.rend() - 1) == 100);
            }
        }
    }

    SECTION("iterator operations")
    {
        SECTION("std::next")
        {
            auto it = vec.rbegin();

            CHECK(*std::next(it) == 7);
            CHECK(*std::next(it, 3) == 5);
        }

        SECTION("std::prev")
        {
            auto it = vec.rend();

            CHECK(*std::prev(it) == 1);
            CHECK(*std::prev(it, 3) == 3);
        }

        SECTION("std::advance")
        {
            auto it1 = vec.rbegin();

            std::advance(it1, 1);
            CHECK(*it1 == 7);

            std::advance(it1, 2);
            CHECK(*it1 == 5);

            auto it2 = vec.rend();

            std::advance(it2, -1);
            CHECK(*it2 == 1);

            std::advance(it2, -2);
            CHECK(*it2 == 3);
        }

        SECTION("std::distance")
        {
            CHECK(std::distance(vec.rbegin(), vec.rend()) == 8);
            CHECK(std::distance(vec.rend(), vec.rbegin()) == -8);
        }
    }

    SECTION("algorithms")
    {
        SECTION("std::accumulate")
        {
            CHECK(std::accumulate(vec.rbegin(), vec.rend(), 0) == 36);
        }

        SECTION("std::find")
        {
            CHECK(std::find(vec.rbegin(), vec.rend(), 5) != vec.rend());
            CHECK(std::find(vec.rbegin(), vec.rend(), 99) == vec.rend());
        }

        SECTION("std::sort")
        {
            CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};

            std::sort(numbers.rbegin(), numbers.rend());  // sort in reverse order

            CHECK(numbers[0] == 8);
            CHECK(numbers[1] == 7);
            CHECK(numbers[2] == 6);

            std::sort(numbers.rbegin(), numbers.rend(), std::greater{});  // sort back to normal order

            CHECK(numbers[0] == 1);
            CHECK(numbers[1] == 2);
            CHECK(numbers[2] == 3);
        }
    }
}

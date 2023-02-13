#include <algorithm>
#include <numeric>

#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"
#include "fmt/core.h"
#include "fmt/format.h"

#include "custom_vector.hpp"

template <typename Iter>
void check_bidirectional_iterator_requirements(Iter iter)
{
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

template <typename Iter>
void check_random_access_iterator_requirements(Iter iter1, Iter iter2)
{
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

template <typename Iter>
void check_spaceship_operator(Iter begin, Iter end)
{
    SECTION("operator==")
    {
        CHECK(begin == begin);
        CHECK((begin == end) == false);
    }

    SECTION("operator!=")
    {
        CHECK(begin != end);
        CHECK((begin != begin) == false);
    }

    SECTION("operator<")
    {
        CHECK(begin < end);
        CHECK((end < begin) == false);
    }

    SECTION("operator<=")
    {
        CHECK(begin <= begin);
        CHECK(begin <= end);
        CHECK((end <= begin) == false);
    }

    SECTION("operator>")
    {
        CHECK(end > begin);
        CHECK((begin > end) == false);
    }

    SECTION("operator>=")
    {
        CHECK(end >= end);
        CHECK(end >= begin);
        CHECK((begin >= end) == false);
    }
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

    SECTION("can use CustomVector with POD types")
    {
        int i1 = 10;
        int i2 = 20;

        const CustomVector<int> vec1{1, 2, 3, 4};
        const CustomVector<double> vec2{1.0, 2.0, 3.0, 4.0};
        const CustomVector<int*> vec3{&i1, nullptr, &i2};

        CHECK(vec1[1] == 2);
        CHECK(vec1[2] == 3);
        CHECK(vec2[1] == Catch::Approx(2.0));
        CHECK(vec2[2] == Catch::Approx(3.0));
        CHECK(vec3[1] == nullptr);
        CHECK(*vec3[2] == 20);
    }

    SECTION("can use CustomVector with objects")
    {
        struct S {
            int x, y;
        };

        const CustomVector<S> vec{{11, 12}, {21, 22}};

        CHECK(vec[0].x == 11);
        CHECK(vec[1].y == 22);
    }

    SECTION("can use CustomVector with std::string")
    {
        const CustomVector<std::string> vec{"abc", "xyzzy"};

        CHECK(vec.begin()->size() == 3);
        CHECK_THAT(vec[0], Catch::Matchers::Equals("abc"));
        CHECK_THAT(vec[1], Catch::Matchers::Equals("xyzzy"));
    }

    SECTION("operator[]")
    {
        SECTION("const")
        {
            const CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};

            CHECK(numbers[0] == 1);
            CHECK(numbers[numbers.size() - 1] == 8);
        }

        SECTION("non-const")
        {
            CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};

            CHECK(numbers[0] == 1);
            CHECK(numbers[numbers.size() - 1] == 8);

            numbers[0] = 100;
            numbers[numbers.size() - 1] = 200;

            CHECK(*numbers.begin() == 100);
            CHECK(*(numbers.end() - 1) == 200);
        }
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
                CHECK(*it1 == 1);
                CHECK(*it2 == 1);
                CHECK(*it3 == 8);
                CHECK(*it4 == 8);
            }

            SECTION("end")
            {
                auto it1 = vec.end();
                auto it2 = vec.cend();
                auto it3 = vec.rend();
                auto it4 = vec.crend();

                CHECK(it1 == it2);
                CHECK(it3 == it4);
                CHECK(*(it1 - 1) == 8);
                CHECK(*(it2 - 1) == 8);
                CHECK(*(it3 - 1) == 1);
                CHECK(*(it4 - 1) == 1);
            }
        }

        SECTION("non-const")
        {
            CustomVector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};

            SECTION("begin")
            {
                auto it1 = vec.begin();
                auto it2 = vec.rbegin();

                CHECK(*it1 == 1);
                CHECK(*it2 == 8);

                *it1 = 100;
                *it2 = 200;

                CHECK(vec[0] == 100);
                CHECK(vec[vec.size() - 1] == 200);
            }

            SECTION("end")
            {
                auto it1 = vec.end();
                auto it2 = vec.rend();

                CHECK(*(it1 - 1) == 8);
                CHECK(*(it2 - 1) == 1);

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
        check_bidirectional_iterator_requirements(vec.begin());
    }

    SECTION("random_access_iterator requirements")
    {
        check_random_access_iterator_requirements(vec.begin() + 1, vec.end() - 2);
    }

    SECTION("operators")
    {
        SECTION("operator*")
        {
            auto it = vec.begin();

            CHECK(*it == 1);
        }

        SECTION("operator->")
        {
            struct S {
                int x;
            };

            CustomVector<S> v{S{1}, S{2}};
            auto it = v.begin();

            CHECK(it->x == 1);
            CHECK((++it)->x == 2);
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

        SECTION("operator<=>")
        {
            check_spaceship_operator(vec.begin(), vec.end());
        }

        SECTION("operator[]")
        {
            SECTION("const")
            {
                auto it = vec.begin();

                CHECK(it[0] == 1);
                CHECK(it[1] == 2);
                CHECK(it[2] == 3);
            }

            SECTION("non-const")
            {
                CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};
                auto it = numbers.begin();

                CHECK(it[0] == 1);
                CHECK(it[1] == 2);
                CHECK(it[2] == 3);

                it[0] = 100;
                it[1] = 200;
                it[2] = 300;

                CHECK(numbers[0] == 100);
                CHECK(numbers[1] == 200);
                CHECK(numbers[2] == 300);
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
        check_bidirectional_iterator_requirements(vec.rbegin());
    }

    SECTION("random_access_iterator requirements")
    {
        check_random_access_iterator_requirements(vec.rbegin() + 1, vec.rend() - 2);
    }

    SECTION("operators")
    {
        SECTION("operator*")
        {
            auto it = vec.rbegin();

            CHECK(*it == 8);
        }

        SECTION("operator->")
        {
            struct S {
                int x;
            };

            const CustomVector<S> v{S{1}, S{2}};
            auto it = v.rbegin();

            CHECK(it->x == 2);
            CHECK((++it)->x == 1);
        }

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

        SECTION("operator<=>")
        {
            check_spaceship_operator(vec.rbegin(), vec.rend());
        }

        SECTION("operator[]")
        {
            SECTION("const")
            {
                auto it = vec.rbegin();

                CHECK(it[0] == 8);
                CHECK(it[1] == 7);
                CHECK(it[2] == 6);
            }

            SECTION("non-const")
            {
                CustomVector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};
                auto it = numbers.rbegin();

                CHECK(it[0] == 8);
                CHECK(it[1] == 7);
                CHECK(it[2] == 6);

                it[0] = 100;
                it[1] = 200;
                it[2] = 300;

                CHECK(numbers[numbers.size() - 3] == 300);
                CHECK(numbers[numbers.size() - 2] == 200);
                CHECK(numbers[numbers.size() - 1] == 100);
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

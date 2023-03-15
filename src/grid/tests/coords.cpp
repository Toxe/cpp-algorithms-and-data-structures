#include "catch2/catch_test_macros.hpp"

#include "../coords.hpp"

TEST_CASE("Coords")
{
    SECTION("creation")
    {
        CHECK(Coords{}.x == 0);
        CHECK(Coords{}.y == 0);
        CHECK(Coords{2, 3}.x == 2);
        CHECK(Coords{2, 3}.y == 3);
    }

    SECTION("assignment")
    {
        const Coords a{4, 5};

        Coords b = a;

        Coords c;
        c = a;

        CHECK(b == a);
        CHECK(c == a);

        b += Coords{2, 1};
        c -= Coords{1, 2};

        CHECK(b == Coords{6, 6});
        CHECK(c == Coords{3, 3});
    }

    SECTION("arithmetic")
    {
        CHECK(Coords{4, 5} + Coords{2, 1} == Coords{6, 6});
        CHECK(Coords{4, 5} - Coords{1, 2} == Coords{3, 3});
    }

    SECTION("comparison")
    {
        CHECK(Coords{2, 3} == Coords{2, 3});
        CHECK((Coords{2, 3} == Coords{3, 2}) == false);

        CHECK(Coords{2, 3} != Coords{3, 2});
        CHECK((Coords{2, 3} != Coords{2, 3}) == false);

        CHECK(Coords{2, 2} < Coords{3, 3});
        CHECK(Coords{2, 3} < Coords{3, 3});
        CHECK(Coords{3, 2} < Coords{3, 3});
        CHECK((Coords{2, 2} < Coords{2, 2}) == false);
        CHECK((Coords{3, 3} < Coords{2, 2}) == false);
        CHECK((Coords{3, 3} < Coords{2, 3}) == false);
        CHECK((Coords{3, 3} < Coords{3, 2}) == false);

        CHECK(Coords{3, 3} > Coords{2, 2});
        CHECK(Coords{3, 3} > Coords{2, 3});
        CHECK(Coords{3, 3} > Coords{3, 2});
        CHECK((Coords{3, 3} > Coords{3, 3}) == false);
        CHECK((Coords{2, 2} > Coords{3, 3}) == false);
        CHECK((Coords{2, 3} > Coords{3, 3}) == false);
        CHECK((Coords{3, 2} > Coords{3, 3}) == false);

        CHECK(Coords{2, 2} <= Coords{2, 2});
        CHECK(Coords{2, 2} <= Coords{3, 3});
        CHECK(Coords{2, 3} <= Coords{3, 3});
        CHECK(Coords{3, 2} <= Coords{3, 3});
        CHECK((Coords{3, 3} <= Coords{2, 2}) == false);
        CHECK((Coords{3, 3} <= Coords{2, 3}) == false);
        CHECK((Coords{3, 3} <= Coords{3, 2}) == false);

        CHECK(Coords{3, 3} >= Coords{3, 3});
        CHECK(Coords{3, 3} >= Coords{2, 2});
        CHECK(Coords{3, 3} >= Coords{2, 3});
        CHECK(Coords{3, 3} >= Coords{3, 2});
        CHECK((Coords{2, 2} >= Coords{3, 3}) == false);
        CHECK((Coords{2, 3} >= Coords{3, 3}) == false);
        CHECK((Coords{3, 2} >= Coords{3, 3}) == false);
    }

    SECTION("col() and row() return x and y coordinates")
    {
        CHECK(Coords{}.col() == 0);
        CHECK(Coords{}.row() == 0);
        CHECK(Coords{2, 3}.col() == 2);
        CHECK(Coords{2, 3}.row() == 3);
    }

    SECTION("move()")
    {
        Coords a{1, 2};
        Coords b{a};

        a.move(2, 1);
        b.move(Coords{2, 1});

        CHECK(a == Coords{3, 3});
        CHECK(b == Coords{3, 3});

        a.move(-6, -9);
        b.move(Coords{-6, -9});

        CHECK(a == Coords{-3, -6});
        CHECK(b == Coords{-3, -6});
    }

    SECTION("move horizontally and vertically")
    {
        Coords a{1, 2};

        a.move_horizontally(3);
        CHECK(a == Coords{4, 2});

        a.move_horizontally(-1);
        CHECK(a == Coords{3, 2});

        a.move_vertically(-1);
        CHECK(a == Coords{3, 1});

        a.move_vertically(3);
        CHECK(a == Coords{3, 4});
    }

    SECTION("move in relative directions")
    {
        Coords a;
        Coords b;

        a.move_up(3);
        b.move_up(-3);
        CHECK(a == Coords{0, -3});
        CHECK(b == Coords{0, 3});

        a.move_down(5);
        b.move_down(-5);
        CHECK(a == Coords{0, 2});
        CHECK(b == Coords{0, -2});

        a.move_right(5);
        b.move_right(-5);
        CHECK(a == Coords{5, 2});
        CHECK(b == Coords{-5, -2});

        a.move_left(8);
        b.move_left(-8);
        CHECK(a == Coords{-3, 2});
        CHECK(b == Coords{3, -2});
    }

    SECTION("move in cardinal directions")
    {
        Coords a;
        Coords b;

        a.move_north(3);
        b.move_north(-3);
        CHECK(a == Coords{0, -3});
        CHECK(b == Coords{0, 3});

        a.move_south(5);
        b.move_south(-5);
        CHECK(a == Coords{0, 2});
        CHECK(b == Coords{0, -2});

        a.move_east(5);
        b.move_east(-5);
        CHECK(a == Coords{5, 2});
        CHECK(b == Coords{-5, -2});

        a.move_west(8);
        b.move_west(-8);
        CHECK(a == Coords{-3, 2});
        CHECK(b == Coords{3, -2});
    }
}

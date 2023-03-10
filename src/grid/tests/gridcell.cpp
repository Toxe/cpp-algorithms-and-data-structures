#include "catch2/catch_test_macros.hpp"

#include "../grid.hpp"

Grid<int> create_grid_with_test_values(int cols, int rows);

TEST_CASE("GridCell")
{
    SECTION("can get coordinates")
    {
        const Grid<int> grid = create_grid_with_test_values(4, 5);
        auto cell1 = grid.cell(1, 2);
        auto cell2 = grid.cell(Coords{2, 3});

        CHECK(cell1.x == 1);
        CHECK(cell1.y == 2);
        CHECK(cell1.col() == 1);
        CHECK(cell1.row() == 2);
        CHECK(cell1 == Coords{1, 2});

        CHECK(cell2.x == 2);
        CHECK(cell2.y == 3);
        CHECK(cell2.col() == 2);
        CHECK(cell2.row() == 3);
        CHECK(cell2 == Coords{2, 3});
    }

    SECTION("can access and change values")
    {
        SECTION("const")
        {
            const Grid<int> grid = create_grid_with_test_values(4, 5);
            auto cell = grid.cell(2, 1);

            CHECK(cell.value() == 23);
        }

        SECTION("non-const")
        {
            Grid<int> grid = create_grid_with_test_values(4, 5);

            auto cell1 = grid.cell(2, 1);
            auto cell2 = grid.cell(Coords{3, 2});

            cell1.value() = 100;
            cell2.value() = 200;

            CHECK(grid.at(2, 1) == 100);
            CHECK(grid.at(3, 2) == 200);
        }
    }

    SECTION("can move cell")
    {
        SECTION("const")
        {
            const Grid<int> grid = create_grid_with_test_values(4, 5);
            auto cell = grid.cell(1, 2);

            cell.move_up(2);
            CHECK(grid.at(1, 0) == 12);

            cell.move_horizontally(2);
            CHECK(grid.at(3, 0) == 14);

            cell.move(-2, 3);
            CHECK(grid.at(1, 3) == 42);
        }

        SECTION("non-const")
        {
            Grid<int> grid = create_grid_with_test_values(4, 5);
            auto cell = grid.cell(1, 2);

            cell.move_up(2);
            cell.value() = 111;
            CHECK(grid.at(1, 0) == 111);

            cell.move_horizontally(2);
            cell.value() = 222;
            CHECK(grid.at(3, 0) == 222);

            cell.move(-2, 3);
            cell.value() = 333;
            CHECK(grid.at(1, 3) == 333);
        }
    }
}

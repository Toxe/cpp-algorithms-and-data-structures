#include "catch2/catch_test_macros.hpp"

#include "../grid.hpp"

Grid<int> create_grid_with_test_values(int rows, int cols);

TEST_CASE("GridCell")
{
    SECTION("can get coordinates")
    {
        Grid<int> grid = create_grid_with_test_values(5, 4);
        auto cell1 = grid.cell(2, 1);
        auto cell2 = grid.cell(Coords{2, 3});

        CHECK(cell1.x == 1);
        CHECK(cell1.y == 2);
        CHECK(cell1.col() == 1);
        CHECK(cell1.row() == 2);
        CHECK(cell1.coords() == Coords{1, 2});

        CHECK(cell2.x == 2);
        CHECK(cell2.y == 3);
        CHECK(cell2.col() == 2);
        CHECK(cell2.row() == 3);
        CHECK(cell2.coords() == Coords{2, 3});
    }

    SECTION("can access values")
    {
        Grid<int> grid = create_grid_with_test_values(5, 4);

        auto cell1 = grid.cell(1, 2);
        auto cell2 = grid.cell(Coords{3, 2});

        cell1.value() = 100;
        cell2.value() = 200;

        CHECK(grid.at(1, 2) == 100);
        CHECK(grid.at(2, 3) == 200);
    }

    SECTION("can move cell")
    {
        Grid<int> grid = create_grid_with_test_values(5, 4);
        auto cell = grid.cell(2, 1);

        cell.move_up(2);
        cell.value() = 111;
        CHECK(grid.at(0, 1) == 111);

        cell.move_horizontally(2);
        cell.value() = 222;
        CHECK(grid.at(0, 3) == 222);

        cell.move(-2, 3);
        cell.value() = 333;
        CHECK(grid.at(3, 1) == 333);
    }
}

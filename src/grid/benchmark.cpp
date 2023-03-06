#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "fmt/core.h"

#include "grid.hpp"

Grid<int> create_grid_with_numbered_values(const int rows, const int cols)
{
    Grid<int> grid{rows, cols};

    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < cols; ++col)
            grid[row][col] = (row + 1) * 10 + col + 1;

    return grid;
}

void benchmark_create_grid()
{
    for (auto size : {4, 16, 256, 1024}) {
        ankerl::nanobench::Bench().run(fmt::format("create grid: {}x{}", size, size), [&] {
            const Grid<int> grid = create_grid_with_numbered_values(size, size);
            ankerl::nanobench::doNotOptimizeAway(grid);
        });
    }
}

void benchmark_sum_vector()
{
    for (auto size : {4, 16, 256, 1024}) {
        const Grid<int> grid = create_grid_with_numbered_values(size, size);

        ankerl::nanobench::Bench().run(fmt::format("sum vector: {}x{}", size, size), [&] {
            int sum = 0;

            for (const auto& i : grid)
                sum += i;

            ankerl::nanobench::doNotOptimizeAway(sum);
        });
    }
}

void benchmark_sum_rows()
{
    for (auto size : {4, 16, 256, 1024}) {
        const Grid<int> grid = create_grid_with_numbered_values(size, size);

        ankerl::nanobench::Bench().run(fmt::format("sum rows: {}x{}", size, size), [&] {
            int sum = 0;

            for (auto& row : grid.rows())
                for (auto i : row)
                    sum += i;

            ankerl::nanobench::doNotOptimizeAway(sum);
        });
    }
}

void benchmark_sum_cols()
{
    for (auto size : {4, 16, 256, 1024}) {
        const Grid<int> grid = create_grid_with_numbered_values(size, size);

        ankerl::nanobench::Bench().run(fmt::format("sum columns: {}x{}", size, size), [&] {
            int sum = 0;

            for (auto& col : grid.cols())
                for (auto i : col)
                    sum += i;

            ankerl::nanobench::doNotOptimizeAway(sum);
        });
    }
}

void benchmark_sum_cell_rows()
{
    for (auto size : {4, 16, 256, 1024}) {
        const Grid<int> grid = create_grid_with_numbered_values(size, size);

        ankerl::nanobench::Bench().run(fmt::format("sum cell rows: {}x{}", size, size), [&] {
            int sum = 0;

            for (int row = 0; row < grid.height(); ++row) {
                auto cell = grid.cell(row, 0);

                while (cell.col() < grid.width()) {
                    sum += cell.value();
                    cell.move_right();
                }
            }

            ankerl::nanobench::doNotOptimizeAway(sum);
        });
    }
}

void benchmark_sum_cell_cols()
{
    for (auto size : {4, 16, 256, 1024}) {
        const Grid<int> grid = create_grid_with_numbered_values(size, size);

        ankerl::nanobench::Bench().run(fmt::format("sum cell cols: {}x{}", size, size), [&] {
            int sum = 0;

            for (int col = 0; col < grid.width(); ++col) {
                auto cell = grid.cell(0, col);

                while (cell.row() < grid.height()) {
                    sum += cell.value();
                    cell.move_down();
                }
            }

            ankerl::nanobench::doNotOptimizeAway(sum);
        });
    }
}

int main()
{
    benchmark_create_grid();
    benchmark_sum_vector();
    benchmark_sum_rows();
    benchmark_sum_cols();
    benchmark_sum_cell_rows();
    benchmark_sum_cell_cols();
}

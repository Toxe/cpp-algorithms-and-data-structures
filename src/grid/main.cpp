#include <algorithm>
#include <numeric>

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

int main()
{
    const Grid<int> grid = create_grid_with_numbered_values(8, 8);

    for (auto& row : grid.rows())
        fmt::print("row sum: {}\n", std::accumulate(row.begin(), row.end(), 0));

    for (auto& col : grid.cols())
        fmt::print("column sum: {}\n", std::accumulate(col.begin(), col.end(), 0));

    int sum = 0;

    for (auto& row : grid.rows())
        for (auto value : row)
            sum += value;

    fmt::print("total sum: {}\n", sum);
}

#pragma once

#include "coords.hpp"

template <typename T>
class Grid;

template <typename T>
class GridCell : public Coords {
public:
    GridCell(Grid<T>* grid, const Coords& coords) : Coords(coords), grid_{grid} { }

    [[nodiscard]] int col() const { return x; }
    [[nodiscard]] int row() const { return y; }

    [[nodiscard]] Coords coords() const { return *this; }

    [[nodiscard]] T& value() { return grid_->at(coords()); }

private:
    Grid<T>* grid_;
};

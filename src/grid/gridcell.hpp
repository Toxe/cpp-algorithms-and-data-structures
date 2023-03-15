#pragma once

#include <cassert>

#include "coords.hpp"

template <typename GridPointer>
class GridCell : public Coords {
public:
    GridCell(GridPointer grid, const Coords& coords) : Coords(coords)
    {
        assert(grid != nullptr);
        grid_ = grid;
    }

    [[nodiscard]] auto& value() { return grid_->at(*this); }

private:
    GridPointer grid_;
};

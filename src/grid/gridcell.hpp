#pragma once

#include <cassert>

#include "coords.hpp"

template <typename GridPointer, typename CoordsType>
class GridCell : public CoordsType {
public:
    GridCell(GridPointer grid, const CoordsType& coords) : CoordsType(coords)
    {
        assert(grid != nullptr);
        grid_ = grid;
    }

    [[nodiscard]] auto& value() { return grid_->at(*this); }

private:
    GridPointer grid_;
};

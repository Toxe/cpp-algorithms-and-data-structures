#pragma once

#include <compare>

struct Coords {
    int x = 0;
    int y = 0;

    void move(const int dx, const int dy)
    {
        x += dx;
        y += dy;
    }

    void move(const Coords& delta)
    {
        x += delta.x;
        y += delta.y;
    }

    void move_horizontally(const int distance) { move(distance, 0); }
    void move_vertically(const int distance) { move(0, distance); }

    void move_up(const int distance) { move(0, -distance); }
    void move_down(const int distance) { move(0, distance); }
    void move_left(const int distance) { move(-distance, 0); }
    void move_right(const int distance) { move(distance, 0); }

    void move_north(const int distance) { move(0, -distance); }
    void move_south(const int distance) { move(0, distance); }
    void move_west(const int distance) { move(-distance, 0); }
    void move_east(const int distance) { move(distance, 0); }

    Coords operator+(const Coords& other) const { return Coords{x + other.x, y + other.y}; }
    Coords operator-(const Coords& other) const { return Coords{x - other.x, y - other.y}; }

    Coords& operator+=(const Coords& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Coords& operator-=(const Coords& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    auto operator<=>(const Coords& rhs) const = default;
};

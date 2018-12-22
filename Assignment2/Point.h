#pragma once

#include <cstdint>
#include <iosfwd>

namespace arkanoid
{

struct Point
{
    int32_t x, y;
    Point(int32_t x, int32_t y): x(x), y(y) {}
};

struct Velocity
{
    int32_t vx, vy;
    Velocity(int32_t vx, int32_t vy): vx(vx), vy(vy) {}
};

std::ostream& operator<<(std::ostream&, Point);
std::ostream& operator<<(std::ostream&, Velocity);

Point updatePoint(Point, Velocity);

}

#include "Point.h"

#include <ostream>

namespace arkanoid
{

Point updatePoint(Point p, Velocity v) {
	return { p.x + v.vx, p.y + v.vy };
}

std::ostream& operator<<(std::ostream& os, Point point) {
	os << "(" << point.x << ", " << point.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, Velocity v) {
	os << "(" << v.vx << ", " << v.vy << ")";
	return os;
}

}
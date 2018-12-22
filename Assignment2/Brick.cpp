#include "Brick.h"

#include <ostream>

namespace arkanoid
{

std::ostream& operator<<(std::ostream& os, const Brick& brick) {
	os << "Brick { " << brick.getUpperLeft() << ", " << brick.getLowerRight() << " }";
	return os;
}

}
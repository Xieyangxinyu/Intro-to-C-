#include "Paddle.h"

#include <ostream>

namespace arkanoid
{

std::ostream& operator<<(std::ostream& os, const Paddle& paddle) {
	os << "Paddle { " << paddle.getUpperLeft() << ", " << paddle.getLowerRight() << ", velocity = " << paddle.getVelocity() << " }"; 
	return os;
}

}

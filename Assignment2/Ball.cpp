#include "Ball.h"

#include <ostream>

namespace arkanoid
{

std::ostream& operator<<(std::ostream& os, const Ball& ball) {
	os << "Ball { center = " << ball.getCenter() << ", radius = " << ball.getRadius() << ", velocity = " << ball.getVelocity() << " }";
	return os;
}

}
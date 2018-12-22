#include "World.h"

#include <ostream>

namespace arkanoid
{

std::ostream& operator<<(std::ostream& os, const std::vector<Brick>& bricks) {
	os << "[ ";
	for (auto const& b: bricks)
		os << b << "; ";
	os << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, const World& world) {
	os << "World {\n  width = " << world.getWidth() << "\n  height = " << world.getHeight() << "\n  " << world.getBall() << ",\n  " << world.getPaddle() << ",\n  " << world.getBricks() << "\n}";

	return os;
}

}
#pragma once

#include "Point.h"

#include <iosfwd>

namespace arkanoid
{

class Brick
{
    
private:
    Point p;
    int32_t w, h;
    
public:
	// Implement the member functions listed below

    Brick(Point p, int32_t w, int32_t h): p(p), w(w), h(h){}

    Point getUpperLeft() const{
        return p;
    };
    Point getLowerRight() const{
        return *new Point(p.x + w, p.y + h);
    };
};

// Additional logic to let std::cout work with Brick objects
std::ostream& operator<<(std::ostream&, const Brick&);

}

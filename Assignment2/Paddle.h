#pragma once

#include "Input.h"
#include "Point.h"

#include <iosfwd>
#include <algorithm>

namespace arkanoid
{

class Paddle
{
private:
    Point p;
    int32_t w, h, v;
    
public:
	static constexpr int32_t DefaultVelocity = 10;

	// Implement the member functions listed below

    Paddle(Point p, int32_t w, int32_t h, int32_t v): p(p), w(w), h(h), v(v){}

    Point getUpperLeft() const{
        return p;
    };
    
    Point getLowerRight() const{
        return Point(p.x + w, p.y + h);
    };
    
    void setLeft(int x){
        p.x = x;
    }
    void setRight(int x){
        p.x = x - w;
    }
    
    int32_t getVelocity() const{
        return v;
    };
    void setVelocity(int dir){
        v = abs(v) * dir;
    }
};

// Additional logic to let std::cout work with Paddle objects
std::ostream& operator<<(std::ostream&, const Paddle&);

}

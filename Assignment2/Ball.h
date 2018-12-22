#pragma once

#include "Paddle.h"

#include <iosfwd>

namespace arkanoid
{

class Ball
{

private:
    Point c;
    int32_t r;
    Velocity v;
    
public:
	static constexpr int32_t DefaultHorizontalVelocity = 8;
	static constexpr int32_t DefaultVerticalVelocity = 8;
    
	// Implement the member functions listed below
    Ball(Point c, int32_t r, Velocity v): c(c), r(r), v(v){}

    Velocity getVelocity() const{
        return v;
    };
    
    void setHorizontalVelocity(int dir){
        v.vx = dir * DefaultHorizontalVelocity;
    }
    
    void setVerticalVelocity(int dir){
        v.vy = dir * DefaultVerticalVelocity;
    }
    
    Point getCenter() const{
        return c;
    };
    void setCenter(){
        c.x += v.vx; c.y += v.vy;
    }
    
    int32_t getRadius() const{
        return r;
    };
};

// Additional logic to let std::cout work with Ball objects
std::ostream& operator<<(std::ostream&, const Ball&);

}

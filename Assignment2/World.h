#pragma once

#include "Ball.h"
#include "Brick.h"
#include "Input.h"
#include "Paddle.h"

#include <iosfwd>
#include <vector>
#include <algorithm>
using namespace std;
#define P pair<Point,Point>

namespace arkanoid
{

    
class World
{
    
private:
    int32_t w, h;
    Ball b;
    Paddle p;
    vector<Brick> bs;
    // The type of the exception that needs to be thrown when the game is over
    class GameOverException: std::exception {};
    
    bool withinBound(int32_t x, int32_t y) const{
        return x >= 0 && y >= 0 && x <= w && y <= h;
    }
    
    bool withinBound(Point temp) const{
        return temp.x >= 0 && temp.y >= 0 && temp.x <= w && temp.y <= h;
    }
    
    bool AlixAlignedBoundingBox(P box1, P box2) const{
        return box1.first.x < box2.second.x
        && box1.second.x > box2.first.x
        && box1.first.y < box2.second.y
        && box1.second.y > box2.first.y;
    }
    
public:
	// Implement the member functions listed below

    World(int32_t w, int32_t h, Ball b, Paddle p, std::vector<Brick> bs): w(w), h(h), b(b), p(p), bs(bs) {}

    int32_t getWidth() const{
        return w;
    };
    int32_t getHeight() const{
        return h;
    };
    Ball getBall() const{
        return b;
    };
    Paddle getPaddle() const{
        return p;
    };
    std::vector<Brick> getBricks() const{
        return bs;
    };
    
    bool isLegal(){
        
        //There should be at least one brick in the world.
        if(!bs.size())
            return false;
        
        //The ball must have a nonzero velocity.
        Velocity v = b.getVelocity();
        if(!v.vx && !v.vy)
            return false;
        
        //All entities (ball, paddle, and bricks) must locate completely inside the boundaries of the world.
        vector<P> box;
        Point check = b.getCenter();
        int r = b.getRadius();
        int dirx[4] = {0, 1, 0, -1}, diry[4] = {1, 0, -1, 0};
        for(int i = 0; i < 4; i ++)
            if(!withinBound(check.x + dirx[i] * r, check.y + diry[i] * r))
                return false;
        box.push_back(P({check.x - r, check.y - r}, {check.x + r, check.y + r}));
        if(!withinBound(p.getUpperLeft()) || !withinBound(p.getLowerRight()))
            return false;
        else box.push_back(P(p.getUpperLeft(), p.getLowerRight()));
        for(int i = 0; i < bs.size(); i ++)
            if(!withinBound(bs[i].getUpperLeft()) || !withinBound(bs[i].getLowerRight()))
                return false;
            else box.push_back(P(bs[i].getUpperLeft(), bs[i].getLowerRight()));
        
        //Entities must not collide with one another
        for(int i = 0; i < box.size(); i ++)
            for(int j = i + 1; j < box.size(); j ++)
                if(AlixAlignedBoundingBox(box[i], box[j]))return false;
        
        return true;
    };
    
    
    void update(Input input){
        //the paddle moves
        int pv = p.getVelocity();
        if(pv > 0)
            p.setRight(min(w, p.getLowerRight().x + pv));
        else
            p.setLeft(max(0, p.getUpperLeft().x + pv));
        
        //the paddle’s velocity changes
        switch (input) {
            case Input::None:
                p.setVelocity(0);
            case Input::Left:
                p.setVelocity(-1);
            case Input::Right:
                p.setVelocity(1);
                break;
            default:
                break;
        }
        
        //the ball moves
        b.setCenter();
        int r = b.getRadius();
        Point check = b.getCenter();
        P ballInfo = P({check.x - r, check.y - r}, {check.x + r, check.y + r});
        
        //the ball’s velocity changes
        if(check.x - r <= 0) b.setHorizontalVelocity(1);
        if(check.x + r >= w)b.setHorizontalVelocity(-1);
        if(check.y - r <= 0) b.setVerticalVelocity(1);
        if(AlixAlignedBoundingBox(ballInfo, P(p.getUpperLeft(), p.getLowerRight()))) b.setVerticalVelocity(-1);
        if(check.y - r >= h) throw GameOverException();
        
        //remove all bricks that collide with the ball
        for(auto itr = bs.begin(); itr != bs.end(); itr ++)
            if(AlixAlignedBoundingBox(ballInfo, P(itr->getUpperLeft(), itr->getLowerRight())))
                bs.erase(itr);
    };
};

// Additional logic to let std::cout work with World objects
std::ostream& operator<<(std::ostream&, const std::vector<Brick>&);
std::ostream& operator<<(std::ostream&, const World&);

}

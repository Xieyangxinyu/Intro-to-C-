#include "World.h"

#include <iostream>

using namespace arkanoid;

int main() {
    // Uncomment the following lines after you finish the coding task mentioned in assignment handout section 2.2
    Ball ball = {{20,20}, 5, {-1,1}};
    Paddle paddle = {{20, 20}, 20, 5, 1};
    std::vector<Brick> bricks = { Brick({30, 30}, 5, 5), Brick({40, 40}, 6, 6) };
    World world = { 100, 100, ball, paddle, std::move(bricks)};
    std::cout << world << std::endl;
    
    // Uncomment the following lines after you finish World::isLegal()
    std::cout << "isLegal = " << world.isLegal() << std::endl;
    
    // Uncomment the following lines after you finish World::update()
    world.update(Input::Left);
    std::cout << world << std::endl;
}


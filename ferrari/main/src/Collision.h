#ifndef COLLISION_H
#define COLLISION_H

#include "Board.h"
#include "Player.h"

class Collision {
public:
    static bool checkCollision(const Board& maze, int newX, int newY);
};

#endif // COLLISION_H
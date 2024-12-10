#include "Collision.h"

bool Collision::checkCollision(const Board& maze, int newX, int newY) {
    if (newX >= 0 && newY >= 0 && newY < maze.getHeight() && newX < maze.getWidth()) {
        if (maze.getCell(newX, newY) == 1) {
            std::cout << "Collision detected at (" << newX << ", " << newY << ")" << std::endl;
            return true;
        }
        return false;
    }
    std::cout << "Out of bounds: (" << newX << ", " << newY << ")" << std::endl;
    return true;
}
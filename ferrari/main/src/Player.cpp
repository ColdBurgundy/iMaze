#include "Player.h"
#include <iostream>
#include "Maze.h"

Player::Player(int startX, int startY) : x(startX), y(startY) {}

void Player::move(int newX, int newY) {
    x = newX;
    y = newY;
}

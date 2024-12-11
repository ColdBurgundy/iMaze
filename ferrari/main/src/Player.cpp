#include "Player.h"

Player::Player(float startX, float startY, float startZ)
    : x(startX), y(startY), z(startZ), directionX(0.0f), directionY(-1.0f) {}


void Player::move(unsigned char key, float forwardX, float forwardY, float rightX, float rightY, float step) {
    if (key == 'w') {
        x += forwardX * step;
        y += forwardY * step;
    }
    else if (key == 's') {
        x -= forwardX * step;
        y -= forwardY * step;
    }
    else if (key == 'a') {
        x += rightX * step;
        y += rightY * step;
    }
    else if (key == 'd') {
        x -= rightX * step;
        y -= rightY * step;
    }
}

void Player::setDirection(float dirX, float dirY) {
    directionX = dirX;
    directionY = dirY;
}

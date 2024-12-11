#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    float x, y, z;       // 위치 좌표
    float directionX;    // 방향 벡터 (X축)
    float directionY;    // 방향 벡터 (Y축)

public:
    Player(float startX, float startY, float startZ);

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; } // z 좌표 반환 함수

    void setPosition(float newX, float newY, float newZ) {
        x = newX;
        y = newY;
        z = newZ;
    }

    void move(unsigned char key, float forwardX, float forwardY, float rightX, float rightY, float step);
    void setDirection(float dirX, float dirY);

    float getDirectionX() const { return directionX; }
    float getDirectionY() const { return directionY; }
};


#endif // PLAYER_H

#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    float x, y, z;       // ��ġ ��ǥ
    float directionX;    // ���� ���� (X��)
    float directionY;    // ���� ���� (Y��)

public:
    Player(float startX, float startY, float startZ);

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; } // z ��ǥ ��ȯ �Լ�

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

#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    int x, y;
public:
    Player(int startX, int startY);
    void move(int newX, int newY);
    int getX() const { return x; }
    int getY() const { return y; }
};

#endif // PLAYER_H
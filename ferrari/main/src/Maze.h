#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <random>
#include <iostream>

class Maze {
private:
    std::vector<std::vector<int>> maze;
    int width, height;
public:
    Maze(int width, int height);
    void generateMaze();
    void printMaze() const;
    int getCell(int x, int y) const;
    void setCell(int x, int y, int value);
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // MAZE_H
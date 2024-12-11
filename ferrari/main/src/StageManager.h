#pragma once
#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

#include <string>
#include <vector>
#include "Board.h"
#include "ObjLoader.h"
#include "TextureManager.h"

class Stage {
public:
    Stage(int mazeSize,
        const std::string& floorTexPath, const std::string& wallTexPath,
        const std::string& objPath1, const std::string& objPath2);

    ~Stage();

    Board* getBoard() const;
    GLuint getFloorTexture() const;
    GLuint getWallTexture() const;
    ObjLoader* getObjLoader1() const;
    ObjLoader* getObjLoader2() const;

    void calculateObjPositions(); // OBJ 모델의 위치 계산


private:
    Board* board; // Board 객체로 미로 관리
    GLuint floorTexture;
    GLuint wallTexture;
    ObjLoader* objLoader1;
    ObjLoader* objLoader2;

    std::pair<int, int> objPosition1;
    std::pair<int, int> objPosition2;
};

class StageManager {
public:
    StageManager();
    ~StageManager();

    void addStage(Stage* stage);
    Stage* getCurrentStage() const;
    void nextStage();
    bool hasNextStage() const;

private:
    std::vector<Stage*> stages;
    int currentStageIndex;
};

#endif // STAGEMANAGER_H

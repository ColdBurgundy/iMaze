#include "StageManager.h"
#include "TextureManager.h"

Stage::Stage(int mazeSize,
    const std::string& floorTexPath, const std::string& wallTexPath,
    const std::string& objPath1, const std::string& objPath2) {
    // Board 생성
    board = new Board(mazeSize, mazeSize);

    // TextureManager를 통해 텍스처 로드
    TextureManager textureManager;
    floorTexture = textureManager.loadTexture(floorTexPath);
    if (floorTexture == 0) {
        std::cerr << "Failed to load floor texture: " << floorTexPath << std::endl;
        exit(1);
    }
    wallTexture = textureManager.loadTexture(wallTexPath);
    if (wallTexture == 0) {
        std::cerr << "Failed to load wall texture: " << wallTexPath << std::endl;
        exit(1);
    }

    // ObjLoader 초기화
    objLoader1 = new ObjLoader();
    if (!objLoader1->loadOBJ(objPath1)) {
        std::cout << "Failed to load OBJ file: " << objPath1 << std::endl;
        std::cout << "Failed to load OBJ file: " << objPath1 << std::endl;
        exit(1);
    }

    objLoader2 = new ObjLoader();
    if (!objLoader2->loadOBJ(objPath2)) {
        std::cout << "Failed to load OBJ file: " << objPath2 << std::endl;
        std::cout << "Failed to load OBJ file: " << objPath2 << std::endl;

        exit(1);
    }
    calculateObjPositions();
}

void Stage::calculateObjPositions() {
    // 탈출 경로 가져오기
    board->findPathWrapper(0, 0);
    int** autoPath = board->getAutoPath(); // 접근 메서드를 통해 autoPath 가져오기

    std::vector<std::pair<int, int>> escapePath;

    // 탈출 경로를 벡터로 저장
    for (int i = 0; i < board->getHeight(); ++i) {
        for (int j = 0; j < board->getWidth(); ++j) {
            if (board->autoPath[i][j] != -1 && !board->_wall[i][j]) { // 경로의 일부
                escapePath.emplace_back(i, j);
                std::cout << "routePoint: " << i << ", " << j << std::endl;
            }
        }
    }

    // 경로 길이 확인
    int pathLength = escapePath.size();
    if (pathLength < 3) {
        std::cerr << "Error: Path is too short to place objects." << std::endl;
        return;
    }

    // 경로의 1/3 및 2/3 위치 계산
    int index1 = pathLength / 3;
    int index2 = (2 * pathLength) / 3;

    // 경로 상의 좌표 가져오기
    objPosition1 = escapePath[index1];
    objPosition2 = escapePath[index2];

    // OBJ 모델 위치 설정
    objLoader1->setMazePosition(objPosition1.first + 0.5f, -objPosition1.second - 0.5f);
    objLoader2->setMazePosition(objPosition2.first + 0.5f, -objPosition2.second - 0.5f);

    std::cout << "Object 1 Position: (" << objPosition1.first + 0.5f << ", " << -objPosition1.second-0.5f << ")" << std::endl;
    std::cout << "Object 2 Position: (" << objPosition2.first + 0.5f << ", " << -objPosition2.second-0.5f << ")" << std::endl;
}



Stage::~Stage() {
    delete board;
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &wallTexture);
    delete objLoader1;
    delete objLoader2;
}

Board* Stage::getBoard() const {
    return board;
}

GLuint Stage::getFloorTexture() const {
    return floorTexture;
}

GLuint Stage::getWallTexture() const {
    return wallTexture;
}

ObjLoader* Stage::getObjLoader1() const {
    return objLoader1;
}

ObjLoader* Stage::getObjLoader2() const {
    return objLoader2;
}

StageManager::StageManager() : currentStageIndex(0) {}

StageManager::~StageManager() {
    for (Stage* stage : stages) {
        delete stage;
    }
}

void StageManager::addStage(Stage* stage) {
    stages.push_back(stage);
}

Stage* StageManager::getCurrentStage() const {
    if (currentStageIndex < stages.size()) {
        return stages[currentStageIndex];
    }
    return nullptr;
}

void StageManager::nextStage() {
    if (hasNextStage()) {
        currentStageIndex++;
    }
}

bool StageManager::hasNextStage() const {
    return currentStageIndex + 1 < stages.size();
}

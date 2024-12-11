#include "StageManager.h"
#include "TextureManager.h"

Stage::Stage(int mazeSize,
    const std::string& floorTexPath, const std::string& wallTexPath,
    const std::string& objPath1, const std::string& objPath2) {
    // Board ����
    board = new Board(mazeSize, mazeSize);

    // TextureManager�� ���� �ؽ�ó �ε�
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

    // ObjLoader �ʱ�ȭ
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
    // Ż�� ��� ��������
    board->findPathWrapper(0, 0);
    int** autoPath = board->getAutoPath(); // ���� �޼��带 ���� autoPath ��������

    std::vector<std::pair<int, int>> escapePath;

    // Ż�� ��θ� ���ͷ� ����
    for (int i = 0; i < board->getHeight(); ++i) {
        for (int j = 0; j < board->getWidth(); ++j) {
            if (board->autoPath[i][j] != -1 && !board->_wall[i][j]) { // ����� �Ϻ�
                escapePath.emplace_back(i, j);
                std::cout << "routePoint: " << i << ", " << j << std::endl;
            }
        }
    }

    // ��� ���� Ȯ��
    int pathLength = escapePath.size();
    if (pathLength < 3) {
        std::cerr << "Error: Path is too short to place objects." << std::endl;
        return;
    }

    // ����� 1/3 �� 2/3 ��ġ ���
    int index1 = pathLength / 3;
    int index2 = (2 * pathLength) / 3;

    // ��� ���� ��ǥ ��������
    objPosition1 = escapePath[index1];
    objPosition2 = escapePath[index2];

    // OBJ �� ��ġ ����
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

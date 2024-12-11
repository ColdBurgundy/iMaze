#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>

struct Vertex {
    float x, y, z;
};

struct Normal {
    float x, y, z;
};

struct Face {
    unsigned int v1, v2, v3; // Indices for vertices
    unsigned int n1, n2, n3; // Indices for normals
};

class ObjLoader {
public:

    // 생성자
    ObjLoader();

    bool loadOBJ(const std::string& filename); // OBJ 파일 로드
    void renderOBJ() const; // OBJ 렌더링
    void setMazePosition(float mazeX, float mazeY);  // 미로 내 위치 설정
    void setRotation(float angleX, float angleY, float angleZ);
    void updateRotation(); // 회전 각도 업데이트


private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    // 크기 및 위치 조정 관련 변수
    float scale;
    float offsetX, offsetY, offsetZ;

    float currentRotation; // 현재 회전 각도

    // 미로 내 위치
    float mazeX, mazeY;
    float rotationX, rotationY, rotationZ;


    float minX = 1e9, minY = 1e9, minZ = 1e9;
    float maxX = -1e9, maxY = -1e9, maxZ = -1e9;
    float maxDimension;
};

#endif

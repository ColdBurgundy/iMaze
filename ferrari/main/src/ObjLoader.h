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

    // ������
    ObjLoader();

    bool loadOBJ(const std::string& filename); // OBJ ���� �ε�
    void renderOBJ() const; // OBJ ������
    void setMazePosition(float mazeX, float mazeY);  // �̷� �� ��ġ ����
    void setRotation(float angleX, float angleY, float angleZ);
    void updateRotation(); // ȸ�� ���� ������Ʈ


private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Face> faces;

    // ũ�� �� ��ġ ���� ���� ����
    float scale;
    float offsetX, offsetY, offsetZ;

    float currentRotation; // ���� ȸ�� ����

    // �̷� �� ��ġ
    float mazeX, mazeY;
    float rotationX, rotationY, rotationZ;


    float minX = 1e9, minY = 1e9, minZ = 1e9;
    float maxX = -1e9, maxY = -1e9, maxZ = -1e9;
    float maxDimension;
};

#endif

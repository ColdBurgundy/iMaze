#include "ObjLoader.h"
#include <algorithm>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <iostream>

ObjLoader::ObjLoader() : scale(1.0f), offsetX(0.0f), offsetY(0.0f), offsetZ(0.0f),
mazeX(0.0f), mazeY(0.0f), rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f),
currentRotation(0.0f) {}

bool ObjLoader::loadOBJ(const std::string& filename) {

    std::cout << "Start to loadOBJ: " << filename << "..." << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return false;
    }


    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string type;
        lineStream >> type;

        if (type == "v") {
            Vertex vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);

            // Bounding box 계산
            minX = std::min(minX, vertex.x);
            minY = std::min(minY, vertex.y);
            minZ = std::min(minZ, vertex.z);
            maxX = std::max(maxX, vertex.x);
            maxY = std::max(maxY, vertex.y);
            maxZ = std::max(maxZ, vertex.z);
        }
        else if (type == "vn") {
            Normal normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "f") {
            std::vector<std::string> vertexData;
            std::string vertexString;

            while (lineStream >> vertexString) {
                vertexData.push_back(vertexString);
            }

            auto parseVertexIndex = [](const std::string& vertexData, unsigned int& vertexIndex, unsigned int& normalIndex) {
                std::istringstream ss(vertexData);
                std::string index;
                std::getline(ss, index, '/');
                vertexIndex = std::stoi(index) - 1;
                std::getline(ss, index, '/'); // Skip texture index
                std::getline(ss, index, '/');
                normalIndex = std::stoi(index) - 1;
                };

            if (vertexData.size() >= 3) {
                for (size_t i = 1; i < vertexData.size() - 1; ++i) {
                    try {
                        Face face;
                        parseVertexIndex(vertexData[0], face.v1, face.n1);
                        parseVertexIndex(vertexData[i], face.v2, face.n2);
                        parseVertexIndex(vertexData[i + 1], face.v3, face.n3);

                        if (face.v1 < vertices.size() && face.v2 < vertices.size() && face.v3 < vertices.size() &&
                            face.n1 < normals.size() && face.n2 < normals.size() && face.n3 < normals.size()) {
                            faces.push_back(face);
                        }
                        else {
                            std::cerr << "Invalid face indices in OBJ file. Skipping face segment." << std::endl;
                        }
                    }
                    catch (...) {
                        std::cerr << "Error parsing face line segment." << std::endl;
                    }
                }
            }
        }
    }
    file.close();

    // 모델 크기 계산
    float modelWidth = maxX - minX;
    float modelHeight = maxY - minY;
    float modelDepth = maxZ - minZ;

    // 한 칸에 맞추기 위해 스케일 계산
    maxDimension = std::max({ modelWidth, modelHeight, modelDepth });
    scale = (1.0f / maxDimension) * 0.5; // 80% size

    // 모델을 바닥에 맞추기 위해 Z 방향으로 이동
    offsetX = -(minX + maxX) / 2.0f; // 중심 X를 원점으로
    offsetY = -(minY + maxY) / 2.0f; // 중심 Y를 원점으로
    offsetZ = -minZ;                 // 바닥 Z를 0으로 이동

    std::cout << "Complete to loadOBJ: " << filename << std::endl;
    std::cout << "modelWidth: " << modelWidth << std::endl;
    std::cout << "modelHeight: " << modelHeight << std::endl;
    std::cout << "modelDepth: " << modelDepth << std::endl;
    std::cout << "Model scaled by: " << scale << std::endl;
    std::cout << "Model offset: (" << offsetX << ", " << offsetY << ", " << offsetZ << ")" << std::endl;

    return true;
}

void ObjLoader::updateRotation() {
    currentRotation += 0.25f; // 매 호출마다 1도씩 회전
    if (currentRotation >= 360.0f) {
        currentRotation -= 360.0f; // 360도를 넘어가면 0도로 초기화
    }
}

// 미로 내 위치 설정
void ObjLoader::setMazePosition(float mazeX, float mazeY) {
    this->mazeX = mazeX;
    this->mazeY = mazeY;
}

void ObjLoader::setRotation(float angleX, float angleY, float angleZ) {
    this->rotationX = angleX;
    this->rotationY = angleY;
    this->rotationZ = angleZ;
}


void ObjLoader::renderOBJ() const {
    glPushMatrix();

    // 미로의 특정 위치에 배치
    glTranslatef(mazeX, mazeY, (maxZ - minZ)/2.0f*scale+0.3);

    // 모델 기본 세팅: 회전 적용
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
    // 애니메이션 회전 적용
    glRotatef(currentRotation, 0.0f, 1.0f, 0.0f); // Y축을 기준으로 회전
    // 크기 및 모델 중심 변환
    glScalef(scale, scale, scale);
    glTranslatef(offsetX, offsetY, offsetZ);


    

    glEnable(GL_LIGHTING); // 조명 활성화
    glDisable(GL_TEXTURE_2D); // 텍스처 비활성화
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // OBJ 렌더링
   


    glBegin(GL_TRIANGLES);
    for (const auto& face : faces) {
        if (face.v1 < vertices.size() && face.v2 < vertices.size() && face.v3 < vertices.size() &&
            face.n1 < normals.size() && face.n2 < normals.size() && face.n3 < normals.size()) {
            const Vertex& v1 = vertices[face.v1];
            const Vertex& v2 = vertices[face.v2];
            const Vertex& v3 = vertices[face.v3];

            const Normal& n1 = normals[face.n1];
            const Normal& n2 = normals[face.n2];
            const Normal& n3 = normals[face.n3];

            // 첫 번째 정점
            glNormal3f(n1.x, n1.y, n1.z); // 법선 지정
            glColor3f(0.9f, 0.75f, 0.3f); // 중간 금색
            glVertex3f(v1.x, v1.y, v1.z);

            // 두 번째 정점
            glNormal3f(n2.x, n2.y, n2.z); // 법선 지정
            glColor3f(0.9f, 0.75f, 0.3f); // 중간 금색
            glVertex3f(v2.x, v2.y, v2.z);

            // 세 번째 정점
            glNormal3f(n3.x, n3.y, n3.z); // 법선 지정
            glColor3f(0.9f, 0.75f, 0.3f); // 중간 금색
            glVertex3f(v3.x, v3.y, v3.z);
        }
    }
    glEnd();
    glEnable(GL_TEXTURE_2D); // 텍스처 활성화

    glPopMatrix();
}

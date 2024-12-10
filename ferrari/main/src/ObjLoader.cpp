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
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return false;
    }

    float minX = 1e9, minY = 1e9, minZ = 1e9;
    float maxX = -1e9, maxY = -1e9, maxZ = -1e9;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string type;
        lineStream >> type;

        if (type == "v") {
            Vertex vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);

            // Bounding box ���
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

    // �� ũ�� ���
    float modelWidth = maxX - minX;
    float modelHeight = maxY - minY;
    float modelDepth = maxZ - minZ;

    // �� ĭ�� ���߱� ���� ������ ���
    float maxDimension = std::max({ modelWidth, modelHeight, modelDepth });
    scale = (1.0f / maxDimension) * 0.5; // 80% size

    // ���� �ٴڿ� ���߱� ���� Z �������� �̵�
    offsetX = -(minX + maxX) / 2.0f; // �߽� X�� ��������
    offsetY = -(minY + maxY) / 2.0f; // �߽� Y�� ��������
    offsetZ = -minZ;                 // �ٴ� Z�� 0���� �̵�

    std::cout << "Total vertices loaded: " << vertices.size() << std::endl;
    std::cout << "Total normals loaded: " << normals.size() << std::endl;
    std::cout << "Total faces loaded: " << faces.size() << std::endl;
    std::cout << "Model scaled by: " << scale << std::endl;
    std::cout << "Model offset: (" << offsetX << ", " << offsetY << ", " << offsetZ << ")" << std::endl;

    return true;
}

void ObjLoader::updateRotation() {
    currentRotation += 0.25f; // �� ȣ�⸶�� 1���� ȸ��
    if (currentRotation >= 360.0f) {
        currentRotation -= 360.0f; // 360���� �Ѿ�� 0���� �ʱ�ȭ
    }
}

// �̷� �� ��ġ ����
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

    //tmp position
    /*
    glTranslatef(5.0f, -5.0f, 2.0f); // ������ �̷� �߽ɿ� ��ġ
    glScalef(0.1f, 0.1f, 0.1f); // ũ�� ����
    */
    // �̷��� Ư�� ��ġ�� ��ġ
    glTranslatef(mazeX, mazeY, 0.0f);

    // �� �⺻ ����: ȸ�� ����
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);

    // ũ�� �� �� �߽� ��ȯ
    glScalef(scale, scale, scale);
    glTranslatef(offsetX, offsetY, offsetZ);


    // �ִϸ��̼� ȸ�� ����
    glRotatef(currentRotation, 0.0f, 1.0f, 0.0f); // Y���� �������� ȸ��


    // ���� ����
    GLfloat ambient[] = { 0.4f, 0.35f, 0.2f, 1.0f };  // �ֺ���
    GLfloat diffuse[] = { 0.8f, 0.7f, 0.5f, 1.0f };  // ���ݻ籤
    GLfloat specular[] = { 0.9f, 0.8f, 0.6f, 1.0f }; // �ݻ籤
    GLfloat shininess[] = { 30.0f };                 // ����

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

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

            glNormal3f(n1.x, n1.y, n1.z);
            glVertex3f(v1.x, v1.y, v1.z);

            glNormal3f(n2.x, n2.y, n2.z);
            glVertex3f(v2.x, v2.y, v2.z);

            glNormal3f(n3.x, n3.y, n3.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
    }
    glEnd();
    glPopMatrix();
}

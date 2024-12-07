#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

struct Vertex {
    float x, y, z;
};

struct Face {
    unsigned int v1, v2, v3; // Indices for vertices
};

std::vector<Vertex> vertices;
std::vector<Face> faces;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 300.0f;
float rotationAngleY = 0.0f;
float rotationAngleX = 0.0f;

// ī�޶� ���� ��� �Լ�
void printCameraInfo() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Camera Position: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")";
    std::cout << " Rotation: (Y: " << rotationAngleY << "��, X: " << rotationAngleX << "��)" << std::endl;
}

// OBJ ���� �ε� �Լ�
void loadOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        exit(1);
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
        }
        else if (type == "f") {
            std::string vertexData1, vertexData2, vertexData3;
            lineStream >> vertexData1 >> vertexData2 >> vertexData3;

            auto parseVertexIndex = [](const std::string& vertexData) -> unsigned int {
                std::istringstream ss(vertexData);
                std::string index;
                std::getline(ss, index, '/'); // '/' �������� ����
                return std::stoi(index) - 1;  // OBJ ������ 1-based indexing
                };

            try {
                Face face;
                face.v1 = parseVertexIndex(vertexData1);
                face.v2 = parseVertexIndex(vertexData2);
                face.v3 = parseVertexIndex(vertexData3);

                // ��ȿ�� �˻�
                if (face.v1 < vertices.size() && face.v2 < vertices.size() && face.v3 < vertices.size()) {
                    faces.push_back(face);
                }
                else {
                    std::cerr << "Invalid face indices in OBJ file. Skipping face: "
                        << vertexData1 << " " << vertexData2 << " " << vertexData3 << std::endl;
                }
            }
            catch (...) {
                std::cerr << "Error parsing face line: " << line << std::endl;
            }
        }
    }
    file.close();

    // ����� ���� ���
    std::cout << "Total vertices loaded: " << vertices.size() << std::endl;
    std::cout << "Total faces loaded: " << faces.size() << std::endl;
}

// ������ �Լ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶� ����
    glTranslatef(-cameraX, -cameraY, -cameraZ);
    glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // �ݼ� ���� ����
    GLfloat ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat specular[] = { 0.77f, 0.77f, 0.77f, 1.0f };
    GLfloat shininess[] = { 76.8f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glScalef(1.0f, 1.0f, 1.0f); // �� ũ�� ����

    // �� ������
    glBegin(GL_TRIANGLES);
    for (const auto& face : faces) {
        if (face.v1 < vertices.size() && face.v2 < vertices.size() && face.v3 < vertices.size()) {
            const Vertex& v1 = vertices[face.v1];
            const Vertex& v2 = vertices[face.v2];
            const Vertex& v3 = vertices[face.v3];

            glNormal3f(0.0f, 0.0f, 1.0f); // �ܼ� ���� ����

            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
    }
    glEnd();

    glDisable(GL_LIGHTING);

    glutSwapBuffers();

    // ī�޶� ���� ���
    printCameraInfo();
}

// Ű���� �Է� ó�� �Լ�
void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 10.0f;
    const float rotateSpeed = 5.0f;

    switch (key) {
    case 'w': cameraZ -= moveSpeed; break; // ������ �̵�
    case 's': cameraZ += moveSpeed; break; // �ڷ� �̵�
    case 'a': cameraX -= moveSpeed; break; // ���� �̵�
    case 'd': cameraX += moveSpeed; break; // ������ �̵�
    case 'z': cameraY += moveSpeed; break; // ���� �̵�
    case 'c': cameraY -= moveSpeed; break; // �Ʒ��� �̵�
    case 'q': rotationAngleY -= rotateSpeed; break; // ���� ȸ��
    case 'e': rotationAngleY += rotateSpeed; break; // ������ ȸ��
    }

    glutPostRedisplay();
}

// OpenGL �ʱ�ȭ �Լ�
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 1000.0); // FOV, Aspect Ratio, Near/Far Clipping
    glMatrixMode(GL_MODELVIEW);

    // ���� ����
    GLfloat lightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

int main(int argc, char** argv) {
    std::string objFilePath = "C:\\Projects\\CG_Practice\\models\\iMAC.obj";
    loadOBJ(objFilePath);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OBJ Model Viewer");

    initOpenGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}

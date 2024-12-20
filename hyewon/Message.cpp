#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <windows.h> // For BMP loading

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

std::vector<Vertex> vertices;
std::vector<Normal> normals;
std::vector<Face> faces;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 100.0f;
float rotationAngleY = 0.0f;
float rotationAngleX = 0.0f;

GLuint bmpTextureID;
bool displayImage = false; // Flag to control when to display the image

void printCameraInfo() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Camera Position: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")";
    std::cout << " Rotation: (Y: " << rotationAngleY << "\u00b0, X: " << rotationAngleX << "\u00b0)" << std::endl;
}

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

    std::cout << "Total vertices loaded: " << vertices.size() << std::endl;
    std::cout << "Total normals loaded: " << normals.size() << std::endl;
    std::cout << "Total faces loaded: " << faces.size() << std::endl;
}

GLuint loadBMP(const char* filepath) {
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER info;
    unsigned char* data;

    FILE* file;
    fopen_s(&file, filepath, "rb");
    if (!file) {
        std::cerr << "Failed to load BMP file: " << filepath << std::endl;
        return 0;
    }

    fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&info, sizeof(BITMAPINFOHEADER), 1, file);

    if (header.bfType != 0x4D42) {
        std::cerr << "Not a valid BMP file." << std::endl;
        fclose(file);
        return 0;
    }

    data = new unsigned char[info.biSizeImage];
    fseek(file, header.bfOffBits, SEEK_SET);
    fread(data, 1, info.biSizeImage, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.biWidth, info.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
    return textureID;
}

void displayBMP() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bmpTextureID);

    glBegin(GL_QUADS);
    // ���� ũ�⸦ ��¦ �ø� (y�� �� ��ǥ�� 50 �ȼ� ����)
    glTexCoord2f(0.0f, 0.0f); glVertex2f(200, 50);   // ���� �Ʒ�
    glTexCoord2f(1.0f, 0.0f); glVertex2f(800, 50);  // ������ �Ʒ�
    glTexCoord2f(1.0f, 1.0f); glVertex2f(800, 400); // ������ �� 
    glTexCoord2f(0.0f, 1.0f); glVertex2f(200, 400);  // ���� �� 
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(-cameraX, -cameraY, -cameraZ);
    glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess[] = { 50.0f };

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

    glDisable(GL_LIGHTING);

    if (displayImage) {
        displayBMP();
    }
    glutSwapBuffers();

    printCameraInfo();
}

void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 5.0f;
    const float rotateSpeed = 2.5f;

    switch (key) {
    case 'w':
        cameraZ -= moveSpeed;
        if (cameraZ <= 50.0f) { // Adjust the threshold as needed
            displayImage = true;
        }
        break;
    case 's': cameraZ += moveSpeed; break;
    case 'a': cameraX -= moveSpeed; break;
    case 'd': cameraX += moveSpeed; break;
    case 'z': cameraY += moveSpeed; break;
    case 'c': cameraY -= moveSpeed; break;
    case 'q': rotationAngleY -= rotateSpeed; break;
    case 'e': rotationAngleY += rotateSpeed; break;
    }

    glutPostRedisplay();
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    GLfloat lightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    bmpTextureID = loadBMP("C:\\Projects\\CG_Practice\\images\\stage2_imac.bmp");
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

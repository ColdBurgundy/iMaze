#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Vertex and Face structures
struct Vertex {
    float x, y, z;
};

struct Face {
    int vertexIndices[3];
};

// Global variables for OBJ data
std::vector<Vertex> vertices;
std::vector<Face> faces;

// Load OBJ file
void loadOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
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
            Face face;
            for (int i = 0; i < 3; ++i) {
                std::string vertexData;
                lineStream >> vertexData;

                std::istringstream vertexStream(vertexData);
                int vertexIndex;
                vertexStream >> vertexIndex;

                face.vertexIndices[i] = vertexIndex - 1; // OBJ is 1-indexed
            }
            faces.push_back(face);
        }
    }

    file.close();
    std::cout << "OBJ file loaded successfully: " << vertices.size() << " vertices, " << faces.size() << " faces" << std::endl;
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라 위치를 조정
    glTranslatef(0.0f, 0.0f, -5.0f);

    // 모델 크기를 조정
    glScalef(0.1f, 0.1f, 0.1f);

    glBegin(GL_TRIANGLES);
    for (const auto& face : faces) {
        for (int i = 0; i < 3; ++i) {
            if (static_cast<size_t>(face.vertexIndices[i]) >= vertices.size()) {
                std::cerr << "Invalid vertex index: " << face.vertexIndices[i] << std::endl;
                continue;
            }

            const Vertex& vertex = vertices[face.vertexIndices[i]];
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();

    glutSwapBuffers();
}


// Reshape callback function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(w) / static_cast<double>(h), 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Initialize OpenGL
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // 조명 설정
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = { 0.0f, 0.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.0f, 1.0f, 1.0f);
}


int main(int argc, char** argv) {
    // Provide the correct path for the OBJ file
    std::string objFilePath = R"(D:\중앙대학교\2024\컴퓨터그래픽스\team\team\Iphone_seceond_version_finished.obj)";
    loadOBJ(objFilePath);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL OBJ Viewer");

    initOpenGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

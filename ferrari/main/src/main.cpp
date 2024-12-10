#include "ObjLoader.h"
#include "Board.h"
#include "Player.h"
#include "Collision.h"
#include "Camera.h"
#include "LightingManager.h"
#include <GL/glut.h>

// ���� ��ü
Board* maze;
Player* player;
Camera* camera;
ObjLoader objLoader;
LightingManager lightingManager; // LightingManager �߰�

void display() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶� ����
    gluLookAt(camera->getPositionX(), camera->getPositionY(), camera->getPositionZ(),
        camera->getPositionX() + camera->getFocusX(),
        camera->getPositionY() + camera->getFocusY(),
        camera->getPositionZ() + camera->getFocusZ(),
        0.0, 0.0, 1.0);

    // �̷� ������
    for (int i = 0; i < maze->getHeight(); ++i) {
        for (int j = 0; j < maze->getWidth(); ++j) {
            if (maze->getCell(j, i) == 1) {
                glPushMatrix();
                glTranslatef(j, -i, 0.0f);
                glColor3f(1.0f, 0.0f, 0.0f);
                glutSolidCube(1.0);
                glPopMatrix();
            }
        }
    }

    // OBJ �� ������
    objLoader.setMazePosition(1.0f, -1.0f);
    objLoader.setRotation(90.0f, 0.0f, 0.0f); // X�� ȸ��
    objLoader.renderOBJ();

    glutSwapBuffers(); 
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    camera->updatePosition(key);
}

void mouseMotion(int x, int y) {
    camera->updateCameraDirection(x, y);
}

void idle() {
    objLoader.updateRotation(); // ȸ�� ���� ������Ʈ
    glutPostRedisplay();        // ȭ�� ���� ��û
}


void init() {
    // OpenGL �⺻ ����
    glEnable(GL_DEPTH_TEST);        // ���� �׽�Ʈ
    glDepthFunc(GL_LEQUAL);         // ���� �� �Լ�
    glClearDepth(1.0f);             // ���� ���� �ʱ�ȭ
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // ����

    glEnable(GL_COLOR_MATERIAL);    // ���� ���� Ȱ��ȭ
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // ���� �ʱ�ȭ
    lightingManager.initLighting();

    // OBJ �� �ε�
    if (!objLoader.loadOBJ(".\\bin\\imac.obj")) {
        exit(1);
    }

    // ���콺 �ʱ� ��ġ ����
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    glutWarpPointer(centerX, centerY);
}

int main(int argc, char** argv) {
    // GLUT �ʱ�ȭ
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Maze Game");

    // ��ü �ʱ�ȭ
    maze = new Board(51, 51);
    player = new Player(1, 1);
    camera = new Camera();

    // OpenGL �ʱ�ȭ
    init();

    // �ݹ� �Լ� ����
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle); // Idle �Լ� ���

    // ���� ���� ����
    glutMainLoop();

    // �޸� ����
    delete maze;
    delete player;
    delete camera;

    return 0;
}

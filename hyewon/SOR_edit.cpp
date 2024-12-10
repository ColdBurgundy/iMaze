#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// ī�޶��� ��ġ
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 200.0f;

// ������Ʈ�� ��ġ
float rightObjectX = 50.0f, rightObjectY = 0.0f, rightObjectZ = 0.0f;
float leftObjectX = -50.0f, leftObjectY = 0.0f, leftObjectZ = 0.0f;

// ������Ʈ�� ī�޶� ���� �浹 �Ÿ�
float collisionDistance = 10.0f;

// Ű���� �Է� ó��
void processKeys(unsigned char key, int x, int y) {
    if (key == 'w') {
        cameraZ -= 5.0f; // ī�޶� ����
    }
    else if (key == 's') {
        cameraZ += 5.0f; // ī�޶� ����
    }
    else if (key == 'a') {
        cameraX -= 5.0f; // ī�޶� ���� �̵�
    }
    else if (key == 'd') {
        cameraX += 5.0f; // ī�޶� ������ �̵�
    }

    // ������ ������Ʈ�� ��Ҵ��� Ȯ��
    float distToRight = sqrt(pow(cameraX - rightObjectX, 2) + pow(cameraY - rightObjectY, 2) + pow(cameraZ - rightObjectZ, 2));
    if (distToRight < collisionDistance) {
        printf("Collided with right object! Moving to left object...\n");
        cameraX = leftObjectX;
        cameraY = leftObjectY;
        cameraZ = leftObjectZ + 20.0f; // ��¦ �տ� ��ġ
    }

    // ���� ������Ʈ�� ��Ҵ��� Ȯ��
    float distToLeft = sqrt(pow(cameraX - leftObjectX, 2) + pow(cameraY - leftObjectY, 2) + pow(cameraZ - leftObjectZ, 2));
    if (distToLeft < collisionDistance) {
        printf("Collided with left object! Moving to right object...\n");
        cameraX = rightObjectX;
        cameraY = rightObjectY;
        cameraZ = rightObjectZ + 20.0f; // ��¦ �տ� ��ġ
    }

    glutPostRedisplay();
}

// OpenGL �ʱ�ȭ
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// ���÷��� �ݹ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶� ��ġ ����
    gluLookAt(cameraX, cameraY, cameraZ, cameraX, cameraY, cameraZ - 10.0f, 0.0f, 1.0f, 0.0f);

    // ���� ������Ʈ ������
    glPushMatrix();
    glTranslatef(leftObjectX, leftObjectY, leftObjectZ);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(5.0, 20, 20);
    glPopMatrix();

    // ������ ������Ʈ ������
    glPushMatrix();
    glTranslatef(rightObjectX, rightObjectY, rightObjectZ);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidSphere(5.0, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

// ������ ũ�� ���� ó��
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Object Collision and Camera Teleportation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processKeys);

    glutMainLoop();
    return 0;
}

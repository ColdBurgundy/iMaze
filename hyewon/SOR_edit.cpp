#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// 카메라의 위치
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 200.0f;

// 오브젝트의 위치
float rightObjectX = 50.0f, rightObjectY = 0.0f, rightObjectZ = 0.0f;
float leftObjectX = -50.0f, leftObjectY = 0.0f, leftObjectZ = 0.0f;

// 오브젝트와 카메라 간의 충돌 거리
float collisionDistance = 10.0f;

// 키보드 입력 처리
void processKeys(unsigned char key, int x, int y) {
    if (key == 'w') {
        cameraZ -= 5.0f; // 카메라 전진
    }
    else if (key == 's') {
        cameraZ += 5.0f; // 카메라 후진
    }
    else if (key == 'a') {
        cameraX -= 5.0f; // 카메라 왼쪽 이동
    }
    else if (key == 'd') {
        cameraX += 5.0f; // 카메라 오른쪽 이동
    }

    // 오른쪽 오브젝트에 닿았는지 확인
    float distToRight = sqrt(pow(cameraX - rightObjectX, 2) + pow(cameraY - rightObjectY, 2) + pow(cameraZ - rightObjectZ, 2));
    if (distToRight < collisionDistance) {
        printf("Collided with right object! Moving to left object...\n");
        cameraX = leftObjectX;
        cameraY = leftObjectY;
        cameraZ = leftObjectZ + 20.0f; // 살짝 앞에 배치
    }

    // 왼쪽 오브젝트에 닿았는지 확인
    float distToLeft = sqrt(pow(cameraX - leftObjectX, 2) + pow(cameraY - leftObjectY, 2) + pow(cameraZ - leftObjectZ, 2));
    if (distToLeft < collisionDistance) {
        printf("Collided with left object! Moving to right object...\n");
        cameraX = rightObjectX;
        cameraY = rightObjectY;
        cameraZ = rightObjectZ + 20.0f; // 살짝 앞에 배치
    }

    glutPostRedisplay();
}

// OpenGL 초기화
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// 디스플레이 콜백
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라 위치 설정
    gluLookAt(cameraX, cameraY, cameraZ, cameraX, cameraY, cameraZ - 10.0f, 0.0f, 1.0f, 0.0f);

    // 왼쪽 오브젝트 렌더링
    glPushMatrix();
    glTranslatef(leftObjectX, leftObjectY, leftObjectZ);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(5.0, 20, 20);
    glPopMatrix();

    // 오른쪽 오브젝트 렌더링
    glPushMatrix();
    glTranslatef(rightObjectX, rightObjectY, rightObjectZ);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidSphere(5.0, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

// 윈도우 크기 변경 처리
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

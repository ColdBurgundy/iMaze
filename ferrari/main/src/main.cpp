#include "ObjLoader.h"
#include "Board.h"
#include "Player.h"
#include "Collision.h"
#include "Camera.h"
#include "LightingManager.h"
#include <GL/glut.h>

// 전역 객체
Board* maze;
Player* player;
Camera* camera;
ObjLoader objLoader;
LightingManager lightingManager; // LightingManager 추가

void display() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라 설정
    gluLookAt(camera->getPositionX(), camera->getPositionY(), camera->getPositionZ(),
        camera->getPositionX() + camera->getFocusX(),
        camera->getPositionY() + camera->getFocusY(),
        camera->getPositionZ() + camera->getFocusZ(),
        0.0, 0.0, 1.0);

    // 미로 렌더링
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

    // OBJ 모델 렌더링
    objLoader.setMazePosition(1.0f, -1.0f);
    objLoader.setRotation(90.0f, 0.0f, 0.0f); // X축 회전
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
    objLoader.updateRotation(); // 회전 각도 업데이트
    glutPostRedisplay();        // 화면 갱신 요청
}


void init() {
    // OpenGL 기본 설정
    glEnable(GL_DEPTH_TEST);        // 깊이 테스트
    glDepthFunc(GL_LEQUAL);         // 깊이 비교 함수
    glClearDepth(1.0f);             // 깊이 버퍼 초기화
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 배경색

    glEnable(GL_COLOR_MATERIAL);    // 색상 재질 활성화
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // 조명 초기화
    lightingManager.initLighting();

    // OBJ 모델 로드
    if (!objLoader.loadOBJ(".\\bin\\imac.obj")) {
        exit(1);
    }

    // 마우스 초기 위치 설정
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    glutWarpPointer(centerX, centerY);
}

int main(int argc, char** argv) {
    // GLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Maze Game");

    // 객체 초기화
    maze = new Board(51, 51);
    player = new Player(1, 1);
    camera = new Camera();

    // OpenGL 초기화
    init();

    // 콜백 함수 설정
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle); // Idle 함수 등록

    // 메인 루프 실행
    glutMainLoop();

    // 메모리 해제
    delete maze;
    delete player;
    delete camera;

    return 0;
}

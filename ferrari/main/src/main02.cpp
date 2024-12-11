#include <GL/glut.h>
#include "StageManager.h"
#include "Camera.h"
#include "Player.h"
#include "LightingManager.h"

// 전역 객체
StageManager* stageManager;
Camera* camera;
Player* player;
LightingManager lightingManager;


void applyDefaultMaterial() {
    // 주변광: 부드럽고 은은한 조명 효과
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

    // 난반사광: 물체의 표면 질감을 강조
    GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    // 반사광: 빛나는 하이라이트 (금속성 효과)
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // 광택: 하이라이트의 크기 (값이 클수록 작고 강한 하이라이트)
    GLfloat shininess = 50.0f;

    // OpenGL 재질 설정
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}


// 바닥과 벽을 렌더링하는 함수
void renderStage(Stage* stage) {
    Board* maze = stage->getBoard();
    // 바닥 텍스처 렌더링
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, stage->getFloorTexture());
    glColor3f(1.0f, 1.0f, 1.0f); // 밝은 색상 유지
    glDisable(GL_LIGHTING); // 조명 비활성화

    glBegin(GL_QUADS);
    for (int i = 0; i < maze->getHeight(); ++i) {
        for (int j = 0; j < maze->getWidth(); ++j) {
            float x1 = j, x2 = j + 1;
            float y1 = -i, y2 = -(i + 1);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, y1, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, y2, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y2, 0.0f);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING); // 조명 다시 활성화

    // 벽 텍스처 렌더링
    glBindTexture(GL_TEXTURE_2D, stage->getWallTexture());
    glColor3f(1.0f, 1.0f, 1.0f); // 텍스처를 제대로 렌더링하기 위해 색상 설정

    for (int i = 0; i < maze->getHeight(); ++i) {
        for (int j = 0; j < maze->getWidth(); ++j) {
            if (maze->getCell(j, i) == 1) {
                glPushMatrix();

                // 벽의 위치와 크기 설정
                glTranslatef(j + 0.5f, -i - 0.5f, 0.5f); // 벽의 중심을 배치
                glScalef(1.0f, 1.0f, 1.0f);

                // 벽의 텍스처 매핑
                glBegin(GL_QUADS);

                // 앞면
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

                // 뒷면
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

                // 왼쪽면
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

                // 오른쪽면
                glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);

                // 윗면
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

                // 아랫면
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

                glEnd();
                glPopMatrix();
            }
        }
    }
    glDisable(GL_TEXTURE_2D); // 텍스처 비활성화
}



// 디스플레이 콜백 함수
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라의 현재 위치
    float camX = camera->getPositionX();
    float camY = camera->getPositionY();
    float camZ = camera->getPositionZ();

    // 카메라의 초점 방향
    float focusX = camera->getFocusX();
    float focusY = camera->getFocusY();
    float focusZ = camera->getFocusZ();

    // 플레이어와 카메라 간의 고정된 거리
    float fixedDistance = 0.7f; // 원하는 고정 거리
    float upOffset = 0.5f;      // 카메라 높이 오프셋

    // 눈의 위치 계산
    float eyeX = camX - focusX * fixedDistance;
    float eyeY = camY - focusY * fixedDistance;
    float eyeZ = camZ - focusZ * fixedDistance + upOffset;

    // 초점 위치는 플레이어의 중심
    float centerX = camX;
    float centerY = camY;
    float centerZ = camZ;

    // 업 벡터
    float upX = 0.0f;
    float upY = 0.0f;
    float upZ = 1.0f;

    gluLookAt(
        eyeX, eyeY, eyeZ,   // 눈의 위치
        centerX, centerY, centerZ, // 초점 위치
        upX, upY, upZ       // 업 벡터
    );

    // 재질 설정 적용
    applyDefaultMaterial();

    Stage* currentStage = stageManager->getCurrentStage();
    if (currentStage) {
        glDisable(GL_LIGHTING); // 조명 비활성화
        glColor3f(1.0f, 1.0f, 1.0f); // 기본 흰색으로 복원
        renderStage(currentStage);
        glEnable(GL_LIGHTING);  // 조명 활성화


        // OBJ 렌더링
        glPushMatrix();
        currentStage->getObjLoader1()->setRotation(90.0f, 0.0f, 0.0f);
        currentStage->getObjLoader1()->renderOBJ();
        glPopMatrix();

        glPushMatrix();
        currentStage->getObjLoader2()->setRotation(90.0f, 0.0f, 0.0f);
        currentStage->getObjLoader2()->renderOBJ();
        glPopMatrix();

    }

    glutSwapBuffers();
}


// 키보드 콜백 함수
void keyboard(unsigned char key, int x, int y) {
    camera->updatePosition(key);
    //player->move(key, camera->getForwardX(), camera->getForwardY());
    glutPostRedisplay();

    // 다음 스테이지로 이동
    if (key == 'n' && stageManager->hasNextStage()) {
        stageManager->nextStage();
    }
}

// 마우스 모션 콜백 함수
void mouseMotion(int x, int y) {
    camera->updateCameraDirection(x, y);
    //player->setDirection(camera->getForwardX(), camera->getForwardY());
    glutPostRedisplay();
}

// 초기화 함수
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);         // 깊이 비교 함수
    glClearDepth(1.0f);             // 깊이 버퍼 초기화
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    lightingManager.initLighting();

    // 스테이지 초기화
    stageManager = new StageManager();

    // 스테이지 추가
    stageManager->addStage(new Stage(11, ".\\bin\\stage1_floor.bmp", ".\\bin\\stage1_wall.bmp",
        ".\\bin\\macbookair1.obj", ".\\bin\\macintoshi.obj"));
    stageManager->addStage(new Stage(19, ".\\bin\\stage2_floor.bmp", ".\\bin\\stage2_wall.bmp",
        ".\\bin\\imac.obj", ".\\bin\\ipad3.obj"));
    stageManager->addStage(new Stage(25, ".\\bin\\stage3_floor.bmp", ".\\bin\\stage3_wall.bmp",
        ".\\bin\\ipad_pro.obj", ".\\bin\\iphone16.obj"));

    camera = new Camera();
    player = new Player(0.5f, 0.5f, 0.3f);

    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    glutWarpPointer(centerX, centerY);

}
void idle() {
    // 회전 각도 업데이트
    Stage* currentStage = stageManager->getCurrentStage();
    currentStage->getObjLoader1()->updateRotation();
    currentStage->getObjLoader2()->updateRotation();

    glutPostRedisplay();        // 화면 갱신 요청
}
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Maze Game with Stages");


    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle); // Idle 함수 등록

    init();
    glutMainLoop();

    delete stageManager;
    delete camera;
    delete player;

    return 0;
}

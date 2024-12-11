#include <GL/glut.h>
#include "StageManager.h"
#include "Camera.h"
#include "Player.h"
#include "LightingManager.h"

// ���� ��ü
StageManager* stageManager;
Camera* camera;
Player* player;
LightingManager lightingManager;


void applyDefaultMaterial() {
    // �ֺ���: �ε巴�� ������ ���� ȿ��
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

    // ���ݻ籤: ��ü�� ǥ�� ������ ����
    GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    // �ݻ籤: ������ ���̶���Ʈ (�ݼӼ� ȿ��)
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // ����: ���̶���Ʈ�� ũ�� (���� Ŭ���� �۰� ���� ���̶���Ʈ)
    GLfloat shininess = 50.0f;

    // OpenGL ���� ����
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}


// �ٴڰ� ���� �������ϴ� �Լ�
void renderStage(Stage* stage) {
    Board* maze = stage->getBoard();
    // �ٴ� �ؽ�ó ������
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, stage->getFloorTexture());
    glColor3f(1.0f, 1.0f, 1.0f); // ���� ���� ����
    glDisable(GL_LIGHTING); // ���� ��Ȱ��ȭ

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
    glEnable(GL_LIGHTING); // ���� �ٽ� Ȱ��ȭ

    // �� �ؽ�ó ������
    glBindTexture(GL_TEXTURE_2D, stage->getWallTexture());
    glColor3f(1.0f, 1.0f, 1.0f); // �ؽ�ó�� ����� �������ϱ� ���� ���� ����

    for (int i = 0; i < maze->getHeight(); ++i) {
        for (int j = 0; j < maze->getWidth(); ++j) {
            if (maze->getCell(j, i) == 1) {
                glPushMatrix();

                // ���� ��ġ�� ũ�� ����
                glTranslatef(j + 0.5f, -i - 0.5f, 0.5f); // ���� �߽��� ��ġ
                glScalef(1.0f, 1.0f, 1.0f);

                // ���� �ؽ�ó ����
                glBegin(GL_QUADS);

                // �ո�
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

                // �޸�
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

                // ���ʸ�
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

                // �����ʸ�
                glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);

                // ����
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

                // �Ʒ���
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

                glEnd();
                glPopMatrix();
            }
        }
    }
    glDisable(GL_TEXTURE_2D); // �ؽ�ó ��Ȱ��ȭ
}



// ���÷��� �ݹ� �Լ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶��� ���� ��ġ
    float camX = camera->getPositionX();
    float camY = camera->getPositionY();
    float camZ = camera->getPositionZ();

    // ī�޶��� ���� ����
    float focusX = camera->getFocusX();
    float focusY = camera->getFocusY();
    float focusZ = camera->getFocusZ();

    // �÷��̾�� ī�޶� ���� ������ �Ÿ�
    float fixedDistance = 0.7f; // ���ϴ� ���� �Ÿ�
    float upOffset = 0.5f;      // ī�޶� ���� ������

    // ���� ��ġ ���
    float eyeX = camX - focusX * fixedDistance;
    float eyeY = camY - focusY * fixedDistance;
    float eyeZ = camZ - focusZ * fixedDistance + upOffset;

    // ���� ��ġ�� �÷��̾��� �߽�
    float centerX = camX;
    float centerY = camY;
    float centerZ = camZ;

    // �� ����
    float upX = 0.0f;
    float upY = 0.0f;
    float upZ = 1.0f;

    gluLookAt(
        eyeX, eyeY, eyeZ,   // ���� ��ġ
        centerX, centerY, centerZ, // ���� ��ġ
        upX, upY, upZ       // �� ����
    );

    // ���� ���� ����
    applyDefaultMaterial();

    Stage* currentStage = stageManager->getCurrentStage();
    if (currentStage) {
        glDisable(GL_LIGHTING); // ���� ��Ȱ��ȭ
        glColor3f(1.0f, 1.0f, 1.0f); // �⺻ ������� ����
        renderStage(currentStage);
        glEnable(GL_LIGHTING);  // ���� Ȱ��ȭ


        // OBJ ������
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


// Ű���� �ݹ� �Լ�
void keyboard(unsigned char key, int x, int y) {
    camera->updatePosition(key);
    //player->move(key, camera->getForwardX(), camera->getForwardY());
    glutPostRedisplay();

    // ���� ���������� �̵�
    if (key == 'n' && stageManager->hasNextStage()) {
        stageManager->nextStage();
    }
}

// ���콺 ��� �ݹ� �Լ�
void mouseMotion(int x, int y) {
    camera->updateCameraDirection(x, y);
    //player->setDirection(camera->getForwardX(), camera->getForwardY());
    glutPostRedisplay();
}

// �ʱ�ȭ �Լ�
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LEQUAL);         // ���� �� �Լ�
    glClearDepth(1.0f);             // ���� ���� �ʱ�ȭ
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    lightingManager.initLighting();

    // �������� �ʱ�ȭ
    stageManager = new StageManager();

    // �������� �߰�
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
    // ȸ�� ���� ������Ʈ
    Stage* currentStage = stageManager->getCurrentStage();
    currentStage->getObjLoader1()->updateRotation();
    currentStage->getObjLoader2()->updateRotation();

    glutPostRedisplay();        // ȭ�� ���� ��û
}
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Maze Game with Stages");


    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle); // Idle �Լ� ���

    init();
    glutMainLoop();

    delete stageManager;
    delete camera;
    delete player;

    return 0;
}

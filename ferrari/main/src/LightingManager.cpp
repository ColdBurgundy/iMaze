#include "LightingManager.h"
#include <GL/glut.h>

void LightingManager::initLighting() {
    // �¾� ���� ����
    GLfloat lightPos0[] = { 5.0f, -5.0f, 10.0f, 1.0f }; // ���� �ָ��� ���� ��
    GLfloat lightAmbient0[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // �ֺ���
    GLfloat lightDiffuse0[] = { 0.8f, 0.8f, 0.8f, 1.0f };   // ���ݻ籤
    GLfloat lightSpecular0[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // �ݻ籤



    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void LightingManager::setLightPosition(float x, float y, float z) {
    GLfloat lightPos0[] = { x, y, z, 1.0f }; // ���ο� ��ġ ����
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}

void LightingManager::updateLightPosition() {
    // ���� ��ġ ������Ʈ�� �߰��Ϸ��� �� �Լ����� ó��
}

#include "LightingManager.h"
#include <GL/glut.h>

void LightingManager::initLighting() {
    // �¾� ���� ����
    GLfloat lightPos0[] = { 0.0f, -1.0f, 1.0f, 1.0f }; // ���� ��ġ�� �� ����

    // ���� �ֺ���
    GLfloat lightAmbient0[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // ���� 0.2f���� ����

    // ���� ���ݻ籤
    GLfloat lightDiffuse0[] = { 0.4f, 0.4f, 0.4f, 1.0f }; // ���� 0.5f~0.6f���� ����

    // ���� �ݻ籤
    GLfloat lightSpecular0[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // ���� 0.3f~0.5f���� ����

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

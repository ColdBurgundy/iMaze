#include "LightingManager.h"
#include <GL/glut.h>

void LightingManager::initLighting() {
    // 태양 광원 설정
    GLfloat lightPos0[] = { 0.0f, -1.0f, 1.0f, 1.0f }; // 높은 위치에 점 광원

    // 약한 주변광
    GLfloat lightAmbient0[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 기존 0.2f에서 줄임

    // 약한 난반사광
    GLfloat lightDiffuse0[] = { 0.4f, 0.4f, 0.4f, 1.0f }; // 기존 0.5f~0.6f에서 줄임

    // 약한 반사광
    GLfloat lightSpecular0[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // 기존 0.3f~0.5f에서 줄임

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void LightingManager::setLightPosition(float x, float y, float z) {
    GLfloat lightPos0[] = { x, y, z, 1.0f }; // 새로운 위치 설정
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}

void LightingManager::updateLightPosition() {
    // 동적 위치 업데이트를 추가하려면 이 함수에서 처리
}

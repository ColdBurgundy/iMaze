#include "LightingManager.h"
#include <GL/glut.h>

void LightingManager::initLighting() {
    // 태양 광원 설정
    GLfloat lightPos0[] = { 5.0f, -5.0f, 10.0f, 1.0f }; // 높고 멀리서 오는 빛
    GLfloat lightAmbient0[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // 주변광
    GLfloat lightDiffuse0[] = { 0.8f, 0.8f, 0.8f, 1.0f };   // 난반사광
    GLfloat lightSpecular0[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // 반사광



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

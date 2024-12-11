#include "Camera.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define PI 3.141592

Camera::Camera() : cameraX(0.0f), cameraY(0.0f), cameraZ(0.3f), angleX(0.0f), angleY(90.0f) {
    focusX = sin(angleY * (PI / 180)) * cos(angleX * (PI / 180));
    focusY = sin(angleY * (PI / 180)) * sin(angleX * (PI / 180));
    focusZ = cos(angleY * (PI / 180));
}

void Camera::updateCameraDirection(int x, int y) {
    static int lastX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    static int lastY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    int deltaX = x - lastX;
    int deltaY = y - lastY;

    angleX -= deltaX * 0.1f;
    angleY += deltaY * 0.1f;

    // 피치 제한 (카메라의 상하 각도 제한)
    if (angleY > 170.0f) angleY = 170.0f;
    if (angleY < 10.0f) angleY = 10.0f;

    // 카메라 방향 갱신
    focusX = sin(angleY * (PI / 180)) * cos(angleX * (PI / 180));
    focusY = sin(angleY * (PI / 180)) * sin(angleX * (PI / 180));
    focusZ = cos(angleY * (PI / 180));

    std::cout << "angleX: " << angleX << ", angleY: " << angleY << ", CamX: " << cameraX << ", CamY: " << cameraY << std::endl;

    glutWarpPointer(lastX, lastY);
    glutPostRedisplay(); // 화면 갱신 요청
}

void Camera::updatePosition(unsigned char key) {
    // "right" 벡터
    float rightX = cos((angleX + 90) * (PI / 180));
    float rightY = sin((angleX + 90) * (PI / 180));

    // "forward" 벡터
    float forwardX = cos(angleX * (PI / 180));
    float forwardY = sin(angleX * (PI / 180));

    const float step = 0.1f;

    // 플레이어 위치를 기준으로 이동
    float newX = cameraX;
    float newY = cameraY;
    if (key == 'w') {
        newX += forwardX * step;
        newY += forwardY * step;
    }
    else if (key == 's') {
        newX -= forwardX * step;
        newY -= forwardY * step;
    }
    else if (key == 'a') {
        newX += rightX * step;
        newY += rightY * step;
    }
    else if (key == 'd') {
        newX -= rightX * step;
        newY -= rightY * step;
    }
    else if (key == 'q') {
        cameraZ -= step;
    }
    else if (key == 'e') {
        cameraZ += step;
    }

    cameraX = newX;
    cameraY = newY;

    glutPostRedisplay();
}

void Camera::setPosition(float x, float y, float z) {
    cameraX = x;
    cameraY = y;
    cameraZ = z;
}

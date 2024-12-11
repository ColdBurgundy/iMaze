#ifndef CAMERA_H
#define CAMERA_H

class Camera {
private:
    float cameraX, cameraY, cameraZ;
    float focusX, focusY, focusZ;
    float angleX, angleY;
public:
    Camera();
    void updateCameraDirection(int deltaX, int deltaY);
    void updatePosition(unsigned char key);
    void setPosition(float x, float y, float z);
    void handleKeyboardInput(unsigned char key);
    void handleMouseMotion(int x, int y);
    float getPositionX() const { return cameraX; }
    float getPositionY() const { return cameraY; }
    float getPositionZ() const { return cameraZ; }
    float getFocusX() const { return focusX; }
    float getFocusY() const { return focusY; }
    float getFocusZ() const { return focusZ; }
    float getAngleX() const { return angleX; } // 카메라 X축 회전 각도 반환
};

#endif // CAMERA_H
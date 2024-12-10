#pragma once
#ifndef LIGHTING_MANAGER_H
#define LIGHTING_MANAGER_H

class LightingManager {
public:
    void initLighting();
    void setLightPosition(float x, float y, float z);
    void updateLightPosition(); // 필요한 경우 동적 업데이트 지원
};

#endif

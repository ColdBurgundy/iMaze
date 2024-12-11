#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <map>
#include <GL/glut.h>

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    GLuint loadTexture(const std::string& filePath);
    void cleanup();

private:
    std::map<std::string, GLuint> loadedTextures;
};

#endif // TEXTURE_MANAGER_H

#define _CRT_SECURE_NO_WARNINGS

#include "TextureManager.h"
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <windows.h> // BMP 파일 처리를 위한 헤더

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
    cleanup();
}

GLuint TextureManager::loadTexture(const std::string& filePath) {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    unsigned char* data;

    FILE* file;
    fopen_s(&file, filePath.c_str(), "rb");
    if (!file) {
        std::cout << "Failed to load BMP file: " << filePath << std::endl;
        return 0;
    }

    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    if (fileHeader.bfType != 0x4D42) { // 'BM' 체크
        std::cout << "Not a valid BMP file: " << filePath << std::endl;
        fclose(file);
        return 0;
    }

    int imageSize = infoHeader.biWidth * infoHeader.biHeight * 3;
    data = new unsigned char[imageSize];

    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    fread(data, 1, imageSize, file);
    fclose(file);

    // BGR 데이터를 RGB로 변환
    for (int i = 0; i < imageSize; i += 3) {
        std::swap(data[i], data[i + 2]);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, infoHeader.biWidth, infoHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    std::cout << "complete to read BMP: " <<filePath << "\n" << std::endl;

    delete[] data;
    return textureID;
}

void TextureManager::cleanup() {
    for (auto& texture : loadedTextures) {
        glDeleteTextures(1, &texture.second);
    }
    loadedTextures.clear();
}

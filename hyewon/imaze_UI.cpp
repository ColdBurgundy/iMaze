#include <iostream>
#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GL_CLAMP_TO_EDGE 정의
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

int width, height;

unsigned char* LoadMeshFromFile(const char* texFile)
{
    FILE* fp = NULL;
    if (fopen_s(&fp, texFile, "rb")) {
        printf("ERROR: Cannot open file %s.\n", texFile);
        return nullptr;
    }
    int channel;
    unsigned char* image = stbi_load_from_file(fp, &width, &height, &channel, 4);
    fclose(fp);

    if (image == nullptr) {
        printf("ERROR: Failed to load texture %s.\n", texFile);
    }
    else {
        printf("Texture loaded: %s (width: %d, height: %d, channels: %d)\n", texFile, width, height, channel);
    }
    return image;
}

void init()
{
    unsigned char* bitmap = LoadMeshFromFile("100.png");
    if (!bitmap) {
        printf("ERROR: Bitmap loading failed.\n");
        exit(EXIT_FAILURE);
    }

    GLuint texID;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // 텍스처 필터링 및 래핑 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    free(bitmap);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawBox()
{
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 1.0); glVertex3d(-0.5, -0.5, 0.0);
    glTexCoord2d(0.0, 0.0); glVertex3d(-0.5, 0.5, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex3d(0.5, 0.5, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex3d(0.5, -0.5, 0.0);
    glEnd();
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawBox();
    glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 30.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Texture Mapping");
    glutReshapeFunc(reshape);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

// �ؽ�ó ID
GLuint texture;

// BMP �̹����� �д� �Լ�
unsigned char* LoadBMP(const char* filename, int* width, int* height, int* channels) {
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0) {
        printf("������ �� �� �����ϴ�: %s\n", filename);
        return NULL;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);  // BMP ��� �б�

    // BMP �������� Ȯ��
    if (header[0] != 'B' || header[1] != 'M') {
        printf("BMP ������ �ƴմϴ�.\n");
        fclose(file);
        return NULL;
    }

    // �ʺ�� ���� �б�
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    // ��Ʈ ���� Ȯ�� (24��Ʈ �� 32��Ʈ ����)
    unsigned short bitDepth = *(unsigned short*)&header[28];
    if (bitDepth != 24 && bitDepth != 32) {
        printf("�������� �ʴ� ��Ʈ ����: %d��Ʈ\n", bitDepth);
        fclose(file);
        return NULL;
    }

    *channels = (bitDepth == 32) ? 4 : 3;  // ä�� �� ���� (32��Ʈ�� RGBA, 24��Ʈ�� RGB)

    // �̹��� ������ ũ�� ���
    int row_padded = ((*width) * (*channels) + 3) & (~3); // �� ���� 4����Ʈ �е� ó��
    int dataSize = row_padded * (*height);

    // �̹��� ������ �б�
    unsigned char* data = (unsigned char*)malloc(dataSize);
    if (!data) {
        printf("�޸� �Ҵ� ����\n");
        fclose(file);
        return NULL;
    }
    fread(data, sizeof(unsigned char), dataSize, file);
    fclose(file);

    // BGR(A)�� RGB(A)�� ��ȯ
    unsigned char* textureData = (unsigned char*)malloc((*width) * (*height) * (*channels));
    if (!textureData) {
        printf("�޸� �Ҵ� ����\n");
        free(data);
        return NULL;
    }

    for (int y = 0; y < *height; y++) {
        for (int x = 0; x < *width; x++) {
            int srcIndex = y * row_padded + x * (*channels);
            int dstIndex = ((y * (*width)) + x) * (*channels);

            textureData[dstIndex] = data[srcIndex + 2];     // Red
            textureData[dstIndex + 1] = data[srcIndex + 1]; // Green
            textureData[dstIndex + 2] = data[srcIndex];     // Blue
            if (*channels == 4) {
                textureData[dstIndex + 3] = data[srcIndex + 3]; // Alpha (32��Ʈ BMP)
            }
        }
    }

    free(data);
    return textureData;
}

// �ؽ�ó ����
void LoadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* image = LoadBMP(filename, &width, &height, &channels);

    if (!image) return;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // �ؽ�ó ���ε� (RGB �Ǵ� RGBA ó��)
    if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    free(image);
}

// OpenGL �ʱ�ȭ
void Initialize() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);  // ���� �׽�Ʈ Ȱ��ȭ
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    LoadTexture("C:\\Projects\\CG_Practice\\images\\apple_logo6.bmp"); // �ؽ�ó�� ����� BMP ���� ���

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);  // ���� ���� ����
    glMatrixMode(GL_MODELVIEW);
}

// ť�� �׸���
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(1.0, 0.0, 3.0,  // ī�޶� ��ġ (��¦ �����ʿ��� �ٶ�)
        0.0, 0.0, 0.0,  // ī�޶� ���� ��ġ
        0.0, 1.0, 0.0); // ī�޶� �� ����

    glBindTexture(GL_TEXTURE_2D, texture);

    float thickness = 0.1f;

    glBegin(GL_QUADS);

    // �ո� (�ؽ�ó ����)
    glTexCoord2f(0.02f, 0.02f); glVertex3f(-0.5f, -0.5f, thickness); // ��¦ �߾� �̵�
    glTexCoord2f(0.98f, 0.02f); glVertex3f(0.5f, -0.5f, thickness);
    glTexCoord2f(0.98f, 0.98f); glVertex3f(0.5f, 0.5f, thickness);
    glTexCoord2f(0.02f, 0.98f); glVertex3f(-0.5f, 0.5f, thickness);

    // ������ ���� �ܻ����� ó��
    glColor3f(0.5f, 0.5f, 0.5f);

    // �޸�
    glVertex3f(-0.5f, -0.5f, -thickness);
    glVertex3f(0.5f, -0.5f, -thickness);
    glVertex3f(0.5f, 0.5f, -thickness);
    glVertex3f(-0.5f, 0.5f, -thickness);

    // ����
    glVertex3f(-0.5f, 0.5f, thickness);
    glVertex3f(0.5f, 0.5f, thickness);
    glVertex3f(0.5f, 0.5f, -thickness);
    glVertex3f(-0.5f, 0.5f, -thickness);

    // �Ʒ���
    glVertex3f(-0.5f, -0.5f, thickness);
    glVertex3f(0.5f, -0.5f, thickness);
    glVertex3f(0.5f, -0.5f, -thickness);
    glVertex3f(-0.5f, -0.5f, -thickness);

    // ����
    glVertex3f(-0.5f, -0.5f, thickness);
    glVertex3f(-0.5f, 0.5f, thickness);
    glVertex3f(-0.5f, 0.5f, -thickness);
    glVertex3f(-0.5f, -0.5f, -thickness);

    // ������
    glVertex3f(0.5f, -0.5f, thickness);
    glVertex3f(0.5f, 0.5f, thickness);
    glVertex3f(0.5f, 0.5f, -thickness);
    glVertex3f(0.5f, -0.5f, -thickness);

    glEnd();

    glFlush();
}

// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(449, 610);
    glutCreateWindow("Texture Mapping on Thin Cube");

    Initialize();

    glutDisplayFunc(Display);
    glutMainLoop();

    return 0;
}

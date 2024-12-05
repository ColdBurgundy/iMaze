#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

// 텍스처 ID
GLuint texture;

// BMP 이미지를 읽는 함수
unsigned char* LoadBMP(const char* filename, int* width, int* height, int* channels) {
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return NULL;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);  // BMP 헤더 읽기

    // BMP 파일인지 확인
    if (header[0] != 'B' || header[1] != 'M') {
        printf("BMP 파일이 아닙니다.\n");
        fclose(file);
        return NULL;
    }

    // 너비와 높이 읽기
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    // 비트 깊이 확인 (24비트 및 32비트 지원)
    unsigned short bitDepth = *(unsigned short*)&header[28];
    if (bitDepth != 24 && bitDepth != 32) {
        printf("지원되지 않는 비트 깊이: %d비트\n", bitDepth);
        fclose(file);
        return NULL;
    }

    *channels = (bitDepth == 32) ? 4 : 3;  // 채널 수 결정 (32비트는 RGBA, 24비트는 RGB)

    // 이미지 데이터 크기 계산
    int row_padded = ((*width) * (*channels) + 3) & (~3); // 각 행의 4바이트 패딩 처리
    int dataSize = row_padded * (*height);

    // 이미지 데이터 읽기
    unsigned char* data = (unsigned char*)malloc(dataSize);
    if (!data) {
        printf("메모리 할당 실패\n");
        fclose(file);
        return NULL;
    }
    fread(data, sizeof(unsigned char), dataSize, file);
    fclose(file);

    // BGR(A)을 RGB(A)로 변환
    unsigned char* textureData = (unsigned char*)malloc((*width) * (*height) * (*channels));
    if (!textureData) {
        printf("메모리 할당 실패\n");
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
                textureData[dstIndex + 3] = data[srcIndex + 3]; // Alpha (32비트 BMP)
            }
        }
    }

    free(data);
    return textureData;
}

// 텍스처 설정
void LoadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* image = LoadBMP(filename, &width, &height, &channels);

    if (!image) return;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 텍스처 업로드 (RGB 또는 RGBA 처리)
    if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    free(image);
}

// OpenGL 초기화
void Initialize() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);  // 깊이 테스트 활성화
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    LoadTexture("C:\\Projects\\CG_Practice\\images\\apple_logo6.bmp"); // 텍스처로 사용할 BMP 파일 경로

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);  // 원근 투영 설정
    glMatrixMode(GL_MODELVIEW);
}

// 큐브 그리기
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(1.0, 0.0, 3.0,  // 카메라 위치 (살짝 오른쪽에서 바라봄)
        0.0, 0.0, 0.0,  // 카메라가 보는 위치
        0.0, 1.0, 0.0); // 카메라 업 벡터

    glBindTexture(GL_TEXTURE_2D, texture);

    float thickness = 0.1f;

    glBegin(GL_QUADS);

    // 앞면 (텍스처 매핑)
    glTexCoord2f(0.02f, 0.02f); glVertex3f(-0.5f, -0.5f, thickness); // 살짝 중앙 이동
    glTexCoord2f(0.98f, 0.02f); glVertex3f(0.5f, -0.5f, thickness);
    glTexCoord2f(0.98f, 0.98f); glVertex3f(0.5f, 0.5f, thickness);
    glTexCoord2f(0.02f, 0.98f); glVertex3f(-0.5f, 0.5f, thickness);

    // 나머지 면은 단색으로 처리
    glColor3f(0.5f, 0.5f, 0.5f);

    // 뒷면
    glVertex3f(-0.5f, -0.5f, -thickness);
    glVertex3f(0.5f, -0.5f, -thickness);
    glVertex3f(0.5f, 0.5f, -thickness);
    glVertex3f(-0.5f, 0.5f, -thickness);

    // 위쪽
    glVertex3f(-0.5f, 0.5f, thickness);
    glVertex3f(0.5f, 0.5f, thickness);
    glVertex3f(0.5f, 0.5f, -thickness);
    glVertex3f(-0.5f, 0.5f, -thickness);

    // 아래쪽
    glVertex3f(-0.5f, -0.5f, thickness);
    glVertex3f(0.5f, -0.5f, thickness);
    glVertex3f(0.5f, -0.5f, -thickness);
    glVertex3f(-0.5f, -0.5f, -thickness);

    // 왼쪽
    glVertex3f(-0.5f, -0.5f, thickness);
    glVertex3f(-0.5f, 0.5f, thickness);
    glVertex3f(-0.5f, 0.5f, -thickness);
    glVertex3f(-0.5f, -0.5f, -thickness);

    // 오른쪽
    glVertex3f(0.5f, -0.5f, thickness);
    glVertex3f(0.5f, 0.5f, thickness);
    glVertex3f(0.5f, 0.5f, -thickness);
    glVertex3f(0.5f, -0.5f, -thickness);

    glEnd();

    glFlush();
}

// 메인 함수
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

#pragma once
#ifndef TYPES_H
#define TYPES_H

// Vertex 구조체 정의
struct Vertex {
    float x, y, z;
};

// Face 구조체 정의
struct Face {
    int vertexIndices[3];
};

#endif // TYPES_H

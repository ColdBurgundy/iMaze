#pragma once
#ifndef TYPES_H
#define TYPES_H

// Vertex ����ü ����
struct Vertex {
    float x, y, z;
};

// Face ����ü ����
struct Face {
    int vertexIndices[3];
};

#endif // TYPES_H

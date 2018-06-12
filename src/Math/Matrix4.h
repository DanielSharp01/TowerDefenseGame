#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector_conversions.h"

typedef struct Matrix4
{
    float cells[16];
} Matrix4;

Matrix4 Matrix4_construct(float cells[16]);

Matrix4 Matrix4_mul(Matrix4 a, Matrix4 b);
Matrix4 Matrix4_mulS(Matrix4 a, float b);

Vector2 Matrix4_transformVec2(Matrix4 mat, Vector2 vec);
Vector3 Matrix4_transformVec3(Matrix4 mat, Vector3 vec);
Vector4 Matrix4_transformVec4(Matrix4 mat, Vector4 vec);

float Matrix4_det(Matrix4 a);
Matrix4 Matrix4_inverse(Matrix4 a);

Matrix4 Matrix4_createIdentity();

Matrix4 Matrix4_createScale2D(Vector2 vec);
Matrix4 Matrix4_createScale3D(Vector3 vec);
Matrix4 Matrix4_createScale2Ds(float x, float y);
Matrix4 Matrix4_createScale3Ds(float x, float y, float z);

Matrix4 Matrix4_createTranslation2D(Vector2 vec);
Matrix4 Matrix4_createTranslation3D(Vector3 vec);
Matrix4 Matrix4_createTranslation2Ds(float x, float y);
Matrix4 Matrix4_createTranslation3Ds(float x, float y, float z);

Matrix4 Matrix4_createRotationX(float rad);
Matrix4 Matrix4_createRotationY(float rad);
Matrix4 Matrix4_createRotationZ(float rad);

Matrix4 Matrix4_createOrthographicProjection(float x, float y, float w, float h, float near, float far);

#endif
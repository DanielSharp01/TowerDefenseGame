#ifndef VECTOR4_H
#define VECTOR4_H

typedef struct Vector4
{
    float x;
    float y;
    float z;
    float w;
} Vector4;

Vector4 Vector4_construct(float x, float y, float z, float w);

Vector4 Vector4_add(Vector4 a, Vector4 b);
Vector4 Vector4_sub(Vector4 a, Vector4 b);

Vector4 Vector4_invert(Vector4 a);

Vector4 Vector4_mul(Vector4 a, Vector4 b);
Vector4 Vector4_mulS(Vector4 a, float b);

Vector4 Vector4_div(Vector4 a, Vector4 b);
Vector4 Vector4_divS(Vector4 a, float b);

float Vector4_dot(Vector4 a, Vector4 b);

Vector4 Vector4_normalize(Vector4 a);

float Vector4_length(Vector4 a);
float Vector4_lengthSquared(Vector4 a);
#endif
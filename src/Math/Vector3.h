#ifndef VECTOR3_H
#define VECTOR3_H

typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;

Vector3 Vector3_construct(float x, float y, float z);

Vector3 Vector3_add(Vector3 a, Vector3 b);
Vector3 Vector3_sub(Vector3 a, Vector3 b);

Vector3 Vector3_invert(Vector3 a);

Vector3 Vector3_mul(Vector3 a, Vector3 b);
Vector3 Vector3_mulS(Vector3 a, float b);

Vector3 Vector3_div(Vector3 a, Vector3 b);
Vector3 Vector3_divS(Vector3 a, float b);

float Vector3_dot(Vector3 a, Vector3 b);
Vector3 Vector3_cross(Vector3 a, Vector3 b);

Vector3 Vector3_normalize(Vector3 a);

float Vector3_length(Vector3 a);
float Vector3_lengthSquared(Vector3 a);

#endif
#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct Vector2
{
    float x;
    float y;
} Vector2;

Vector2 Vector2_construct(float x, float y);

Vector2 Vector2_add(Vector2 a, Vector2 b);
Vector2 Vector2_sub(Vector2 a, Vector2 b);

Vector2 Vector2_invert(Vector2 a);

Vector2 Vector2_mul(Vector2 a, Vector2 b);
Vector2 Vector2_mulS(Vector2 a, float b);

Vector2 Vector2_div(Vector2 a, Vector2 b);
Vector2 Vector2_divS(Vector2 a, float b);

float Vector2_dot(Vector2 a, Vector2 b);

Vector2 Vector2_normalize(Vector2 a);

float Vector2_length(Vector2 a);
float Vector2_lengthSquared(Vector2 a);

#endif
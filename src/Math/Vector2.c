#include "Vector2.h"
#include "Vector_conversions.h"
#include <math.h>

Vector2 Vector2_construct(float x, float y)
{
    Vector2 ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

Vector2 Vector2_add(Vector2 a, Vector2 b)
{
    Vector2 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    return ret;
}

Vector2 Vector2_sub(Vector2 a, Vector2 b)
{
    Vector2 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    return ret;
}

Vector2 Vector2_invert(Vector2 a)
{
    Vector2 ret;
    ret.x = -a.x;
    ret.y = -a.y;
    return ret;
}

Vector2 Vector2_mul(Vector2 a, Vector2 b)
{
    Vector2 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    return ret;
}

Vector2 Vector2_mulS(Vector2 a, float b)
{
    Vector2 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    return ret;
}

Vector2 Vector2_div(Vector2 a, Vector2 b)
{
    Vector2 ret;
    ret.x = a.x / b.x;
    ret.y = a.y / b.y;
    return ret;
}

Vector2 Vector2_divS(Vector2 a, float b)
{
    Vector2 ret;
    ret.x = a.x / b;
    ret.y = a.y / b;
    return ret;
}

float Vector2_dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 Vector2_normalize(Vector2 a)
{
    Vector2 ret;
    float len = Vector2_length(a);
    ret.x = a.x / len;
    ret.y = a.y / len;
    return ret;
}

float Vector2_length(Vector2 a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

float Vector2_lengthSquared(Vector2 a)
{
    return a.x * a.x + a.y * a.y;
}

Vector2 Vector3_toVector2(Vector3 vec)
{
    Vector2 ret;
    ret.x = vec.x;
    ret.y = vec.y;
    return ret;
}

Vector2 Vector4_toVector2(Vector4 vec)
{
    Vector2 ret;
    ret.x = vec.x;
    ret.y = vec.y;
    return ret;
}
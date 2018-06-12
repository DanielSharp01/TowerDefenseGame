#include "Vector4.h"
#include "Vector_conversions.h"
#include <math.h>

Vector4 Vector4_construct(float x, float y, float z, float w)
{
    Vector4 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    ret.w = w;
    return ret;
}

Vector4 Vector4_add(Vector4 a, Vector4 b)
{
    Vector4 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;
    ret.w = a.w + b.w;
    return ret;
}

Vector4 Vector4_sub(Vector4 a, Vector4 b)
{
    Vector4 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;
    ret.w = a.w - b.w;
    return ret;
}

Vector4 Vector4_invert(Vector4 a)
{
    Vector4 ret;
    ret.x = -a.x;
    ret.y = -a.y;
    ret.z = -a.z;
    ret.w = -a.w;
    return ret;
}

Vector4 Vector4_mul(Vector4 a, Vector4 b)
{
    Vector4 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    ret.z = a.z * b.z;
    ret.w = a.w * b.w;
    return ret;
}

Vector4 Vector4_mulS(Vector4 a, float b)
{
    Vector4 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    ret.z = a.z * b;
    ret.w = a.w * b;
    return ret;
}

Vector4 Vector4_div(Vector4 a, Vector4 b)
{
    Vector4 ret;
    ret.x = a.x / b.x;
    ret.y = a.y / b.y;
    ret.z = a.z / b.z;
    ret.w = a.w / b.w;
    return ret;
}

Vector4 Vector4_divS(Vector4 a, float b)
{
    Vector4 ret;
    ret.x = a.x / b;
    ret.y = a.y / b;
    ret.z = a.z / b;
    return ret;
}

float Vector4_dot(Vector4 a, Vector4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector4 Vector4_normalize(Vector4 a)
{
    Vector4 ret;
    float len = Vector4_length(a);
    ret.x = a.x / len;
    ret.y = a.y / len;
    ret.z = a.z / len;
    ret.w = a.w / len;
    return ret;
}

float Vector4_length(Vector4 a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

float Vector4_lengthSquared(Vector4 a)
{
    return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

Vector4 Vector2_toVector4(Vector2 vec)
{
    Vector4 ret;
    ret.x = vec.x;
    ret.y = vec.y;
    ret.z = 0;
    ret.w = 1;
    return ret;
}

Vector4 Vector3_toVector4(Vector3 vec)
{
    Vector4 ret;
    ret.x = vec.x;
    ret.y = vec.y;
    ret.z = vec.z;
    ret.w = 1;
    return ret;
}
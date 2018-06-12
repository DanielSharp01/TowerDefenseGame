#include "Vector3.h"
#include "Vector_conversions.h"
#include <math.h>

Vector3 Vector3_construct(float x, float y, float z)
{
    Vector3 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    return ret;
}

Vector3 Vector3_add(Vector3 a, Vector3 b)
{
    Vector3 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;
    return ret;
}

Vector3 Vector3_sub(Vector3 a, Vector3 b)
{
    Vector3 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;
    return ret;
}

Vector3 Vector3_invert(Vector3 a)
{
    Vector3 ret;
    ret.x = -a.x;
    ret.y = -a.y;
    ret.z = -a.z;
    return ret;
}

Vector3 Vector3_mul(Vector3 a, Vector3 b)
{
    Vector3 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    ret.z = a.z * b.z;
    return ret;
}

Vector3 Vector3_mulS(Vector3 a, float b)
{
    Vector3 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    ret.z = a.z * b;
    return ret;
}

Vector3 Vector3_div(Vector3 a, Vector3 b)
{
    Vector3 ret;
    ret.x = a.x / b.x;
    ret.y = a.y / b.y;
    ret.z = a.z / b.z;
    return ret;
}

Vector3 Vector3_divS(Vector3 a, float b)
{
    Vector3 ret;
    ret.x = a.x / b;
    ret.y = a.y / b;
    ret.z = a.z / b;
    return ret;
}

float Vector3_dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3_cross(Vector3 a, Vector3 b)
{
    Vector3 ret;
    ret.x = a.y * b.z - a.z * b.y;
    ret.y = a.z * b.x - a.x * b.z;
    ret.z = a.x * b.y - a.y * b.x;
    return ret;
}

Vector3 Vector3_normalize(Vector3 a)
{
    Vector3 ret;
    float len = Vector3_length(a);
    ret.x = a.x / len;
    ret.y = a.y / len;
    ret.z = a.z / len;
    return ret;
}

float Vector3_length(Vector3 a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float Vector3_lengthSquared(Vector3 a)
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

Vector3 Vector2_toVector3(Vector2 vec)
{
    Vector3 ret;
    ret.x = vec.x;
    ret.y = vec.y;
    ret.z = 0;
    return ret;
}

Vector3 Vector4_toVector3(Vector4 vec)
{
    Vector3 ret;
    ret.x = vec.x;
    ret.y = vec.y;
    ret.z = vec.z;
    return ret;
}
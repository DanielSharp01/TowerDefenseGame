#include "Matrix4.h"
#include <math.h>
#include <string.h>

Matrix4 Matrix4_construct(float cells[16])
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = cells[i];
    }
    return ret;
}

Matrix4 Matrix4_mul(Matrix4 a, Matrix4 b)
{
    Matrix4 ret;
    int r, c, d;

    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            float sum = 0;
            for (d = 0; d < 4; d++)
            {
                sum += a.cells[d * 4 + r] * b.cells[c * 4 + d];
            }
            ret.cells[c * 4 + r] = sum;
        }
    }

    return ret;
}

Matrix4 Matrix4_mulS(Matrix4 a, float b)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = a.cells[i] * b;
    }

    return ret;
}

Vector2 Matrix4_transformVec2(Matrix4 mat, Vector2 vec)
{
    Vector2 ret;
    ret.x = mat.cells[0] * vec.x + mat.cells[4] * vec.y + mat.cells[12];
    ret.y = mat.cells[1] * vec.x + mat.cells[5] * vec.y + mat.cells[13];
    return ret;
}

Vector3 Matrix4_transformVec3(Matrix4 mat, Vector3 vec)
{
    Vector3 ret;
    ret.x = mat.cells[0] * vec.x + mat.cells[4] * vec.y + mat.cells[8] * vec.z + mat.cells[12];
    ret.y = mat.cells[1] * vec.x + mat.cells[5] * vec.y + mat.cells[9] * vec.z + mat.cells[13];
    ret.z = mat.cells[2] * vec.x + mat.cells[6] * vec.y + mat.cells[10] * vec.z + mat.cells[14];
    return ret;
}

Vector4 Matrix4_transformVec4(Matrix4 mat, Vector4 vec)
{
    Vector4 ret;
    ret.x = mat.cells[0] * vec.x + mat.cells[4] * vec.y + mat.cells[8] * vec.z + mat.cells[12] * vec.w;
    ret.y = mat.cells[1] * vec.x + mat.cells[5] * vec.y + mat.cells[9] * vec.z + mat.cells[13] * vec.w;
    ret.z = mat.cells[2] * vec.x + mat.cells[6] * vec.y + mat.cells[10] * vec.z + mat.cells[14] * vec.w;
    ret.w = mat.cells[3] * vec.x + mat.cells[7] * vec.y + mat.cells[11] * vec.z + mat.cells[15] * vec.w;
    return ret;
}

float det2x2(float a, float b, float c, float d)
{
    return a*d - b*c;
}

float det3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
    return a * det2x2(e, f, h, i) - d * det2x2(b, c, h, i) + g * det2x2(b, c, e, f);
}

float Matrix4_det(Matrix4 a)
{
    return a.cells[0] * det3x3(a.cells[5], a.cells[6], a.cells[7], a.cells[9], a.cells[10], a.cells[11], a.cells[13], a.cells[14], a.cells[15])
         - a.cells[4] * det3x3(a.cells[1], a.cells[2], a.cells[3], a.cells[9], a.cells[10], a.cells[11], a.cells[13], a.cells[14], a.cells[15])
         + a.cells[8] * det3x3(a.cells[1], a.cells[2], a.cells[3], a.cells[5], a.cells[6], a.cells[7], a.cells[13], a.cells[14], a.cells[15])
         - a.cells[12] * det3x3(a.cells[1], a.cells[2], a.cells[3], a.cells[5], a.cells[6], a.cells[7], a.cells[9], a.cells[10], a.cells[11]);
}

Matrix4 Matrix4_inverse(Matrix4 a)
{
    Matrix4 ret;
    ret.cells[0] = det3x3(a.cells[5], a.cells[6], a.cells[7], a.cells[9], a.cells[10], a.cells[11], a.cells[13], a.cells[14], a.cells[15]);
    ret.cells[1] = -det3x3(a.cells[1], a.cells[2], a.cells[3], a.cells[9], a.cells[10], a.cells[11], a.cells[13], a.cells[14], a.cells[15]);
    ret.cells[2] = det3x3(a.cells[1], a.cells[2], a.cells[3], a.cells[5], a.cells[6], a.cells[7], a.cells[13], a.cells[14], a.cells[15]);
    ret.cells[3] = -det3x3(a.cells[1], a.cells[2], a.cells[3], a.cells[5], a.cells[6], a.cells[7], a.cells[9], a.cells[10], a.cells[11]);

    ret.cells[4] = -det3x3(a.cells[4], a.cells[6], a.cells[7], a.cells[8], a.cells[10], a.cells[11], a.cells[12], a.cells[14], a.cells[15]);
    ret.cells[5] = det3x3(a.cells[0], a.cells[2], a.cells[3], a.cells[8], a.cells[10], a.cells[11], a.cells[12], a.cells[14], a.cells[15]);
    ret.cells[6] = -det3x3(a.cells[0], a.cells[2], a.cells[3], a.cells[4], a.cells[6], a.cells[7], a.cells[12], a.cells[14], a.cells[15]);
    ret.cells[7] = det3x3(a.cells[0], a.cells[2], a.cells[3], a.cells[4], a.cells[6], a.cells[7], a.cells[8], a.cells[10], a.cells[11]);

    ret.cells[8] = det3x3(a.cells[4], a.cells[5], a.cells[7], a.cells[8], a.cells[9], a.cells[11], a.cells[12], a.cells[13], a.cells[15]);
    ret.cells[9] = -det3x3(a.cells[0], a.cells[1], a.cells[3], a.cells[8], a.cells[9], a.cells[11], a.cells[12], a.cells[13], a.cells[15]);
    ret.cells[10] = det3x3(a.cells[0], a.cells[1], a.cells[3], a.cells[4], a.cells[5], a.cells[7], a.cells[12], a.cells[13], a.cells[15]);
    ret.cells[11] = -det3x3(a.cells[0], a.cells[1], a.cells[3], a.cells[4], a.cells[5], a.cells[7], a.cells[8], a.cells[9], a.cells[11]);

    ret.cells[12] = -det3x3(a.cells[4], a.cells[5], a.cells[6], a.cells[8], a.cells[9], a.cells[10], a.cells[12], a.cells[13], a.cells[14]);
    ret.cells[13] = det3x3(a.cells[0], a.cells[1], a.cells[2], a.cells[8], a.cells[9], a.cells[10], a.cells[12], a.cells[13], a.cells[14]);
    ret.cells[14] = -det3x3(a.cells[0], a.cells[1], a.cells[2], a.cells[4], a.cells[5], a.cells[6], a.cells[12], a.cells[13], a.cells[14]);
    ret.cells[15] = det3x3(a.cells[0], a.cells[1], a.cells[2], a.cells[4], a.cells[5], a.cells[6], a.cells[8], a.cells[9], a.cells[10]);

    float det = a.cells[0] * ret.cells[0] + a.cells[4] * ret.cells[1] + a.cells[8] * ret.cells[2] + a.cells[12] * ret.cells[3];
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = ret.cells[i] / det;
    }

    return ret;
}

Matrix4 Matrix4_createIdentity()
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = 1;
    ret.cells[5] = 1;
    ret.cells[10] = 1;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createScale2D(Vector2 vec)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = vec.x;
    ret.cells[5] = vec.y;
    ret.cells[10] = 1;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createScale3D(Vector3 vec)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = vec.x;
    ret.cells[5] = vec.y;
    ret.cells[10] = vec.z;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createScale2Ds(float x, float y)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = x;
    ret.cells[5] = y;
    ret.cells[10] = 1;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createScale3Ds(float x, float y, float z)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = x;
    ret.cells[5] = y;
    ret.cells[10] = z;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createTranslation2D(Vector2 vec)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = 1;
    ret.cells[5] = 1;
    ret.cells[10] = 1;
    ret.cells[12] = vec.x;
    ret.cells[13] = vec.y;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createTranslation3D(Vector3 vec)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = 1;
    ret.cells[5] = 1;
    ret.cells[10] = 1;
    ret.cells[12] = vec.x;
    ret.cells[13] = vec.y;
    ret.cells[14] = vec.z;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createTranslation2Ds(float x, float y)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = 1;
    ret.cells[5] = 1;
    ret.cells[10] = 1;
    ret.cells[12] = x;
    ret.cells[13] = y;
    ret.cells[14] = 0;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createTranslation3Ds(float x, float y, float z)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }
    ret.cells[0] = 1;
    ret.cells[5] = 1;
    ret.cells[10] = 1;
    ret.cells[12] = x;
    ret.cells[13] = y;
    ret.cells[14] = z;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createRotationX(float rad)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }

    float c = cos(rad);
    float s = sin(rad);

    ret.cells[0] = 1;
    ret.cells[5] = c;
    ret.cells[6] = s;
    ret.cells[9] = -s;
    ret.cells[10] = c;
    ret.cells[15] = 1;
    return ret;
}
Matrix4 Matrix4_createRotationY(float rad)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }

    float c = cos(rad);
    float s = sin(rad);

    ret.cells[0] = c;
    ret.cells[2] = -s;
    ret.cells[8] = s;
    ret.cells[10] = c;
    ret.cells[10] = 1;
    ret.cells[15] = 1;
    return ret;
}

Matrix4 Matrix4_createRotationZ(float rad)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }

    float c = cos(rad);
    float s = sin(rad);

    ret.cells[0] = c;
    ret.cells[1] = s;
    ret.cells[4] = -s;
    ret.cells[5] = c;
    ret.cells[10] = 1;
    ret.cells[15] = 1;

    return ret;
}

Matrix4 Matrix4_createOrthographicProjection(float left, float top, float right, float bottom, float near, float far)
{
    Matrix4 ret;
    int i;
    for (i = 0; i < 16; i++)
    {
        ret.cells[i] = 0;
    }

    ret.cells[0] = 2.0/(right - left);
    ret.cells[5] = 2.0/(top - bottom);
    ret.cells[10] = 2.0/(near - far);
    ret.cells[12] = -(right + left)/(right - left);
    ret.cells[13] = -(top + bottom)/(top - bottom);
    ret.cells[14] = -(far + near)/(far - near);
    ret.cells[15] = 1;

    return ret;
}
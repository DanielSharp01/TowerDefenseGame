#include "VectorFuncs.h"

float pointDirection(Vector2 from, Vector2 to)
{
    Vector2 delta = Vector2_sub(to, from);
    return atan2(delta.y, delta.x);
}

float pointDistance(Vector2 from, Vector2 to)
{
    Vector2 delta = Vector2_sub(to, from);
    return Vector2_length(delta);
}

Vector2 directionSpeed(float rotation, float speed)
{
    return Vector2_construct(cos(rotation) * speed, sin(rotation) * speed);
}

float projectVec(Vector2 vec, Vector2 axis)
{
    return Vector2_dot(vec, Vector2_normalize(axis));
}

Vector2 bezierCurve(Vector2 p, Vector2 q, Vector2 m, float t)
{
    return Vector2_add(Vector2_add(Vector2_mulS(p, (1-t)*(1-t)), Vector2_mulS(m, 2*(1-t)*t)), Vector2_mulS(q, t*t));
}

Vector2 bezierCurveDerivative(Vector2 p, Vector2 q, Vector2 m, float t)
{
    return Vector2_add(Vector2_add(Vector2_mulS(p, 2*(t-1)), Vector2_mulS(m, 2*(1-2*t))), Vector2_mulS(q, 2*t));
}
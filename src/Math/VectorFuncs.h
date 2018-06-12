#ifndef VECTORFUNCS_H
#define VECTORFUNCS_H

#include "Vector2.h"
#include <stdbool.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

float pointDirection(Vector2 from, Vector2 to);
float pointDistance(Vector2 from, Vector2 to);
Vector2 directionSpeed(float rotation, float speed);

Vector2 bezierCurve(Vector2 p, Vector2 q, Vector2 c, float t);
Vector2 bezierCurveDerivative(Vector2 p, Vector2 q, Vector2 c, float t);

float projectVec(Vector2 vec, Vector2 axis);

#endif
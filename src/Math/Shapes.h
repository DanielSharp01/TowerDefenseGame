#ifndef SHAPES_H
#define SHAPES_H

#include "Matrix4.h"
#include "Vector2.h"
#include <stdbool.h>

typedef struct Rectangle
{
    Vector2 TL;
    Vector2 TR;
    Vector2 BL;
    Vector2 BR;
    Vector2 center;
} Rectangle;

typedef struct Circle
{
    Vector2 center;
    float radius;
} Circle;

Rectangle Rectangle_construct(float x, float y, float w, float h, float angle);
Rectangle Rectangle_constructVec(Vector2 center, Vector2 width, Vector2 height);
Rectangle Rectangle_constructCorners(Vector2 TL, Vector2 TR, Vector2 BL, Vector2 BR);

float Rectangle_minProjection(Rectangle rect, Vector2 axis);
float Rectangle_maxProjection(Rectangle rect, Vector2 axis);

bool Rectangle_intersectsRect(Rectangle a, Rectangle b);
bool Rectangle_containsRect(Rectangle a, Rectangle b);
bool Rectangle_intersectsCircle(Rectangle a, Circle b);
bool Rectangle_containsCircle(Rectangle a, Circle b);
bool Rectangle_containsPoint(Rectangle rect, Vector2 point);

Rectangle Rectangle_transform(Rectangle rect, Matrix4 mat);

Circle Circle_construct(Vector2 center, float radius);

float Circle_minProjection(Circle circle, Vector2 axis);
float Circle_maxProjection(Circle circle, Vector2 axis);

bool Circle_intersectsCircle(Circle a, Circle b);
bool Circle_containsCircle(Circle a, Circle b);
bool Circle_intersectsRect(Circle a, Rectangle b);
bool Circle_containsRect(Circle a, Rectangle b);
bool Circle_containsPoint(Circle circle, Vector2 point);

#endif
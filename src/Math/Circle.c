#include "Shapes.h"
#include "Interval.h"
#include "VectorFuncs.h"

Circle Circle_construct(Vector2 center, float radius)
{
    Circle ret;
    ret.center = center;
    ret.radius = radius;
    return ret;
}

float Circle_minProjection(Circle circle, Vector2 axis)
{
    return projectVec(circle.center, axis) - circle.radius;
}

float Circle_maxProjection(Circle circle, Vector2 axis)
{
    return projectVec(circle.center, axis) + circle.radius;
}

bool Circle_intersectsCircle(Circle a, Circle b)
{
    return Vector2_length(Vector2_sub(b.center, a.center)) <= a.radius + b.radius;
}

bool Circle_containsCircle(Circle a, Circle b)
{
    return Vector2_length(Vector2_sub(b.center, a.center)) <= a.radius - b.radius;
}

bool Circle_intersectsRect(Circle a, Rectangle b)
{
    return Rectangle_intersectsCircle(b, a);
}

bool Circle_containsRect(Circle a, Rectangle b)
{
    float mina, maxa, minb, maxb;
    Vector2 axis;

    //B - width
    axis = Vector2_sub(b.TR, b.TL);
    mina = Circle_minProjection(a, axis);
    maxa = Circle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //B - height
    axis = Vector2_sub(b.BL, b.TL);
    mina = Circle_minProjection(a, axis);
    maxa = Circle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //A Center - B TL
    axis = Vector2_sub(b.TL, a.center);
    mina = Circle_minProjection(a, axis);
    maxa = Circle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //A Center - B BL
    axis = Vector2_sub(b.BL, a.center);
    mina = Circle_minProjection(a, axis);
    maxa = Circle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //A Center - B TR
    axis = Vector2_sub(b.TR, a.center);
    mina = Circle_minProjection(a, axis);
    maxa = Circle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //A Center - B BR
    axis = Vector2_sub(b.BR, a.center);
    mina = Circle_minProjection(a, axis);
    maxa = Circle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    return true;
}

bool Circle_containsPoint(Circle circle, Vector2 point)
{
    return Vector2_length(Vector2_sub(circle.center, point)) <= circle.radius;
}
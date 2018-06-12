#include "Shapes.h"
#include "Interval.h"
#include "VectorFuncs.h"

Rectangle Rectangle_construct(float x, float y, float w, float h, float angle)
{
    Vector2 width = Vector2_construct(w * cos(angle), w * sin(angle));
    Vector2 height = Vector2_construct(h * cos(angle + PI / 2), h * sin(angle + PI / 2));
    Rectangle ret;
    ret.TL.x = x;
    ret.TL.y = y;
    ret.TR.x = x + width.x;
    ret.TR.y = y + width.y;
    ret.BL.x = x + height.x;
    ret.BL.y = y + height.y;
    ret.BR.x = x + width.x + height.x;
    ret.BR.y = y + width.y + height.y;
    ret.center.x = x + width.x/2+height.x/2;
    ret.center.y = y + width.y/2+height.y/2;
    
    return ret;
}

Rectangle Rectangle_constructVec(Vector2 center, Vector2 width, Vector2 height)
{
    Rectangle ret;
    ret.center = center;
    ret.TL.x = center.x - width.x / 2 - height.x / 2;
    ret.TL.y = center.y - width.y / 2 - height.y / 2;
    ret.TR.x = center.x + width.x / 2 - height.x / 2;
    ret.TR.y = center.y + width.y / 2 - height.y / 2;
    ret.BL.x = center.x - width.x / 2 + height.x / 2;
    ret.BL.y = center.y - width.y / 2 + height.y / 2;
    ret.BR.x = center.x + width.x / 2 + height.x / 2;
    ret.BR.y = center.y + width.y / 2 + height.y / 2;

    return ret;
}

Rectangle Rectangle_constructCorners(Vector2 TL, Vector2 TR, Vector2 BL, Vector2 BR)
{
    Rectangle ret;
    ret.TL = TL;
    ret.TR = TR;
    ret.BL = BL;
    ret.BR = BR;
    ret.center = Vector2_divS(Vector2_add(ret.TL, ret.BR), 2);

    return ret;
}

float Rectangle_minProjection(Rectangle rect, Vector2 axis)
{
    float ret = projectVec(rect.TL, axis);
    float try = projectVec(rect.TR, axis);
    if (try < ret) ret = try;
    try = projectVec(rect.BL, axis);
    if (try < ret) ret = try;
    try = projectVec(rect.BR, axis);
    if (try < ret) ret = try;

    return ret;
}

float Rectangle_maxProjection(Rectangle rect, Vector2 axis)
{
    float ret = projectVec(rect.TL, axis);
    float try = projectVec(rect.TR, axis);
    if (try > ret) ret = try;
    try = projectVec(rect.BL, axis);
    if (try > ret) ret = try;
    try = projectVec(rect.BR, axis);
    if (try > ret) ret = try;

    return ret;
}

bool Rectangle_intersectsRect(Rectangle a, Rectangle b)
{
    float mina, maxa, minb, maxb;
    Vector2 axis;

    //A - width
    axis = Vector2_sub(a.TR, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //A - height
    axis = Vector2_sub(a.BL, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //B - width
    axis = Vector2_sub(b.TR, b.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //B - height
    axis = Vector2_sub(b.BL, b.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    return true;
}

bool Rectangle_containsRect(Rectangle a, Rectangle b)
{
    float mina, maxa, minb, maxb;
    Vector2 axis;

    //A - width
    axis = Vector2_sub(a.TR, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //A - height
    axis = Vector2_sub(a.BL, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //B - width
    axis = Vector2_sub(b.TR, b.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //B - height
    axis = Vector2_sub(b.BL, b.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Rectangle_minProjection(b, axis);
    maxb = Rectangle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    return true;
}

bool Rectangle_intersectsCircle(Rectangle a, Circle b)
{
    float mina, maxa, minb, maxb;
    Vector2 axis;

    //A - width
    axis = Vector2_sub(a.TR, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //A - height
    axis = Vector2_sub(a.BL, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //B Center - A TL
    axis = Vector2_sub(a.TL, b.center);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //B Center - A BL
    axis = Vector2_sub(a.BL, b.center);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //B Center - A TR
    axis = Vector2_sub(a.TR, b.center);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    //B Center - A BR
    axis = Vector2_sub(a.BR, b.center);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_intersects(mina, maxa, minb, maxb)) return false;

    return true;
}

bool Rectangle_containsCircle(Rectangle a, Circle b)
{
    float mina, maxa, minb, maxb;
    Vector2 axis;

    //A - width
    axis = Vector2_sub(a.TR, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    //A - height
    axis = Vector2_sub(a.BL, a.TL);
    mina = Rectangle_minProjection(a, axis);
    maxa = Rectangle_maxProjection(a, axis);

    minb = Circle_minProjection(b, axis);
    maxb = Circle_maxProjection(b, axis);

    if (!Interval_contains(mina, maxa, minb, maxb)) return false;

    return true;
}

bool Rectangle_containsPoint(Rectangle rect, Vector2 point)
{
    float min, max, pt;
    Vector2 axis;
    //A - width
    axis = Vector2_sub(rect.TR, rect.TL);
    min = Rectangle_minProjection(rect, axis);
    max = Rectangle_maxProjection(rect, axis);
    pt = projectVec(point, axis);

    if (!Interval_contains(min, max, pt, pt)) return false;

    //A - height
    axis = Vector2_sub(rect.BL, rect.TL);
    min = Rectangle_minProjection(rect, axis);
    max = Rectangle_maxProjection(rect, axis);
    pt = projectVec(point, axis);

    if (!Interval_contains(min, max, pt, pt)) return false;

    return true;
}

Rectangle Rectangle_transform(Rectangle rect, Matrix4 mat)
{
    Rectangle ret;
    ret.TL = Matrix4_transformVec2(mat, rect.TL);
    ret.TR = Matrix4_transformVec2(mat, rect.TR);
    ret.BL = Matrix4_transformVec2(mat, rect.BL);
    ret.BR = Matrix4_transformVec2(mat, rect.BR);
    return ret;
}
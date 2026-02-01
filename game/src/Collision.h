#pragma once
#include "raymathext.h"
// All formulas for circle, capsule, AABB, and infinite plane collision!

// Outputs nearest two points along line top1_bot1 and line top2_bot2
RMAPI void NearestLinePoints(
    Vector2 top1, Vector2 bot1,
    Vector2 top2, Vector2 bot2,
    Vector2& near1, Vector2& near2);

// mtv resolves 1 from 2
RMAPI bool CircleCircle(
    Vector2 pos1, float rad1,
    Vector2 pos2, float rad2,
    Vector2* mtv = nullptr)
{
    float rad = rad1 + rad2;
    bool collision = Vector2DistanceSqr(pos1, pos2) <= rad * rad;
    if (collision && mtv != nullptr)
    {
        *mtv = Vector2Normalize(pos1 - pos2) * (rad - Vector2Distance(pos1, pos2));
    }
    return collision;
}

// mtv resolves 1 from 2
RMAPI bool CircleCapsule(
    Vector2 pos1, float rad1,
    Vector2 pos2, Vector2 dir2, float rad2, float hh2,
    Vector2* mtv = nullptr)
{
    Vector2 top2 = pos2 + dir2 * hh2;
    Vector2 bot2 = pos2 - dir2 * hh2;
    Vector2 proj = Vector2ProjectPointLine(top2, bot2, pos1);
    return CircleCircle(pos1, rad1, proj, rad2, mtv);
}

// mtv resolves 1 from 2
RMAPI bool CapsuleCapsule(
    Vector2 pos1, Vector2 dir1, float rad1, float hh1,
    Vector2 pos2, Vector2 dir2, float rad2, float hh2,
    Vector2* mtv = nullptr)
{
    Vector2 top1 = pos1 + dir1 * hh1;
    Vector2 bot1 = pos1 - dir1 * hh1;
    Vector2 top2 = pos2 + dir2 * hh2;
    Vector2 bot2 = pos2 - dir2 * hh2;
    Vector2 near1, near2;
    NearestLinePoints(top1, bot1, top2, bot2, near1, near2);
    return CircleCircle(near1, rad1, near2, rad2, mtv);
}

// mtv resolves circle from plane
RMAPI bool CirclePlane(
    Vector2 circle, float radius,
    Vector2 plane, Vector2 normal,
    Vector2* mtv = nullptr)
{
    float distance = Vector2DotProduct(circle - plane, normal);
    bool collision = distance <= radius;
    if (collision && mtv != nullptr)
        *mtv = normal * (radius - distance);
    return collision;
}

// mtv resolves capsule from plane
RMAPI bool CapsulePlane(
    Vector2 cap, Vector2 dir, float radius, float hh,
    Vector2 plane, Vector2 normal,
    Vector2* mtv = nullptr)
{
    Vector2 top = cap + dir * hh;
    Vector2 bot = cap - dir * hh;
    float d1 = Vector2DotProduct(top - plane, normal);
    float d2 = Vector2DotProduct(bot - plane, normal);
    float d = d1 < d2 ? d1 : d2;
    bool collision = d <= radius;
    if (collision && mtv != nullptr)
        *mtv = normal * (radius - d);
    return collision;
}

// mtv resolves rectangle from plane
RMAPI bool RectanglePlane(
    Vector2 rect, Vector2 extents,
    Vector2 plane, Vector2 normal,
    Vector2* mtv = nullptr)
{
    Vector2 near = Vector2Clamp(rect - normal * Vector2LengthSqr(extents), rect - extents, rect + extents);
    return CirclePlane(near, 1.0f, plane, normal, mtv);
}

// mtv resolves circle from rectangle
RMAPI bool CircleRectangle(
    Vector2 circle, float radius,
    Vector2 rect, Vector2 extents,
    Vector2* mtv = nullptr)
{
    Vector2 near = Vector2Clamp(circle, rect - extents, rect + extents);
    return CircleCircle(circle, radius, near, 1.0f, mtv);
}

// mtv resolves capsule from rectangle
RMAPI bool CapsuleRectangle(
    Vector2 cap, Vector2 dir, float radius, float hh,
    Vector2 rect, Vector2 extents,
    Vector2* mtv = nullptr)
{
    Vector2 top = cap + dir * hh;
    Vector2 bot = cap - dir * hh;
    Vector2 min = rect - extents;
    Vector2 max = rect + extents;

    Vector2 top_left = rect + Vector2{ -extents.x, extents.y };
    Vector2 top_right = rect + Vector2{ extents.x, extents.y };
    Vector2 bot_left = rect + Vector2{ -extents.x, -extents.y };
    Vector2 bot_right = rect + Vector2{ extents.x, -extents.y };

    Vector2 fProjections[4]
    {
        Vector2ProjectPointLine(top, bot, top_left),
        Vector2ProjectPointLine(top, bot, top_right),
        Vector2ProjectPointLine(top, bot, bot_left),
        Vector2ProjectPointLine(top, bot, bot_right)
    };

    Vector2 clamps[4]
    {
        Vector2Clamp(fProjections[0], min, max),
        Vector2Clamp(fProjections[1], min, max),
        Vector2Clamp(fProjections[2], min, max),
        Vector2Clamp(fProjections[3], min, max)
    };

    int index = 0;
    float d0 = Vector2LengthSqr(fProjections[0] - clamps[0]);
    for (int i = 1; i < 4; i++)
    {
        float d = Vector2LengthSqr(fProjections[i] - clamps[i]);
        if (d < d0)
        {
            d0 = d;
            index = i;
        }
    }

    Vector2 proj = fProjections[index];
    Vector2 point = clamps[index];
    return CircleCircle(proj, radius, point, 1.0f, mtv);
}

// mtv resolves 1 from 2
RMAPI bool RectangleRectangle(
    Vector2 rect1, Vector2 extents1,
    Vector2 rect2, Vector2 extents2,
    Vector2* mtv = nullptr)
{
    float xMin1 = rect1.x - extents1.x;
    float xMax1 = rect1.x + extents1.x;
    float yMin1 = rect1.y - extents1.y;
    float yMax1 = rect1.y + extents1.y;

    float xMin2 = rect2.x - extents2.x;
    float xMax2 = rect2.x + extents2.x;
    float yMin2 = rect2.y - extents2.y;
    float yMax2 = rect2.y + extents2.y;

    bool collision =
        Overlaps(xMin1, xMax1, xMin2, xMax2) &&
        Overlaps(yMin1, yMax1, yMin2, yMax2);
    if (collision && mtv != nullptr)
    {
        // Overlapping area (rectangle)
        float xMin = fmaxf(xMin1, xMin2);
        float xMax = fminf(xMax1, xMax2);
        float yMin = fmaxf(yMin1, yMin2);
        float yMax = fminf(yMax1, yMax2);

        // Resolve along the shortest axis (x vs y)
        float x = xMax - xMin;
        float y = yMax - yMin;
        if (x < y)
        {
            *mtv = { x, 0.0f };
            if (rect2.x > xMin) mtv->x *= -1.0f;
        }
        else if (y < x)
        {
            *mtv = { 0.0f, y };
            if (rect2.y > yMin) mtv->y *= -1.0f;
        }
        else
        {
            *mtv = { x, y };
            if (rect2.x > xMin) mtv->x *= -1.0f;
            if (rect2.y > yMin) mtv->y *= -1.0f;
        }
    }
    return collision;
}

RMAPI void NearestLinePoints(
    Vector2 top1, Vector2 bot1,
    Vector2 top2, Vector2 bot2,
    Vector2& near1, Vector2& near2)
{
    Vector2 A = Vector2ProjectPointLine(top2, bot2, top1);
    Vector2 B = Vector2ProjectPointLine(top2, bot2, bot1);
    Vector2 C = Vector2ProjectPointLine(top1, bot1, A);
    Vector2 D = Vector2ProjectPointLine(top1, bot1, B);
    near1 = Vector2LengthSqr(A - C) < Vector2LengthSqr(B - D) ? C : D;

    Vector2 E = Vector2ProjectPointLine(top1, bot1, top2);
    Vector2 F = Vector2ProjectPointLine(top1, bot1, bot2);
    Vector2 G = Vector2ProjectPointLine(top2, bot2, E);
    Vector2 H = Vector2ProjectPointLine(top2, bot2, F);
    near2 = Vector2LengthSqr(E - G) < Vector2LengthSqr(F - H) ? G : H;
}

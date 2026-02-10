#pragma once
// All formulas for sphere, capsule, AABB, and infinite plane collision!

// Outputs nearest two points along line top1_bot1 and line top2_bot2
RMAPI void NearestLinePoints(
    Vector3 top1, Vector3 bot1,
    Vector3 top2, Vector3 bot2,
    Vector3& near1, Vector3& near2);

// mtv resolves 1 from 2
RMAPI bool SphereSphere(
    Vector3 pos1, float rad1,
    Vector3 pos2, float rad2,
    Vector3* mtv = nullptr)
{
    float rad = rad1 + rad2;
    bool collision = Vector3DistanceSqr(pos1, pos2) <= rad * rad;

    if (collision && mtv != nullptr)
    {
        *mtv = Vector3Normalize(pos1 - pos2) * (rad - Vector3Distance(pos1, pos2));
    }
    return collision;
}

// mtv resolves 1 from 2
RMAPI bool SphereCapsule(
    Vector3 pos1, float rad1,
    Vector3 pos2, Vector3 dir2, float rad2, float hh2,
    Vector3* mtv = nullptr)
{
    Vector3 top2 = pos2 + dir2 * hh2;
    Vector3 bot2 = pos2 - dir2 * hh2;
    Vector3 proj = Vector3ProjectPointLine(top2, bot2, pos1);
    return SphereSphere(pos1, rad1, proj, rad2, mtv);
}

// mtv resolves 1 from 2
RMAPI bool CapsuleCapsule(
    Vector3 pos1, Vector3 dir1, float rad1, float hh1,
    Vector3 pos2, Vector3 dir2, float rad2, float hh2,
    Vector3* mtv = nullptr)
{
    Vector3 top1 = pos1 + dir1 * hh1;
    Vector3 bot1 = pos1 - dir1 * hh1;
    Vector3 top2 = pos2 + dir2 * hh2;
    Vector3 bot2 = pos2 - dir2 * hh2;
    Vector3 near1, near2;
    NearestLinePoints(top1, bot1, top2, bot2, near1, near2);
    return SphereSphere(near1, rad1, near2, rad2, mtv);
}

// mtv resolves sphere from plane
RMAPI bool SpherePlane(
    Vector3 sphere, float radius,
    Vector3 plane, Vector3 normal,
    Vector3* mtv = nullptr)
{
    float distance = Vector3DotProduct(sphere - plane, normal);
    bool collision = fabsf(distance) <= radius;
    if (collision && mtv != nullptr)
        *mtv = normal * (radius - distance);
    return collision;
}

// mtv resolves capsule from plane
RMAPI bool CapsulePlane(
    Vector3 cap, Vector3 dir, float radius, float hh,
    Vector3 plane, Vector3 normal,
    Vector3* mtv = nullptr)
{
    Vector3 top = cap + dir * hh;
    Vector3 bot = cap - dir * hh;
    float d1 = Vector3DotProduct(top - plane, normal);
    float d2 = Vector3DotProduct(bot - plane, normal);
    float distance = d1 < d2 ? d1 : d2;
    bool collision = fabsf(distance) <= radius;
    if (collision && mtv != nullptr)
        *mtv = normal * (radius - fabsf(distance)) * (distance < 0 ? 1.0f : -1.0f);
    return collision;
}

// mtv resolves box from plane
RMAPI bool BoxPlane(
    Vector3 box, Vector3 extents,
    Vector3 plane, Vector3 normal,
    Vector3* mtv = nullptr)
{
    Vector3 near = Vector3Clamp(box - normal * Vector3LengthSqr(extents), box - extents, box + extents);
    return SpherePlane(near, 1.0f, plane, normal, mtv);
}

// mtv resolves sphere from box
RMAPI bool SphereBox(
    Vector3 sphere, float radius,
    Vector3 box, Vector3 extents,
    Vector3* mtv = nullptr)
{
    Vector3 near = Vector3Clamp(sphere, box - extents, box + extents);
    return SphereSphere(sphere, radius, near, 1.0f, mtv);
}

// mtv resolves capsule from box
RMAPI bool CapsuleBox(
    Vector3 cap, Vector3 dir, float radius, float hh,
    Vector3 box, Vector3 extents,
    Vector3* mtv = nullptr)
{
    Vector3 top = cap + dir * hh;
    Vector3 bot = cap - dir * hh;
    Vector3 min = box - extents;
    Vector3 max = box + extents;

    Vector3 top_left_front = box + Vector3{ -extents.x, extents.y, extents.z };
    Vector3 top_right_front = box + Vector3{ extents.x, extents.y, extents.z };
    Vector3 bot_left_front = box + Vector3{ -extents.x, -extents.y,extents.z };
    Vector3 bot_right_front = box + Vector3{ extents.x, -extents.y, extents.z };
    Vector3 top_left_back = box + Vector3{ -extents.x, extents.y, -extents.z };
    Vector3 top_right_back = box + Vector3{ extents.x, extents.y, -extents.z };
    Vector3 bot_left_back = box + Vector3{ -extents.x, -extents.y, -extents.z };
    Vector3 bot_right_back = box + Vector3{ extents.x, -extents.y, -extents.z };

    Vector3 fProjections[8]
    {
        Vector3ProjectPointLine(top, bot, top_left_front),
        Vector3ProjectPointLine(top, bot, top_right_front),
        Vector3ProjectPointLine(top, bot, bot_left_front),
        Vector3ProjectPointLine(top, bot, bot_right_front),
        Vector3ProjectPointLine(top, bot, top_left_back),
        Vector3ProjectPointLine(top, bot, top_right_back),
        Vector3ProjectPointLine(top, bot, bot_left_back),
        Vector3ProjectPointLine(top, bot, bot_right_back)
    };

    Vector3 clamps[8]
    {
        Vector3Clamp(fProjections[0], min, max),
        Vector3Clamp(fProjections[1], min, max),
        Vector3Clamp(fProjections[2], min, max),
        Vector3Clamp(fProjections[3], min, max),
        Vector3Clamp(fProjections[4], min, max),
        Vector3Clamp(fProjections[5], min, max),
        Vector3Clamp(fProjections[6], min, max),
        Vector3Clamp(fProjections[7], min, max)
    };

    int index = 0;
    float d0 = Vector3LengthSqr(fProjections[0] - clamps[0]);
    for (int i = 1; i < 8; i++)
    {
        float d = Vector3LengthSqr(fProjections[i] - clamps[i]);
        if (d < d0)
        {
            d0 = d;
            index = i;
        }
    }

    Vector3 proj = fProjections[index];
    Vector3 point = clamps[index];
    return SphereSphere(proj, radius, point, 1.0f, mtv);
}

// mtv resolves 1 from 2
RMAPI bool BoxBox(
    Vector3 box1, Vector3 extents1,
    Vector3 box2, Vector3 extents2,
    Vector3* mtv = nullptr)
{
    float xMin1 = box1.x - extents1.x;
    float xMax1 = box1.x + extents1.x;
    float yMin1 = box1.y - extents1.y;
    float yMax1 = box1.y + extents1.y;
    float zMin1 = box1.z - extents1.z;
    float zMax1 = box1.z + extents1.z;

    float xMin2 = box2.x - extents2.x;
    float xMax2 = box2.x + extents2.x;
    float yMin2 = box2.y - extents2.y;
    float yMax2 = box2.y + extents2.y;
    float zMin2 = box2.z - extents2.z;
    float zMax2 = box2.z + extents2.z;

    bool collision =
        Overlaps(xMin1, xMax1, xMin2, xMax2) &&
        Overlaps(yMin1, yMax1, yMin2, yMax2) &&
        Overlaps(zMin1, zMax1, zMin2, zMax2);
    if (collision && mtv != nullptr)
    {
        // Overlapping area (box)
        float xMin = fmaxf(xMin1, xMin2);
        float xMax = fminf(xMax1, xMax2);
        float yMin = fmaxf(yMin1, yMin2);
        float yMax = fminf(yMax1, yMax2);
        float zMin = fmaxf(zMin1, zMin2);
        float zMax = fminf(zMax1, zMax2);

        // Resolve along the shortest axis (x vs y vs z)
        float x = xMax - xMin;
        float y = yMax - yMin;
        float z = zMax - zMin;

        if (x < y && x < z)
        {
            *mtv = { x, 0.0f , 0.0f};
            if (box2.x > xMin) mtv->x *= -1.0f;
        }
        else if (y < x && y < z)
        {
            *mtv = { 0.0f, y , 0.0f };
            if (box2.y > yMin) mtv->y *= -1.0f;
        }
        else if (z < x && z < y)
        {
            *mtv = { 0.0f, 0.0f, z };
            if (box2.z > zMin) mtv->z *= -1.0f;
        }
        else
        {
            *mtv = { x, y, z };
            if (box2.x > xMin) mtv->x *= -1.0f;
            if (box2.y > yMin) mtv->y *= -1.0f;
            if (box2.z > zMin) mtv->z *= -1.0f;
        }
    }
    return collision;
}

RMAPI void NearestLinePoints(
    Vector3 top1, Vector3 bot1,
    Vector3 top2, Vector3 bot2,
    Vector3& near1, Vector3& near2)
{
    Vector3 A = Vector3ProjectPointLine(top2, bot2, top1);
    Vector3 B = Vector3ProjectPointLine(top2, bot2, bot1);
    Vector3 C = Vector3ProjectPointLine(top1, bot1, A);
    Vector3 D = Vector3ProjectPointLine(top1, bot1, B);
    near1 = Vector3LengthSqr(A - C) < Vector3LengthSqr(B - D) ? C : D;

    Vector3 E = Vector3ProjectPointLine(top1, bot1, top2);
    Vector3 F = Vector3ProjectPointLine(top1, bot1, bot2);
    Vector3 G = Vector3ProjectPointLine(top2, bot2, E);
    Vector3 H = Vector3ProjectPointLine(top2, bot2, F);
    near2 = Vector3LengthSqr(E - G) < Vector3LengthSqr(F - H) ? G : H;
}

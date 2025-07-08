#pragma once
#include "raymath.h"
#include <cstdlib>

RMAPI float Random(float min, float max)
{
    return min + (rand() / ((float)RAND_MAX / (max - min)));
}

RMAPI float Sign(float value)
{
    float result = value < 0.0f ? -1.0f : 1.0f;
    return result;
}

RMAPI bool Overlaps(float min1, float max1, float min2, float max2)
{
    return !((max1 < min2) || (max2 < min1));
}

RMAPI Vector3 RandomSpherePosition(float radius)
{
    Vector3 position = Vector3Zeros;
    position.x = Random(-1.0f, 1.0f);
    position.y = Random(-1.0f, 1.0f);
    position.z = Random(-1.0f, 1.0f);
    position = Vector3Normalize(position) * radius;
    return position;
}

RMAPI Vector3 RandomBoxPosition(Vector3 size)
{
    Vector3 position = Vector3Zeros;
    position.x = Random(-size.x * 0.5f, size.x * 0.5f);
    position.y = Random(-size.y * 0.5f, size.y * 0.5f);
    position.z = Random(-size.z * 0.5f, size.z * 0.5f);
    return position;
}

RMAPI float Vector2CrossProduct(Vector2 v1, Vector2 v2)
{
    float result = v1.x * v2.y - v1.y * v2.x;
    return result;
}

RMAPI Vector2 Vector2RotateTowards(Vector2 from, Vector2 to, float maxRadiansDelta)
{
    float angle = Vector2Angle(from, to);
    float angleDelta = fminf(fabsf(angle), maxRadiansDelta) * Sign(angle);
    Vector2 result = Vector2Rotate(from, angleDelta);
    return result;
}

RMAPI Vector2 Vector2ProjectPointLine(Vector2 A, Vector2 B, Vector2 P)
{
    Vector2 AB = (B - A);
    float t = Vector2DotProduct((P - A), AB) / Vector2DotProduct(AB, AB);
    return A + (AB * Clamp(t, 0.0f, 1.0f));
}

RMAPI Vector3 Vector3ProjectPointLine(Vector3 A, Vector3 B, Vector3 P)
{
    Vector3 AB = (B - A);
    float t = Vector3DotProduct((P - A), AB) / Vector3DotProduct(AB, AB);
    return A + (AB * Clamp(t, 0.0f, 1.0f));
}

RMAPI float QuaternionAngle(Quaternion from, Quaternion to)
{
    // Flip to-quaternion and dot-product if negative to ensure optimal rotation
    float dot = Vector4DotProduct(from, to);
    if (dot < 0.0f)
    {
        to = QuaternionScale(to, -1.0f);
        dot = -dot;
    }

    // Quaternions store half-angles ie 45 degrees in 4D space is 90 degrees in 3D space
    float angle = acosf(dot) * 2.0f;
    return angle;
}

RMAPI Quaternion QuaternionRotateTowards(Quaternion from, Quaternion to, float maxRadiansDelta)
{
    float angle = QuaternionAngle(from, to);
    float t = fminf(angle, maxRadiansDelta) / angle;
    return QuaternionSlerp(from, to, t);
}

RMAPI Matrix MatrixLookRotation(Vector3 direction)
{
    Vector3 forward = Vector3Normalize(direction);
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3UnitZ));
    Vector3 up = Vector3Normalize(Vector3CrossProduct(right, forward));

    Matrix mat = MatrixIdentity();
    mat.m0 = right.x;
    mat.m1 = right.y;
    mat.m2 = right.z;

    mat.m4 = forward.x;
    mat.m5 = forward.y;
    mat.m6 = forward.z;

    mat.m8 = up.x;
    mat.m9 = up.y;
    mat.m10 = up.z;
    return mat;
}

RMAPI Vector3 MatrixColX(Matrix m)
{
    Vector3 result = { m.m0, m.m1, m.m2 };
    return result;
}

RMAPI Vector3 MatrixColY(Matrix m)
{
    Vector3 result = { m.m4, m.m5, m.m6 };
    return result;
}

RMAPI Vector3 MatrixColZ(Matrix m)
{
    Vector3 result = { m.m8, m.m9, m.m10 };
    return result;
}

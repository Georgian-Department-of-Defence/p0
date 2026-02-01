#include "DebugDraw.h"

void DrawLineDebug(Vector3 p0, Vector3 p1, Color color, float thickness)
{
    rlDrawRenderBatchActive();
    rlSetLineWidth(thickness);

    DrawLine3D(p0, p1, color);

    rlDrawRenderBatchActive();
    rlSetLineWidth(1.0f);
}

void DrawAxesDebug(Vector3 position, Matrix rotation, float length, float thickness)
{
    Vector3 x = MatrixColX(rotation);
    Vector3 y = MatrixColY(rotation);
    Vector3 z = MatrixColZ(rotation);
    
    DrawLineDebug(position, position + x * length, RED, thickness);
    DrawLineDebug(position, position + y * length, GREEN, thickness);
    DrawLineDebug(position, position + z * length, BLUE, thickness);
}

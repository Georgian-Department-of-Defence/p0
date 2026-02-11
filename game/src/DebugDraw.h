#pragma once
void DrawLineDebug(Vector3 p0, Vector3 p1, Color color, float thickness = 1.0f);
void DrawAxesDebug(Vector3 position, Matrix rotation, float length, float thickness = 1.0f);

void DrawTextureColor(RenderTexture rt);
void DrawTextureDepth(RenderTexture rt);

void DrawCollider(Collider collider, Color color);
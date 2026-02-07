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

void DrawTextureColor(RenderTexture rt)
{
    Rectangle src_rec;
    src_rec.x = 0;
    src_rec.y = 0;
    src_rec.width = (float)rt.texture.width;
    src_rec.height = (float)-rt.texture.height;

    Rectangle dst_rec;
    dst_rec.x = 0;
    dst_rec.y = 0;
    dst_rec.width = (float)GetScreenWidth();
    dst_rec.height = (float)GetScreenHeight();

    DrawTexturePro(rt.texture, src_rec, dst_rec, Vector2Zeros, 0.0f, WHITE);
}

void DrawTextureDepth(RenderTexture rt)
{
    Rectangle src_rec;
    src_rec.x = 0;
    src_rec.y = 0;
    src_rec.width = (float)rt.depth.width;
    src_rec.height = (float)-rt.depth.height;

    Rectangle dst_rec;
    dst_rec.x = 0;
    dst_rec.y = 0;
    dst_rec.width = (float)GetScreenWidth();
    dst_rec.height = (float)GetScreenHeight();

    Shader shader = assets.material.depth.shader;
    BeginShaderMode(shader);
    int loc_depth_tex = GetShaderLocation(shader, "tex_depth");
    int loc_z_near = GetShaderLocation(shader, "z_near");
    int loc_z_far = GetShaderLocation(shader, "z_far");
    float z_near = (float)rlGetCullDistanceNear();
    float z_far = (float)rlGetCullDistanceFar();
    SetShaderValue(shader, loc_z_near, &z_near, RL_SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, loc_z_far, &z_far, RL_SHADER_UNIFORM_FLOAT);
    SetShaderValueTexture(shader, loc_depth_tex, rt.depth);
    DrawTexturePro(rt.depth, src_rec, dst_rec, Vector2Zeros, 0.0f, WHITE);
    EndShaderMode();
}

#include "Camera.h"
#include "rcamera.h"
#include "rlgl.h"
#include <cfloat>

CameraSystem g_camera_system;

void SetCursor(bool enabled)
{
    if (enabled)
        DisableCursor();
    else
        EnableCursor();
}

void LoadCamera()
{
    // Original FoV was 28.5 degrees
    //CameraPitch(&td_camera, 10.0f * DEG2RAD, false, false, true);

    Camera camera_top_down;
    camera_top_down.up = Vector3UnitY;
    camera_top_down.position = { 0.0f, -45.0f, 230.0f };
    camera_top_down.target = { 0.0f, 5.0f, 0.0f };
    camera_top_down.fovy = 30.0f;
    camera_top_down.projection = CAMERA_PERSPECTIVE;

    Camera camera_first_person;
    camera_first_person.position = { 0.0f, 25.0f, 2.0f };
    camera_first_person.target = { 0.0f, 0.0f, 2.0f };
    camera_first_person.up = Vector3UnitZ;
    camera_first_person.fovy = 60.0f;
    camera_first_person.projection = CAMERA_PERSPECTIVE;

    Camera3D camera_ui_3d;
    camera_ui_3d.position = { 0.0f, 0.0f, 100.0f };
    camera_ui_3d.target = Vector3Zeros;
    camera_ui_3d.up = Vector3UnitY;
    camera_ui_3d.fovy = 50.0f;
    camera_ui_3d.projection = CAMERA_ORTHOGRAPHIC;

    Camera2D camera_ui_2d;
    camera_ui_2d.offset = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    camera_ui_2d.target = { 0.0f, 0.0f };
    camera_ui_2d.rotation = 0.0f;
    camera_ui_2d.zoom = 1.0f;

    double edge = 96.0;
    g_camera_system.light_pos = { WORLD_MAX_X, WORLD_MAX_Y * 2.0f, 250.0f };
    g_camera_system.light_view = MatrixLookAt(g_camera_system.light_pos, Vector3Zeros, Vector3UnitZ);
    g_camera_system.light_proj = MatrixOrtho(-edge, edge, -edge, edge, 0.1, 512.0);

    g_camera_system.cursor_enabled = false;
    g_camera_system.mode = CAMERA_MODE_TOP_DOWN;
    g_camera_system.camera_top_down = camera_top_down;
    g_camera_system.camera_first_person = camera_first_person;
    g_camera_system.camera_ui_3d = camera_ui_3d;
    g_camera_system.camera_ui_2d = camera_ui_2d;

    SetCursor(g_camera_system.cursor_enabled);
}

void UnloadCamera()
{
}

void UpdateCamera()
{
    Camera* camera = GetCamera();
    if (g_camera_system.cursor_enabled)
    {
        UpdateCamera(camera, CAMERA_FREE);
    }

    if (IsKeyPressed(KEY_V))
    {
        g_camera_system.cursor_enabled = !g_camera_system.cursor_enabled;
        SetCursor(g_camera_system.cursor_enabled);
    }

    if (IsKeyPressed(KEY_C))
    {
        int mode = g_camera_system.mode;
        ++mode %= CAMERA_MODE_COUNT;
        g_camera_system.mode = (CameraSystemMode)mode;
    }
}

// Auto-bound is too finicky for it to be worth-while
//Vector3 vertices[8];
//for (int i = 0; i < 8; i++) {
//    vertices[i] = Vector3
//    {
//        (i & 1) ? WORLD_MIN_X : WORLD_MAX_X,    // bit 0: x coordinate
//        (i & 2) ? WORLD_MIN_Y : WORLD_MAX_Y,    // bit 1: y coordinate  
//        (i & 4) ? 0.0f : 50.0f                  // bit 2: z coordinate
//    };
//}
//
//
//Vector3 light_min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
//Vector3 light_max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
//for (int i = 0; i < 8; i++)
//{
//    Vector3 v = Vector3Transform(vertices[i], light_view);
//    light_min = Vector3Min(v, light_min);
//    light_max = Vector3Max(v, light_max);
//}
//
//g_camera_system.light_proj = MatrixOrtho(light_min.x, light_max.x, light_min.y, light_max.y, fabsf(light_max.z), fabsf(light_min.z));

#include "Camera.h"
#include "rcamera.h"

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
    Camera td_camera;
    //tdCamera.position = Vector3UnitZ * 100.0f;
    //tdCamera.target = Vector3Zeros;
    td_camera.position = { 0.0f, -30.0f, 70.0f };
    td_camera.target = { 0.0f, -20.0f, 0.0f };
    td_camera.up = Vector3UnitY;
    td_camera.fovy = 75.0f;
    td_camera.projection = CAMERA_PERSPECTIVE;
    CameraPitch(&td_camera, 10.0f * DEG2RAD, false, false, true);

    Camera fp_camera;
    fp_camera.position = { 0.0f, 25.0f, 2.0f };
    fp_camera.target = { 0.0f, 0.0f, 2.0f };
    fp_camera.up = Vector3UnitZ;
    fp_camera.fovy = 60.0f;
    fp_camera.projection = CAMERA_PERSPECTIVE;

    Camera editor_camera;
    editor_camera.position = Vector3UnitZ * 100.0f;
    editor_camera.target = Vector3Zeros;
    editor_camera.up = Vector3UnitY;
    editor_camera.fovy = WORLD_MAX_Y - WORLD_MIN_Y;
    editor_camera.projection = CAMERA_ORTHOGRAPHIC;

    g_camera_system.td_camera = td_camera;
    g_camera_system.fp_camera = fp_camera;
    g_camera_system.editor_camera = editor_camera;
    g_camera_system.behaviour = CAM_TOP_DOWN;
    g_camera_system.is_enabled = false;

    SetCursor(g_camera_system.is_enabled);
}

void UnloadCamera()
{
}

void UpdateCamera()
{
    Camera* camera = GetCamera();
    if (g_camera_system.is_enabled)
    {
        UpdateCamera(camera, CAMERA_FREE);
        // gCameraSystem.isFirstPerson ? CAMERA_FIRST_PERSON : CAMERA_FREE);
    }

    if (IsKeyPressed(KEY_V))
    {
        g_camera_system.is_enabled = !g_camera_system.is_enabled;
        SetCursor(g_camera_system.is_enabled);
    }

    if (IsKeyPressed(KEY_C))
    {
        g_camera_system.behaviour = g_camera_system.behaviour == CAM_FIRST_PERSON ?
            CAM_TOP_DOWN : CAM_FIRST_PERSON;
    }
}

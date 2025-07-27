#include "Camera.h"
#include "rcamera.h"
#include "rlgl.h"

CameraSystem g_camera_system;

void SetCursor(bool enabled)
{
    if (enabled)
        DisableCursor();
    else
        EnableCursor();
}

void OnCameraChange(CameraSystemMode mode);

void LoadCamera()
{
    // Original FoV was 28.5 degrees
    //CameraPitch(&td_camera, 10.0f * DEG2RAD, false, false, true);

    Camera td_camera;
    td_camera.position = { 0.0f, -45.0f, 230.0f };
    td_camera.target = { 0.0f, 5.0f, 0.0f };
    td_camera.up = Vector3UnitY;
    td_camera.fovy = 30.0f;
    td_camera.projection = CAMERA_PERSPECTIVE;

    Camera fp_camera;
    fp_camera.position = { 0.0f, 25.0f, 2.0f };
    fp_camera.target = { 0.0f, 0.0f, 2.0f };
    fp_camera.up = Vector3UnitZ;
    fp_camera.fovy = 60.0f;
    fp_camera.projection = CAMERA_PERSPECTIVE;

    g_camera_system.td_camera = td_camera;
    g_camera_system.fp_camera = fp_camera;
    g_camera_system.cursor_enabled = false;
    g_camera_system.mode = CAMERA_MODE_TOP_DOWN;

    OnCameraChange(g_camera_system.mode);
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
        g_camera_system.mode = g_camera_system.mode == CAMERA_MODE_FIRST_PERSON ?
            CAMERA_MODE_TOP_DOWN : CAMERA_MODE_FIRST_PERSON;

        OnCameraChange(g_camera_system.mode);
    }
}

void OnCameraChange(CameraSystemMode mode)
{
    float zNear = 0.0f, zFar = 0.0f;
    if (mode == CAMERA_MODE_FIRST_PERSON)
    {
        zNear = 0.1f;
        zFar = 1000.0f;
    }
    else
    {
        zNear = 100.0f;
        zFar = 1000.0f;
    }
    rlSetClipPlanes(zNear, zFar);
}

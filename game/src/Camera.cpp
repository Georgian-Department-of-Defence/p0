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

    Camera top_down_camera;
    top_down_camera.up = Vector3UnitY;
    top_down_camera.position = { 0.0f, -45.0f, 230.0f };
    top_down_camera.target = { 0.0f, 5.0f, 0.0f };
    top_down_camera.fovy = 30.0f;
    top_down_camera.projection = CAMERA_PERSPECTIVE;
    //td_camera.position = { 0.0f, -30.0f, 230.0f };
    //td_camera.target = Vector3Zeros;
    //td_camera.fovy = 110.0f;
    //td_camera.projection = CAMERA_ORTHOGRAPHIC;

    Camera first_person_camera;
    first_person_camera.position = { 0.0f, 25.0f, 2.0f };
    first_person_camera.target = { 0.0f, 0.0f, 2.0f };
    first_person_camera.up = Vector3UnitZ;
    first_person_camera.fovy = 60.0f;
    first_person_camera.projection = CAMERA_PERSPECTIVE;

    g_camera_system.top_down_camera = top_down_camera;
    g_camera_system.first_person_camera = first_person_camera;
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

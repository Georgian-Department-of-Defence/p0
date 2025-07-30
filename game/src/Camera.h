#pragma once
#include "Constants.h"

enum CameraSystemMode : int
{
    CAMERA_MODE_TOP_DOWN,
    CAMERA_MODE_FIRST_PERSON,
    CAMERA_MODE_COUNT,
};

struct CameraSystem
{
    CameraSystemMode mode;
    bool cursor_enabled;

    Camera top_down_camera;
    Camera first_person_camera;

    // Shadow camera implemented via rlgl
    Vector3 light_pos;
    Matrix light_view;
    Matrix light_proj;
};

extern CameraSystem g_camera_system;

inline Camera* GetCamera()
{
    Camera* camera = nullptr;
    switch (g_camera_system.mode)
    {
    case CAMERA_MODE_TOP_DOWN:
        camera = &g_camera_system.top_down_camera;
        break;

    case CAMERA_MODE_FIRST_PERSON:
        camera = &g_camera_system.first_person_camera;
        break;
    }
    return camera;
}

void LoadCamera();
void UnloadCamera();

void UpdateCamera();
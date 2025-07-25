#pragma once
#include "Constants.h"

enum CameraSystemMode
{
    CAMERA_MODE_TOP_DOWN,
    CAMERA_MODE_FIRST_PERSON
};

struct CameraSystem
{
    CameraSystemMode mode;
    bool cursor_enabled;
    Camera top_down_camera;
    Camera first_person_camera;
    //Camera shadow_map_camera;
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
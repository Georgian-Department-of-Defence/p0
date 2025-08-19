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

    Camera camera_top_down;
    Camera camera_first_person;

    Camera3D camera_ui_3d;
    Camera2D camera_ui_2d;

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
        camera = &g_camera_system.camera_top_down;
        break;

    case CAMERA_MODE_FIRST_PERSON:
        camera = &g_camera_system.camera_first_person;
        break;
    }
    return camera;
}

void LoadCamera();
void UnloadCamera();

void UpdateCamera();
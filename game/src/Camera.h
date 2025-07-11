#pragma once
#include "Constants.h"

enum CameraBehaviour
{
    CAM_TOP_DOWN,
    CAM_FIRST_PERSON
    //CAM_FOLLOW,
    //CAM_EDITOR
};

struct CameraSystem
{
    Camera td_camera;
    Camera fp_camera;
    //Camera fl_camera;
    //Camera editor_camera;
    CameraBehaviour behaviour;
    bool is_enabled;
};

extern CameraSystem g_camera_system;

inline Camera* GetCamera()
{
    Camera* camera = nullptr;
    switch (g_camera_system.behaviour)
    {
    case CAM_TOP_DOWN:
        camera = &g_camera_system.td_camera;
        break;

    case CAM_FIRST_PERSON:
        camera = &g_camera_system.fp_camera;
        break;

    //case CAM_FOLLOW:
    //    camera = &g_camera_system.fl_camera;
    //    break;
    //
    //case CAM_EDITOR:
    //    camera = &g_camera_system.editor_camera;
    //    break;
    }
    return camera;
}

void LoadCamera();
void UnloadCamera();

void UpdateCamera();
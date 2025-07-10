#include "EditorScene.h"
#include "Camera.h"
#include "rlgl.h"

static Camera f_camera;

void EditorScene::OnLoad()
{
}

void EditorScene::OnUnload()
{
}

void EditorScene::OnStart()
{
    g_camera_system.behaviour = CAM_EDITOR;
}

void EditorScene::OnUpdate()
{
	UpdateCamera();
}

void EditorScene::OnDraw()
{
    BeginMode3D(*GetCamera());

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(-50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    EndMode3D();
}

void EditorScene::OnDrawDebug()
{
}

void EditorScene::OnDrawGui()
{
    Ray ray = GetScreenToWorldRay(GetMousePosition(), *GetCamera());
    Vector3 p1 = { WORLD_MIN_X, WORLD_MAX_Y, 0.0f };
    Vector3 p2 = { WORLD_MIN_X, WORLD_MIN_Y, 0.0f };
    Vector3 p3 = { WORLD_MAX_X, WORLD_MIN_Y, 0.0f };
    Vector3 p4 = { WORLD_MAX_X, WORLD_MAX_Y, 0.0f };
    RayCollision collision = GetRayCollisionQuad(ray, p1, p2, p3, p4);
    if (collision.hit)
    {
        DrawText(TextFormat("%f %f %f", collision.point.x, collision.point.y, collision.point.z), 10, 40, 20, RED);
    }
}

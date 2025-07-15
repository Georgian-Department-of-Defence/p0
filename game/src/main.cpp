#include "raylib.h"
#include "raymathext.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"
#include "DebugDraw.h"

#include "Scene.h"
#include "World.h"

#include <algorithm>
#include <vector>
#include <iostream>

void AppLoad()
{
    LoadCamera();
    LoadMeshes();
    LoadShaders();
    LoadTextures();
    LoadAudio();
}

void AppUnload()
{
    UnloadAudio();
    UnloadTextures();
    UnloadShaders();
    UnloadMeshes();
    UnloadCamera();
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(144);
    AppLoad();
    Scene::Load(SCENE_DEV_EDITOR);

    while (!WindowShouldClose())
    {
#ifdef DEBUG
        if (IsKeyPressed(KEY_G))
        {
            for (int i = 0; i < 4; i++)
                TraceLog(LOG_INFO, "Gamepad %i %s", i, IsGamepadAvailable(i) ? "connected" : "disconnected");
        }
#endif

        Scene::Update();
        BeginDrawing();
        ClearBackground(BLACK);
        Scene::Draw();
#ifdef DEBUG
        Scene::DrawDebug();
#endif
        Scene::DrawGui();
        EndDrawing();
    }

    Scene::Unload();
    AppUnload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

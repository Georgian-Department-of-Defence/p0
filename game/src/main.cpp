#include "raylib.h"
#include "raymathext.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"

#include "Scene.h"
#include "Game.h"

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
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    AppLoad();

    Game game;
    LoadRenderer(game.renderer);
    Scene::Load(game, SCENE_DEV_MAP);
    while (!WindowShouldClose())
    {
#ifdef DEBUG
        if (IsKeyPressed(KEY_G))
        {
            for (int i = 0; i < 4; i++)
                TraceLog(LOG_INFO, "Gamepad %i %s", i, IsGamepadAvailable(i) ? "connected" : "disconnected");
        }
#endif

        Scene::Update(game);
        BeginDrawing();
        ClearBackground(BLACK);
        Scene::Draw(game);
#ifdef DEBUG
        Scene::DrawDebug(game);
#endif
        Scene::DrawGui(game);
        EndDrawing();
    }
    UnloadRenderer(game.renderer);
    Scene::Unload(game);

    AppUnload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

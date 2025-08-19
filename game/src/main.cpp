#include "raylib.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"

#include "Scene.h"
#include "Game.h"

void LoadAssets()
{
    LoadCamera();
    LoadMeshes();
    LoadShaders();
    LoadTextures();
    LoadAudio();
}

void UnloadAssets()
{
    UnloadAudio();
    UnloadTextures();
    UnloadShaders();
    UnloadMeshes();
    UnloadCamera();
}

int main()
{
    Game game;
    game.renderer.flags = FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT;// | FLAG_MSAA_4X_HINT;

    SetConfigFlags(game.renderer.flags);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    LoadAssets();
    LoadRenderer(game.renderer);
    Scene::Load(game, SCENE_TEAM_SELECT);

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
        ClearBackground(MAGENTA);
        
            Scene::Draw(game);
//#ifdef DEBUG
//          Scene::DrawDebug(game);
//#endif
//          Scene::DrawGui(game);

        // TODO - draw this in world-space because this currently depends on screen resolution
        DrawFPS(10, 30);
        EndDrawing();
    }
    UnloadRenderer(game.renderer);
    Scene::Unload(game);
    UnloadAssets();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

#include "raylib.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"

#include "Camera.h"
#include "Assets.h"

#include "Scene.h"
#include "Game.h"

int main()
{
    Game game;
    game.renderer.flags = FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT;// | FLAG_MSAA_4X_HINT;

    SetConfigFlags(game.renderer.flags);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetMasterVolume(0.25f); // Make audio quiet for testing
    
    LoadCamera();
    LoadAssets();
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
    Scene::Unload(game);
    UnloadRenderer(game.renderer);
    UnloadAssets();
    UnloadCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

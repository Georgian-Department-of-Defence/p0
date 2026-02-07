#include "p0.h"

struct Game
{
    World2 world;
};

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetMasterVolume(0.25f); // Make audio quiet for testing
    
    LoadCamera();
    LoadAssets();

    Game game;
    WorldLoad(game.world);

    while (!WindowShouldClose())
    {
        UpdateCamera();
        WorldUpdate(game.world);

        BeginDrawing();
            ClearBackground(MAGENTA);
            WorldDraw(game.world);
            DrawFPS(10, 30);
        EndDrawing();
    }

    WorldUnload(game.world);
    UnloadAssets();
    UnloadCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

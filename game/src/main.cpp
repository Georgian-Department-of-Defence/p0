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
    LoadWorld(game.world);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            if (!game.world.entities.empty())
                game.world.entities.back()->destroy_flag = true;
        }

        UpdateCamera();
        UpdateWorldFrame(game.world);
        UpdateWorld(game.world);

        BeginDrawing();
        ClearBackground(MAGENTA);

        DrawWorld(game.world);        
        
        DrawFPS(10, 30);
        EndDrawing();
    }

    UnloadWorld(game.world);
    UnloadAssets();
    UnloadCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

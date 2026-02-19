#include "p0.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetMasterVolume(0.25f); // Make audio quiet for testing
    
    // TODO -- Merge camera with Game?
    InitCamera();
    InitAssets();

    Game game;
    GameInit(game, SCENE_DEV_MAP);
    while (!WindowShouldClose())
    {
        UpdateCamera();
        GameUpdate(game);

        BeginDrawing();
            GameDraw(game);
            DrawFPS(10, 10);
        EndDrawing();
    }
    GameQuit(game);

    QuitAssets();
    QuitCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

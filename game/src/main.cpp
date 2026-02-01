#include "p0.h"

struct Game
{
    //World world;
    Renderer renderer;
};

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
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(MAGENTA);
        
        DrawFPS(10, 30);
        EndDrawing();
    }
    UnloadRenderer(game.renderer);
    UnloadAssets();
    UnloadCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

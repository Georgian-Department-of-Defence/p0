#include "p0.h"

struct Game
{
    World2 world;
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
    LoadWorld(game.world);

    while (!WindowShouldClose())
    {
        UpdateCamera();
        for (Entity* entity : game.world.entities)
            entity->OnUpdate();

        BeginDrawing();
        ClearBackground(MAGENTA);

        BeginMode3D(*GetCamera());
        for (const Entity* entity : game.world.entities)
            entity->OnDraw();
        EndMode3D();
        
        DrawFPS(10, 30);
        EndDrawing();
    }

    UnloadWorld(game.world);
    UnloadRenderer(game.renderer);
    UnloadAssets();
    UnloadCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

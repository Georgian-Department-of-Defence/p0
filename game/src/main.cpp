#include "p0.h"

struct Game
{
    std::vector<Entity*> entities;
    //World world;
    Renderer renderer;
};

int main()
{
    Game game;
    game.renderer.flags = FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT;// | FLAG_MSAA_4X_HINT;
    game.entities.push_back(new Mech2);

    SetConfigFlags(game.renderer.flags);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetMasterVolume(0.25f); // Make audio quiet for testing
    
    LoadCamera();
    LoadAssets();
    LoadRenderer(game.renderer);

    for (Entity* entity : game.entities)
        entity->OnLoad();

    while (!WindowShouldClose())
    {
        UpdateCamera();
        for (Entity* entity : game.entities)
            entity->OnUpdate();

        BeginDrawing();
        ClearBackground(MAGENTA);

        BeginMode3D(*GetCamera());
        for (const Entity* entity : game.entities)
            entity->OnDraw();
        EndMode3D();
        
        DrawFPS(10, 30);
        EndDrawing();
    }

    for (Entity* entity : game.entities)
        entity->OnUnload();

    for (size_t i = 0; i < game.entities.size(); i++)
        delete game.entities[i];
    game.entities.clear();

    UnloadRenderer(game.renderer);
    UnloadAssets();
    UnloadCamera();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

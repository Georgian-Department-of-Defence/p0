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
        UpdateCamera();

        Matrix rz = MatrixRotateZ(100.0f * GetTime() * DEG2RAD);
        Matrix rx = MatrixRotateX(100.0f * GetTime() * DEG2RAD);
        Matrix r = rz * rx;
        //r = MatrixRotateZ(30.0f * DEG2RAD);

        Vector3 x = MatrixColX(r);
        Vector3 y = MatrixColY(r);
        Vector3 z = MatrixColZ(r);

        BeginDrawing();
        ClearBackground(MAGENTA);

        BeginMode3D(*GetCamera());

        DrawMesh(assets.mesh.torso, assets.material.flat, r);
        DrawMesh(assets.mesh.legs, assets.material.flat, r);
        DrawAxesDebug(Vector3Zeros, r, 25.0f, 5.0f);
        EndMode3D();
        
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

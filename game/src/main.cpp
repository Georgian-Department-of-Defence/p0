#include "raylib.h"
#include "raymath.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Game");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Hello World!", 10, 10, 20, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

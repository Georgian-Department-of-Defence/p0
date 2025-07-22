#include "raylib.h"
#include "raymathext.h"
#include "rlgl.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"

#include "Scene.h"
#include "Game.h"

void AppLoad()
{
    LoadCamera();
    LoadMeshes();
    LoadShaders();
    LoadTextures();
    LoadAudio();
}

void AppUnload()
{
    UnloadAudio();
    UnloadTextures();
    UnloadShaders();
    UnloadMeshes();
    UnloadCamera();
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    AppLoad();
    rlClearColor(255, 0, 255, 255);

    Game game;
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
        ClearBackground(BLACK);
        
        rlActiveDrawBuffers(1);
        rlEnableFramebuffer(game.renderer.fbo);
        rlClearScreenBuffers();

        Scene::Draw(game);
#ifdef DEBUG
        Scene::DrawDebug(game);
#endif
        Scene::DrawGui(game);

        rlDisableFramebuffer();

        Texture2D screen_tex;
        screen_tex.id = game.renderer.tex_color;
        screen_tex.width = RENDER_WIDTH;
        screen_tex.height = RENDER_HEIGHT;

        Rectangle screen_rec;
        screen_rec.x = 0;
        screen_rec.y = 0;
        screen_rec.width = GetScreenWidth();
        screen_rec.height = -GetScreenHeight();

        DrawTextureRec(screen_tex, screen_rec, Vector2Zeros, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadRenderer(game.renderer);
    Scene::Unload(game);

    AppUnload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

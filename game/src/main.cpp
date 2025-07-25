#include "raylib.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"

#include "Scene.h"
#include "Game.h"

void LoadAssets()
{
    LoadCamera();
    LoadMeshes();
    LoadShaders();
    LoadTextures();
    LoadAudio();
}

void UnloadAssets()
{
    UnloadAudio();
    UnloadTextures();
    UnloadShaders();
    UnloadMeshes();
    UnloadCamera();
}

int main()
{
    Game game;
    game.renderer.flags = FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT;

    SetConfigFlags(game.renderer.flags);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    LoadAssets();
    LoadRenderer(game.renderer);
    Scene::Load(game, SCENE_DEV_MAP);

    int loc_depth_tex = GetShaderLocation(g_shaders.depth, "tex_depth");
    int loc_z_near = GetShaderLocation(g_shaders.depth, "z_near");
    int loc_z_far = GetShaderLocation(g_shaders.depth, "z_far");

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
        // Anything not sampled from custom fbo will have a magenta background
        
        BeginTextureMode(game.renderer.rt_downsample);
            ClearBackground(BLACK);
            Scene::Draw(game);
#ifdef DEBUG
            Scene::DrawDebug(game);
#endif
            Scene::DrawGui(game);
        EndTextureMode();
        
        RenderTexture& rt = game.renderer.rt_downsample;

        Rectangle src_rec;
        src_rec.x = 0;
        src_rec.y = 0;
        src_rec.width = rt.texture.width;
        src_rec.height = -rt.texture.height;
        
        Rectangle dst_rec;
        dst_rec.x = 0;
        dst_rec.y = 0;
        dst_rec.width = GetScreenWidth();
        dst_rec.height = GetScreenHeight();
        
        DrawTexturePro(rt.texture, src_rec, dst_rec, Vector2Zeros, 0.0f, WHITE);

        BeginShaderMode(g_shaders.depth);
            float z_near = rlGetCullDistanceNear();
            float z_far = rlGetCullDistanceFar();
            SetShaderValue(g_shaders.depth, loc_z_near, &z_near, RL_SHADER_UNIFORM_FLOAT);
            SetShaderValue(g_shaders.depth, loc_z_far, &z_far, RL_SHADER_UNIFORM_FLOAT);
            SetShaderValueTexture(g_shaders.depth, loc_depth_tex, rt.depth);
            DrawTexturePro(rt.depth, src_rec, dst_rec, Vector2Zeros, 0.0f, WHITE);
        EndShaderMode();

        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadRenderer(game.renderer);
    Scene::Unload(game);
    UnloadAssets();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

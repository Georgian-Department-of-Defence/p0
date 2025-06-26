#include "raylib.h"
#include "raymathext.h"

#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"
#include "DebugDraw.h"

#include "Scene.h"
#include "World.h"

#include <algorithm>
#include <vector>
#include <iostream>

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

struct Object {
    int id;
    bool enabled;

    Object(int i, bool e) : id(i), enabled(e) {}
};

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "PRIMEOPS ZERO");
    InitAudioDevice();
    SetTargetFPS(144);
    AppLoad();
    Scene::Load(SCENE_DEV_MAP);

    //std::vector<Object> objects = {
    //    Object(1, false),
    //    Object(2, true),
    //    Object(3, false),
    //    Object(4, true),
    //    Object(5, false),
    //    Object(6, true)
    //};
    //
    //std::cout << "Before partition:\n";
    //for (const auto& obj : objects) {
    //    std::cout << "ID: " << obj.id << ", Enabled: " << obj.enabled << "\n";
    //}
    //
    //// Partition: enabled objects at front, disabled at back
    //auto partition_point = std::partition(objects.begin(), objects.end(),
    //    [](const Object& obj) { return obj.enabled; });
    //
    //std::cout << "\nAfter partition:\n";
    //for (const auto& obj : objects) {
    //    std::cout << "ID: " << obj.id << ", Enabled: " << obj.enabled << "\n";
    //}
    //
    //// You can also get the count of enabled objects
    //size_t enabled_count = std::distance(objects.begin(), partition_point);
    //std::cout << "\nNumber of enabled objects: " << enabled_count << "\n";
    //
    //for (size_t i = 0; i < enabled_count; i++)
    //{
    //    Object obj = objects[i];
    //    std::cout << "ID: " << obj.id << ", Enabled: " << obj.enabled << "\n";
    //}
    //
    //for (size_t i = enabled_count; i < objects.size(); i++)
    //{
    //    Object obj = objects[i];
    //    std::cout << "ID: " << obj.id << ", Enabled: " << obj.enabled << "\n";
    //}
    //
    //objects.erase(partition_point, objects.end());
    //for (const auto& obj : objects) {
    //    std::cout << "ID: " << obj.id << ", Enabled: " << obj.enabled << "\n";
    //}

    while (!WindowShouldClose())
    {
#ifdef DEBUG
        if (IsKeyPressed(KEY_G))
        {
            for (int i = 0; i < 4; i++)
                TraceLog(LOG_INFO, "Gamepad %i %s", i, IsGamepadAvailable(i) ? "connected" : "disconnected");
        }
#endif

        Scene::Update();
        BeginDrawing();
        ClearBackground(BLACK);
        Scene::Draw();
#ifdef DEBUG
        Scene::DrawDebug();
#endif
        Scene::DrawGui();
        EndDrawing();
    }

    Scene::Unload();
    AppUnload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

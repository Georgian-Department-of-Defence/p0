#include "EditorScene.h"
#include "rlgl.h"
#include "Camera.h"
#include "Meshes.h"
#include "Building.h"
#include <array>
#include <vector>
#include <algorithm>

struct EditorBuilding
{
    Id id;
    Vector3 pos;
    Color color;
    Material material;
    Mesh* mesh;
};

static EditorBuilding* f_selected = nullptr;
static std::vector<EditorBuilding> f_buildings;

static Vector3 f_cursor = Vector3Zeros;
static BuildingType f_type = BUILDING_TD;

void RemoveSelected()
{
    if (f_selected == nullptr) return;

    auto first = std::remove_if(f_buildings.begin(), f_buildings.end(),
        [](const EditorBuilding& building) { return f_selected->id == building.id; });

    f_buildings.erase(first, f_buildings.end());
    f_selected = nullptr;
}

void EditorScene::OnLoad()
{
    //float step = 40.0f;
    //for (float y = -40.0f; y <= 40.0f; y += step)
    //{
    //    for (float x = -80.0f; x <= 80.0f; x += step)
    //    {
    //        EditorBuilding building;
    //        building.id = GenId();
    //        building.pos = { x, y, 0.0f };
    //        building.mesh = g_meshes.bld_td;
    //        building.material = LoadMaterialDefault();
    //        building.color = GRAY;
    //        f_buildings.push_back(building);
    //    }
    //}
}

void EditorScene::OnUnload()
{
}

void EditorScene::OnUpdate()
{
	UpdateCamera();
    Ray ray = GetScreenToWorldRay(GetMousePosition(), *GetCamera());

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        EditorBuilding* building_hit = nullptr;
        for (size_t i = 0; i < f_buildings.size(); i++)
        {
            EditorBuilding& bld = f_buildings[i];

            // Not worth caching mesh bounding boxes since this is an editor
            BoundingBox box = GetMeshBoundingBox(*bld.mesh);
            box.min += bld.pos;
            box.max += bld.pos;

            RayCollision collision = GetRayCollisionBox(ray, box);
            if (collision.hit)
            {
                building_hit = &bld;
                break;
            }
        }

        if (f_selected != nullptr)
            f_selected->color = GRAY;

        f_selected = building_hit;

        if (f_selected != nullptr)
            f_selected->color = GREEN;
    }

    if (IsKeyPressed(KEY_DELETE))
        RemoveSelected();

    Vector3 p1 = { WORLD_MIN_X, WORLD_MAX_Y, 0.0f };
    Vector3 p2 = { WORLD_MIN_X, WORLD_MIN_Y, 0.0f };
    Vector3 p3 = { WORLD_MAX_X, WORLD_MIN_Y, 0.0f };
    Vector3 p4 = { WORLD_MAX_X, WORLD_MAX_Y, 0.0f };
    RayCollision ground_collision = GetRayCollisionQuad(ray, p1, p2, p3, p4);
    if (ground_collision.hit)
        f_cursor = ground_collision.point;

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && ground_collision.hit)
    {
        EditorBuilding building;
        building.id = GenId();
        building.pos = f_cursor;
        building.mesh = BuildingMesh(f_type);
        building.material = LoadMaterialDefault();
        building.color = GRAY;
        f_buildings.push_back(building);
    }

    if (IsKeyPressed(KEY_EIGHT))
    {
        int type = f_type;
        type = (type - 1 + BUILDING_TYPE_COUNT) % BUILDING_TYPE_COUNT;
        f_type = (BuildingType)type;
    }
    if (IsKeyPressed(KEY_NINE))
    {
        int type = f_type;
        ++type %= BUILDING_TYPE_COUNT;
        f_type = (BuildingType)type;
    }

    for (EditorBuilding& bld : f_buildings)
        bld.material.maps[MATERIAL_MAP_DIFFUSE].color = bld.color;
}

void EditorScene::OnDraw()
{
    static Material mat = LoadMaterialDefault();
    mat.maps[MATERIAL_MAP_DIFFUSE].color = SKYBLUE;

    BeginMode3D(*GetCamera());

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    rlPushMatrix();
    rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    rlTranslatef(-50.0f, 0.0f, 0.0f);
    DrawGrid(100, 1.0f);
    rlPopMatrix();

    for (const EditorBuilding& bld : f_buildings)
        DrawMesh(*bld.mesh, bld.material, MatrixTranslate(bld.pos.x, bld.pos.y, bld.pos.z));

    DrawMesh(*BuildingMesh(f_type), mat, MatrixTranslate(f_cursor.x, f_cursor.y, f_cursor.z));

    EndMode3D();
}

void EditorScene::OnDrawDebug()
{
}

void EditorScene::OnDrawGui()
{
}

#include "Building.h"
#include "Meshes.h"
#include "Shaders.h"
#include "rlgl.h"
#include <cassert>

inline float BuildingDurability(BuildingType type)
{
    return 100.0f;
}

inline Color BuildingColor(const Building& building)
{
    Color src = LIGHTGRAY;
    Color dst = DARKGRAY;
    float t = 1.0f - (building.durability / BuildingDurability(building.type));
    return ColorLerp(src, dst, t);
}

Mesh* BuildingMesh(BuildingType type)
{
    Mesh* mesh = nullptr;

    switch (type)
    {
    case BUILDING_TD:
        mesh = g_meshes.bld_td;
        break;

    case BUILDING_BMO:
        mesh = g_meshes.bld_bmo;
        break;

    case BUILDING_CONDO:
        mesh = g_meshes.bld_condo;
        break;
    }

    assert(mesh != nullptr, "Invalid building type");
    return mesh;
}

inline float BuildingRadius(BuildingType type)
{
    float radius = 0.0f;
    switch (type)
    {
    case BUILDING_TD:
        radius = 5.0f;
        break;

    case BUILDING_BMO:
        radius = 2.5f;
        break;

    case BUILDING_CONDO:
        radius = 4.0f;
        break;
    }
    return radius;
}

inline float BuildingHeight(BuildingType type)
{
    float height = 0.0f;
    switch (type)
    {
    case BUILDING_TD:
        height = 50.0f;
        break;

    case BUILDING_BMO:
        height = 25.0f;
        break;

    case BUILDING_CONDO:
        height = 25.0f;
        break;
    }
    return height;
}

void CreateBuilding(Building* building, BuildingType type)
{
    building->type = type;
    building->id = GenId();

    building->radius = BuildingRadius(type);
    building->height = BuildingHeight(type);

    building->durability = BuildingDurability(type);
    building->death_timer = 2.0f;

    building->mesh = BuildingMesh(type);
    building->color = BuildingColor(*building);
}

void DestroyBuilding(Building* building)
{
    building->type = BUILDING_TYPE_COUNT;
    building->id = 0;
}

void UpdateBuilding(Building& building)
{
    assert(building.type != BUILDING_TYPE_COUNT);
    assert(building.id != 0);

    float dt = GetFrameTime();
    if (building.durability <= 0.0f)
    {
        building.death_timer -= dt;
        building.pos.z -= 10.0f * dt;
    }

    building.color = BuildingColor(building);
    building.destroy = building.death_timer <= 0.0f;
}

void DrawBuilding(const Building& building, const Renderer& renderer)
{
    Material& mat = g_materials.lighting;
    mat.maps[MATERIAL_MAP_DIFFUSE].color = building.color;
    DrawMesh(*building.mesh, mat, MatrixTranslate(building.pos.x, building.pos.y, building.pos.z));
}

void DrawBuildingDebug(const Building& building, const Renderer& renderer)
{
#if DEBUG
    Color color = building.debug_collion ? RED : building.color;
    color.a = 128;
    
    //Vector3 p = building.pos;
    //p.z += building.extents.z * 0.5f;
    //DrawCube(p, building.extents.x * 2.0f, building.extents.y * 2.0f, building.extents.z, color);

    //DrawSphere(building.pos, building.radius, color);

    // Buildings switched to radius + height, might need to revert to capsules for sideways colliders.
    // Building a map editor would be kind of cool long-term!
    //
    //Vector3 bot = building.pos;
    //Vector3 top = building.pos + Vector3UnitZ * (building.length - building.radius);
    //DrawCapsule(bot, top, building.radius, 8, 4, color);
    // 
    // top & bot spheres vs capsule test
    //DrawCapsule(bot, top, building.radius, 8, 4, { 200, 122, 255, 128 });
    //DrawSphere(top, building.radius, { 0, 82, 172, 128 });
    //DrawSphere(bot, building.radius, { 0, 82, 172, 128 });
#endif
}

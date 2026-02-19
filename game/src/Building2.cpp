#include "Building2.h"

Building2 MakeBuilding(Vector3 position, BuildingType type)
{
    Building2 b;
    b.building_type = type;
    b.entity_type = ENTITY_BUILDING;
    b.team = TEAM_NONE;

    b.collision_type_mask = ENTITY_MASK_ALL;
    b.collision_team_mask = TARGET_MASK_ALL;

    b.durability_max = 50.0f;
    b.durability = b.durability_max;

    switch (type)
    {
    case BUILDING_TD:
        b.mesh = &assets.mesh.td;
        b.collider = MakeCapsule(position, position + Vector3UnitZ * 50.0f, 5.0f);
        break;

    case BUILDING_BMO:
        b.mesh = &assets.mesh.bmo;
        b.collider = MakeCapsule(position, position + Vector3UnitZ * 22.5f, 2.5f);
        break;

    case BUILDING_CONDO:
        b.mesh = &assets.mesh.condo;
        b.collider = MakeCapsule(position, position + Vector3UnitZ * 25.0f, 4.0f);
        break;
    }

    b.pos = position;
    b.collider_offset = b.collider.pos - b.pos;

    return b;
}

void BuildingUpdate(Building2& building)
{
    float dt = GetFrameTime();
    if (building.durability <= 0.0f)
    {
        building.death_timer += dt;
        building.pos.z -= 10.0f * dt;
    }

    building.color = ColorLerp(DARKGRAY, WHITE, building.durability / building.durability_max);
    building.destroy_flag = building.death_timer >= 2.0f;

    building.collider.pos = building.pos + building.collider_offset;
}

void BuildingDraw(const Building2& building, Material material)
{
	material.maps[MATERIAL_MAP_DIFFUSE].color = building.color;
	material.maps[MATERIAL_MAP_DIFFUSE].texture = assets.texture.white;
	DrawMesh(*building.mesh, material, MatrixTranslate(building.pos.x, building.pos.y, building.pos.z));
}

void Building2::OnCollisionPre(Entity* entity)
{
    // Might be easier to remove this and add it to all projectile OnCollision (in case some projectiles have custom damage rules)
    if (entity->entity_type == ENTITY_PROJECTILE)
    {
        Projectile2* p = static_cast<Projectile2*>(entity);
        durability -= p->damage;
    }
}

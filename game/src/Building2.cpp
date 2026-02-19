#include "Building2.h"

Building2 MakeBuilding(BuildingType type)
{
    Building2 b;
    return b;
}

void BuildingUpdate(Building2& building)
{
    float dt = GetFrameTime();
    if (building.durability <= 0.0f)
    {
        building.death_timer -= dt;
        building.pos.z -= 10.0f * dt;
    }

    building.color = ColorLerp(DARKGRAY, WHITE, building.durability / building.durability_max);
    building.destroy_flag = building.death_timer <= 0.0f;

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
    if (entity->type == ENTITY_PROJECTILE)
    {
        Projectile2* p = static_cast<Projectile2*>(entity);
        durability -= p->damage;
    }
}

#pragma once

struct World2
{
	std::vector<Mech2> mechs;
	std::vector<Building2> buildings;
	std::vector<Projectile2*> projectiles;
	std::vector<Light> lights;
};

using EntityCommand = void(*)(World2& world, Entity* e);
void WorldForEach(World2& world, EntityCommand cmd);

void WorldLoad(World2& world);
void WorldUnload(World2& world);
void WorldUpdate(World2& world);
void WorldDraw(const World2& world);

BoundingBox WorldGetBoundingBox(const World2& world);

std::vector<Entity*> WorldGetEntities(const World2& world);
std::vector<Entity*> WorldGetStaticEntities(const World2& world);
std::vector<Entity*> WorldGetDynamicEntities(const World2& world);

void WorldCheckCollisions(const World2& world, std::vector<EntityHit>* hits);
void WorldResolveCollisions(World2& world, std::vector<EntityHit> hits);
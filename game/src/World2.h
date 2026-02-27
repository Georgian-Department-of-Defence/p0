#pragma once

struct World2
{
	std::vector<Mech2> mechs;
	std::vector<Building2> buildings;
	std::vector<Projectile2*> projectiles;
	std::vector<Light> lights;
};

using EntityCommand = void(*)(World2& world, Entity* e);
using EntityCommandConst = void(*)(const World2& world, const Entity* e);
void WorldForEach(World2& world, EntityCommand cmd);
void WorldForEachConst(const World2& world, EntityCommandConst cmd);

void WorldLoad(World2& world);
void WorldUnload(World2& world);
void WorldUpdate(World2& world);
void WorldDraw(const World2& world);

void WorldCollisionDebugPre(World2& world);
void WorldCheckCollisions(const World2& world, std::vector<EntityHit>* hits);
void WorldResolveCollisions(World2& world, std::vector<EntityHit> hits);
void WorldCollisionDebugPost(World2& world, std::vector<EntityHit> hits);


BoundingBox WorldGetBoundingBox(const World2& world);
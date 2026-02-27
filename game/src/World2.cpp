#include "World2.h"
#include "WorldInit.cpp"
#include "WorldCollision.cpp"
#include "WorldUpdate.cpp"
#include "WorldDraw.cpp"

BoundingBox WorldGetBoundingBox(const World2& world)
{
	BoundingBox box;
	box.min = WORLD_MIN;
	box.max = WORLD_MAX;
	return box;
}

std::vector<Entity*> WorldGetEntities(const World2& world)
{
	size_t i = 0;
	std::vector<Entity*> entities;
	entities.resize(world.mechs.size() + world.buildings.size() + world.projectiles.size());

	for (const Mech2& mech : world.mechs)
	{
		entities[i] = (Entity*)&mech;
		i++;
	}

	for (const Building2& building : world.buildings)
	{
		entities[i] = (Entity*)&building;
		i++;
	}

	for (const Projectile2* projectile : world.projectiles)
	{
		entities[i] = (Entity*)projectile;
		i++;
	}

	return entities;
}

std::vector<Entity*> WorldGetStaticEntities(const World2& world)
{
	size_t i = 0;
	std::vector<Entity*> entities;
	entities.resize(world.buildings.size());

	for (const Building2& building : world.buildings)
	{
		entities[i] = (Entity*)&building;
		i++;
	}

	return entities;
}

std::vector<Entity*> WorldGetDynamicEntities(const World2& world)
{
	size_t i = 0;
	std::vector<Entity*> entities;
	entities.resize(world.mechs.size() + world.projectiles.size());

	for (const Mech2& mech : world.mechs)
	{
		entities[i] = (Entity*)&mech;
		i++;
	}

	for (const Projectile2* projectile : world.projectiles)
	{
		entities[i] = (Entity*)projectile;
		i++;
	}

	return entities;
}

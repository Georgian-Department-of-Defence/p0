#include "World2.h"
#include "WorldInit.cpp"
#include "WorldCollision.cpp"
#include "WorldUpdate.cpp"
#include "WorldDraw.cpp"

void WorldForEach(World2& world, EntityCommand cmd)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
	{
		Entity* e = (Entity*)&world.mechs[i];
		cmd(world, e);
	}

	for (size_t i = 0; i < world.buildings.size(); i++)
	{
		Entity* e = (Entity*)&world.buildings[i];
		cmd(world, e);
	}

	for (size_t i = 0; i < world.projectiles.size(); i++)
	{
		Entity* e = (Entity*)world.projectiles[i];
		cmd(world, e);
	}
}

void WorldForEachConst(const World2& world, EntityCommandConst cmd)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
	{
		const Entity* e = (const Entity*)&world.mechs[i];
		cmd(world, e);
	}

	for (size_t i = 0; i < world.buildings.size(); i++)
	{
		const Entity* e = (const Entity*)&world.buildings[i];
		cmd(world, e);
	}

	for (size_t i = 0; i < world.projectiles.size(); i++)
	{
		const Entity* e = (const Entity*)world.projectiles[i];
		cmd(world, e);
	}
}

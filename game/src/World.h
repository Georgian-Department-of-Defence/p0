#pragma once
#include "raylib.h"
#include "raymathext.h"

#include "Constants.h"
#include "Mech.h"
#include "Building.h"
#include "Projectile.h"

#include <vector>


using Mechs = std::vector<Mech>;
using Buildings = std::vector<Building>;
using Projectiles = std::vector<Projectile>;

struct World
{
	Mechs mechs;
	Buildings buildings;
	Projectiles projectiles;
	// Walls walls; // Probably don't need wall planes anymore unless we want things to bounce off walls?
};

void LoadWorld(World& world);
void UnloadWorld(World& world);

void UpdateWorld(World& world);
void DrawWorld(const World& world);
void DrawWorldDebug(const World& world);

inline Mech* GetMechById(uint32_t id, World& world)
{
	for (Mech& mech : world.mechs)
	{
		if (id == mech.id)
			return &mech;
	}
	return nullptr;
}

inline BoundingBox WorldBox()
{
	static BoundingBox world;
	world.min = { WORLD_MIN_X, WORLD_MIN_Y, WORLD_MIN_Z };
	world.max = { WORLD_MAX_X, WORLD_MAX_Y, WORLD_MAX_Z };
	return world;
}

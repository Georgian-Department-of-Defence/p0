#pragma once
#include "WorldDef.h"

struct Building
{
	BuildingType type = BUILDING_TYPE_COUNT;

	Vector3 pos = Vector3Zeros;

	float radius = 0.0f;
	float height = 0.0f;

	//Vector3 extents = Vector3Zeros;
	// Note: only xy should be used in rect collisions
	// (Currently extents.z = full height but 3d box collisions use extents.z as half height)

	Mesh* mesh;
	Material material;

	float durability = 0.0f;
	float death_timer = 0.0f;

	bool destroy = false;

	OnCollisionMechBuilding on_collision_mech = nullptr;
	OnCollisionProjectileBuilding on_collision_projectile = nullptr;

#ifdef DEBUG
	bool debug_collion = false;
#endif
};

void CreateBuilding(Building* building, BuildingType type);
void DestroyBuilding(Building* building);

void UpdateBuilding(Building& building);
void DrawBuilding(const Building& building);
void DrawBuildingDebug(const Building& building);

#pragma once
#include "WorldDef.h"

struct Building
{
	BuildingType type = BUILDING_TYPE_COUNT;

	Vector3 pos = Vector3Zeros;

	float radius = 0.0f;
	float height = 0.0f;

	Mesh* mesh;
	Material material;

	float durability = 0.0f;
	float death_timer = 0.0f;

	bool destroy = false;

	OnCollisionMechBuilding on_collision_mech = nullptr;
	OnCollisionProjectileBuilding on_collision_projectile = nullptr;

#ifdef DEBUG
	bool debug_collion = false;

	// Only add map editor information in debug
	Id edt_id;
	Color edt_color;
#endif
};

void CreateBuilding(Building* building, BuildingType type);
void DestroyBuilding(Building* building);

void UpdateBuilding(Building& building);
void DrawBuilding(const Building& building);
void DrawBuildingDebug(const Building& building);

Mesh* BuildingMesh(BuildingType type);

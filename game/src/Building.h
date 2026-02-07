#pragma once
#include "WorldDef.h"
#include "Renderer.h"

struct Building
{
	BuildingType type = BUILDING_TYPE_COUNT;
	Id id = 0;

	Vector3 pos = Vector3Zeros;

	float radius = 0.0f;
	float height = 0.0f;

	float durability = 0.0f;
	float death_timer = 0.0f;

	bool destroy = false;

	Mesh* mesh = nullptr;
	Color color = WHITE;

#ifdef DEBUG
	bool debug_collion = false;
#endif
};

void CreateBuilding(Building* building, BuildingType type);
void DestroyBuilding(Building* building);

void UpdateBuilding(Building& building);
void DrawBuilding(const Building& building, Material material, const Renderer& renderer);
void DrawBuildingDebug(const Building& building, const Renderer& renderer);

Mesh* BuildingMesh(BuildingType type);

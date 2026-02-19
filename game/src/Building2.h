#pragma once
struct Building2 : public Entity
{
	BuildingType building_type = BUILDING_TYPE_COUNT;

	float durability = 0.0f;
	float durability_max = 0.0f;

	float death_timer = 0.0f;

	void OnCollisionPre(Entity* entity) final;
};

Building2 MakeBuilding(Vector3 position, BuildingType type);

void BuildingUpdate(Building2& building);
void BuildingDraw(const Building2& building, Material material);
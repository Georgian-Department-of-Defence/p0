#pragma once
struct World2
{
	std::vector<Entity*> entities;
	std::vector<Light> lights;
};

void LoadWorld(World2& world);
void UnloadWorld(World2& world);

void UpdateWorld(World2& world);
void DrawWorld(const World2& world);
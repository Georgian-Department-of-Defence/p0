#pragma once
struct World2
{
	std::vector<Entity*> entities;
};

void LoadWorld(World2& world);
void UnloadWorld(World2& world);
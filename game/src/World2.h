#pragma once

struct World2
{
	std::vector<Mech2> mechs;
	std::vector<Light> lights;
};

void LoadWorld(World2& world);
void UnloadWorld(World2& world);
void UpdateWorld(World2& world);
void DrawWorld(const World2& world);

void MechLoad(size_t index, World2& world);
void MechUnload(size_t index, World2& world);
void MechUpdate(size_t index, World2& world);
void MechDraw(size_t index, Material material, const World2& world);

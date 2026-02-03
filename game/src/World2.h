#pragma once
struct WorldFrame
{
	std::vector<Mech2*> mechs;
	//std::vector<Building*> buildings;
	//std::vector<Projectile*> projectiles;
};

struct World2
{
	WorldFrame frame;
	std::vector<Entity*> entities;
	std::vector<Light> lights;
};

void LoadWorld(World2& world);
void UnloadWorld(World2& world);

void UpdateWorldFrame(World2& world);
void UpdateWorld(World2& world);

void DrawWorld(const World2& world);
#pragma once

struct World2
{
	std::vector<Mech2> mechs;
	std::vector<Building2> buildings;
	std::vector<Projectile2*> projectiles;
	std::vector<Light> lights;
};

void WorldLoad(World2& world);
void WorldUnload(World2& world);
void WorldUpdate(World2& world);
void WorldDraw(const World2& world);

std::vector<Entity*> WorldGetEntities(const World2& world);
void WorldCheckCollisions(const World2& world, std::vector<EntityHit>* hits);
void WorldResolveCollisions(World2& world, std::vector<EntityHit> hits);

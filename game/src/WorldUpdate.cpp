#include "World2.h"

void WorldForEach(World2& world, EntityCommand cmd)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
	{
		Entity* e = (Entity*)&world.mechs[i];
		cmd(world, e);
	}
		
	for (size_t i = 0; i < world.buildings.size(); i++)
	{
		Entity* e = (Entity*)&world.buildings[i];
		cmd(world, e);
	}

	for (size_t i = 0; i < world.projectiles.size(); i++)
	{
		Entity* e = (Entity*)world.projectiles[i];
		cmd(world, e);
	}
}

void WorldUpdate(World2& world)
{
	// 1. Static (per-type) updates
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechUpdate(i, world);

	for (Building2& building : world.buildings)
		BuildingUpdate(building);

	for (Projectile2* p : world.projectiles)
	{
		p->OnUpdate();
		p->destroy_flag |= !CheckCollisionBoxSphere(WorldGetBoundingBox(world), p->pos, 1.0f);
	}

	for (Light& light : world.lights)
		LightUpdateUniforms(light, assets.material.lighting.shader);

	// 2. Collision checks & callbacks
	std::vector<EntityHit> hits;
	WorldCheckCollisions(world, &hits);
	WorldResolveCollisions(world, hits);

	// 3. Destroy callbacks
	WorldForEach(world, [](World2& w, Entity* e)
	{
		if (e->destroy_flag)
			e->OnDestroy(w);
	});

	// 4. Deletion & removal
	std::erase_if(world.mechs, [](Mech2& mech) { return mech.destroy_flag; });
	std::erase_if(world.buildings, [](Building2& building) { return building.destroy_flag; });
	std::erase_if(world.projectiles, [](Projectile2* p)
	{
		bool destroy = p->destroy_flag;
		if (destroy)
			delete p;
		return destroy;
	});
}

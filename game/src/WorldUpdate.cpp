#include "World2.h"

void WorldUpdate(World2& world)
{
	if (IsKeyPressed(KEY_K) && !world.mechs.empty())
		world.mechs.back().destroy_flag |= true;

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

	std::vector<EntityHit> hits;
	WorldCheckCollisions(world, &hits);
	WorldResolveCollisions(world, hits);

	for (Mech2& mech : world.mechs)
	{
		if (mech.destroy_flag)
			mech.OnDestroy(world);
	}

	for (Building2& building : world.buildings)
	{
		if (building.destroy_flag)
			building.OnDestroy(world);
	}

	for (Projectile2* p : world.projectiles)
	{
		if (p->destroy_flag)
			p->OnDestroy(world);
	}

	auto projectile_predicate = [](Projectile2* p)
		{
			bool destroy = p->destroy_flag;
			if (destroy)
				delete p;
			return destroy;
		};

	std::erase_if(world.mechs, [](Mech2& mech) { return mech.destroy_flag; });
	std::erase_if(world.buildings, [](Building2& building) { return building.destroy_flag; });
	std::erase_if(world.projectiles, projectile_predicate);
}

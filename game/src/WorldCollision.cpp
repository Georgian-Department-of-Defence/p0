#include "World2.h"

void WorldCheckCollisions(const World2& world, std::vector<EntityHit>* hits)
{
	// So far, this is the only function that needs to store a collection of all entities (because we need a nested loop)
	// Everything else can get by with WorldForEach

	std::vector<Entity*> entities;
	entities.resize(world.mechs.size() + world.buildings.size() + world.projectiles.size());
	{
		size_t i = 0;

		for (const Mech2& mech : world.mechs)
		{
			entities[i] = (Entity*)&mech;
			i++;
		}

		for (const Building2& building : world.buildings)
		{
			entities[i] = (Entity*)&building;
			i++;
		}

		for (const Projectile2* projectile : world.projectiles)
		{
			entities[i] = (Entity*)projectile;
			i++;
		}
	}

	auto CollisionHelper = [&hits](Entity* a, Entity* b)
	{
		Vector3 mtv = Vector3Zeros;
		if (EntityCheckCollision3D(*a, *b, &mtv))
		{
			mtv.z = 0.0f;

			EntityHit hit;
			hit.a = a;
			hit.b = b;
			hit.mtv = mtv;
			hits->push_back(hit);
		}
	};

	for (size_t i = 0; i < entities.size(); i++)
	{
		for (size_t j = i + 1; j < entities.size(); j++)
		{
			Entity* a = entities[i];
			Entity* b = entities[j];
			CollisionHelper(a, b);
		}
	}
}

void WorldResolveCollisions(World2& world, std::vector<EntityHit> hits)
{
	// Pre-pass to ensure A is *always* dynamic and B is either static or dynamic
	for (EntityHit& hit : hits)
	{
		if (EntityIsMassInfinite(*hit.a))
		{
			Entity* tmp = hit.b;
			hit.b = hit.a;
			hit.a = tmp;
			hit.mtv *= -1.0f;
		}

		// MTV direction sanity-check
		Vector3 dir = hit.a->pos - hit.b->pos;
		float dot = Vector3DotProduct(dir, hit.mtv);
		assert(dot > 0.0f);
	}

	for (const EntityHit& hit : hits)
	{
		hit.a->OnCollisionPre(hit.b);
		hit.b->OnCollisionPre(hit.a);
	}

	for (const EntityHit& hit : hits)
	{
		// *Insert impulse & friction code here*
		// Friction & impulse probably not necessary.
		// Might even make things worse when coupled with type-specific collision logic...

		if (EntityIsMassInfinite(*hit.b))
		{
			hit.a->pos += hit.mtv;
		}
		else
		{
			hit.a->pos += hit.mtv * 0.5f;
			hit.b->pos -= hit.mtv * 0.5f;
		}
	}

	for (const EntityHit& hit : hits)
	{
		hit.a->OnCollisionPost(hit.b);
		hit.b->OnCollisionPost(hit.a);
	}
}

void WorldCollisionDebugPre(World2& world)
{
	WorldForEach(world, [](World2& w, Entity* e)
	{
		e->collider.debug_collision = false;
		e->collider.debug_color = ColorFromNormalized({ 0.0f, 1.0f, 0.0f, 0.75f });
	});
}

void WorldCollisionDebugPost(World2& world, std::vector<EntityHit> hits)
{
	for (EntityHit& hit : hits)
	{
		hit.a->collider.debug_collision = hit.b->collider.debug_collision = true;
		hit.a->collider.debug_color = ColorFromNormalized({ 1.0f, 0.0f, 0.0f, 0.75f });
		hit.b->collider.debug_color = ColorFromNormalized({ 1.0f, 0.0f, 0.0f, 0.75f });
	}
}

BoundingBox WorldGetBoundingBox(const World2& world)
{
	BoundingBox box;
	box.min = WORLD_MIN;
	box.max = WORLD_MAX;
	return box;
}

// Shoot single projectile at single building to test collision:
//Bullet* b = new Bullet;
//b->pos = Vector3UnitY * 0.0f + Vector3UnitZ * 10.0f;
//b->vel = Vector3UnitY * 0.0f;
//
//b->mesh = &assets.mesh.bullet;
//b->color = RED;
//
//b->collider.type = COLLIDER_SPHERE;
//b->collider.sphere.radius = 5.0f;
//
//b->collision_type_mask = ENTITY_MASK_MECH | ENTITY_MASK_BUILDING;
//b->collision_team_mask = TARGET_MASK_ENEMY;
//b->team = TEAM_RED;
//b->type = ENTITY_PROJECTILE;
//
//world.projectiles.push_back(b);

//if (!world.projectiles.empty() && IsKeyPressed(KEY_T))
//{
//	Vector3 mtv = Vector3Zeros;
//	bool result = EntityCheckCollision3D(*world.projectiles.back(), world.buildings.back(), &mtv);
//	if (result)
//		TraceLog(LOG_INFO, "Lit");
//}

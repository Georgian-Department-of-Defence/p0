#include "World2.h"

void WorldCheckCollisions(const World2& world, std::vector<EntityHit>* hits)
{
	std::vector<Entity*> entities = WorldGetEntities(world);
	for (Entity* entity : entities) entity->collider.debug_color = ColorFromNormalized({ 0.0f, 1.0f, 0.0f, 0.75f });

	for (size_t i = 0; i < entities.size(); i++)
	{
		for (size_t j = i + 1; j < entities.size(); j++)
		{
			Vector3 mtv = Vector3Zeros;
			Entity* a = entities[i];
			Entity* b = entities[j];
			if (EntityCheckCollision3D(*a, *b, &mtv))
			{
				a->collider.debug_color = ColorFromNormalized({ 1.0f, 0.0f, 0.0f, 0.75f });
				b->collider.debug_color = ColorFromNormalized({ 1.0f, 0.0f, 0.0f, 0.75f });
				mtv.z = 0.0f;

				EntityHit hit;
				hit.a = a;
				hit.b = b;
				hit.mtv = mtv;
				hits->push_back(hit);
			}
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

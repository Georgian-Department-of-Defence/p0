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
	// Pre-pass to ensure mtvs resolve A from B
	//for (EntityHit& hit : hits)
	//{
	//	Vector2 pA = { hit.a->pos.x, hit.a->pos.y };
	//	Vector2 pB = { hit.b->pos.x, hit.b->pos.y };
	//	Vector2 BA = pA - pB;
	//	if (Vector2DotProduct(BA, hit.mtv) < 0.0f)
	//		hit.mtv *= -1.0f;
	//}
	// Update: easier to swap the direction of MTV in the case of B??

	for (const EntityHit& hit : hits)
	{
		Vector3 mtv_a = hit.mtv * 1.0f;
		Vector3 mtv_b = hit.mtv * -1.0f;
		hit.a->OnCollisionPre(hit.b);
		hit.b->OnCollisionPre(hit.a);
	}

	// *Insert impulse & friction code here*
	// *Insert position code here*

	for (const EntityHit& hit : hits)
	{
		Vector3 mtv_a = hit.mtv * 1.0f;
		Vector3 mtv_b = hit.mtv * -1.0f;
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

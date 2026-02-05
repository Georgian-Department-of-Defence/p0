#include "Entity.h"

bool EntityCheckCollision(Entity* a, Entity* b, HitInfo* hit_info)
{
	assert(a->collider.type != COLLIDER_TYPE_COUNT && b->collider.type != COLLIDER_TYPE_COUNT);
	Vector2 pA = { a->pos.x, a->pos.y };
	Vector2 pB = { b->pos.x, b->pos.y };

	if (a->collider.type == COLLIDER_CIRCLE)
	{
		float rA = a->collider.circle.radius;
		
		if (b->collider.type == COLLIDER_CIRCLE)
		{
			CircleCircle(pA, rA, pB, b->collider.circle.radius, &hit_info->mtv);
		}
		else if (b->collider.type == COLLIDER_CAPSULE)
		{
			Vector3 dir = EntityGetDirection(*b);
			CircleCapsule(pA, rA, pB, { dir.x, dir.y }, b->collider.capsule.radius, b->collider.capsule.half_height, &hit_info->mtv);
		}
		else if (b->collider.type == COLLIDER_PLANE)
		{
			CirclePlane(pA, rA, pB, b->collider.plane.normal, &hit_info->mtv);
		}
		else if (b->collider.type == COLLIDER_BOX)
		{
			CircleRectangle(pA, rA, pB, b->collider.box.extents, &hit_info->mtv);
		}
	}

	if (a->collider.type == COLLIDER_CAPSULE)
	{
		if (b->collider.type == COLLIDER_CIRCLE)
		{

		}
		else if (b->collider.type == COLLIDER_CAPSULE)
		{

		}
		else if (b->collider.type == COLLIDER_PLANE)
		{

		}
		else if (b->collider.type == COLLIDER_BOX)
		{

		}
	}

	if (a->collider.type == COLLIDER_PLANE)
	{
		if (b->collider.type == COLLIDER_CIRCLE)
		{

		}
		else if (b->collider.type == COLLIDER_CAPSULE)
		{

		}
		else if (b->collider.type == COLLIDER_PLANE)
		{

		}
		else if (b->collider.type == COLLIDER_BOX)
		{

		}
	}

	if (a->collider.type == COLLIDER_BOX)
	{
		if (b->collider.type == COLLIDER_CIRCLE)
		{

		}
		else if (b->collider.type == COLLIDER_CAPSULE)
		{

		}
		else if (b->collider.type == COLLIDER_PLANE)
		{

		}
		else if (b->collider.type == COLLIDER_BOX)
		{

		}
	}

	return false;
}

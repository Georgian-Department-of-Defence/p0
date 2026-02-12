#include "Entity.h"

bool EntityCheckCollision3D(const Entity& a, const Entity& b, Vector3* mtv)
{
	bool collision = false;
	assert(mtv != nullptr);
	assert(a.collider.type != COLLIDER_TYPE_COUNT && b.collider.type != COLLIDER_TYPE_COUNT);
	{
		bool can_types_collide = a.collision_type_mask & b.collision_type_mask;
		bool can_teams_collide = false;
		
		auto can_target = [](const Entity& src, const Entity& dst) 
		{
			bool result = false;
			result |= (src.collision_team_mask & TARGET_MASK_ALLY) && src.team == dst.team;
			result |= (src.collision_team_mask & TARGET_MASK_ENEMY) && src.team != dst.team;
			return result;
		};

		can_teams_collide = can_target(a, b) && can_target(b, a);
		if (!(can_types_collide && can_teams_collide)) return false;
	}

	Vector3 pA = a.collider.pos;
	Vector3 pB = b.collider.pos;

	// TODO -- Add capsule checks
	if (a.collider.type == COLLIDER_SPHERE)
	{
		Sphere cA = a.collider.sphere;
		if (b.collider.type == COLLIDER_SPHERE)
		{
			Sphere cB = b.collider.sphere;
			collision = SphereSphere(pA, cA.radius, pB, cB.radius, mtv);
		}
		else if (b.collider.type == COLLIDER_CAPSULE)
		{
			Capsule cB = b.collider.capsule;
			collision = SphereCapsule(pA, cA.radius, pB, cB.direction, cB.radius, cB.half_height, mtv);
		}
		else if (b.collider.type == COLLIDER_BOX)
		{
			Box cB = b.collider.box;
			collision = SphereBox(pA, cA.radius, pB, cB.extents, mtv);
		}
		else if (b.collider.type == COLLIDER_PLANE)
		{
			Plane cB = b.collider.plane;
			collision = SpherePlane(pA, cA.radius, pB, cB.normal, mtv);
		}
	}

	if (a.collider.type == COLLIDER_CAPSULE)
	{
		Capsule cA = a.collider.capsule;
		if (b.collider.type == COLLIDER_SPHERE)
		{
			Sphere cB = b.collider.sphere;
			collision = SphereCapsule(pB, cB.radius, pA, cA.direction, cA.radius, cA.half_height, mtv);
			*mtv *= -1.0f;
		}
		else if (b.collider.type == COLLIDER_CAPSULE)
		{
			Capsule cB = b.collider.capsule;
			collision = CapsuleCapsule(pA, cA.direction, cA.radius, cA.half_height, pB, cB.direction, cB.radius, cB.half_height, mtv);
		}
		else if (b.collider.type == COLLIDER_BOX)
		{
			Box cB = b.collider.box;
			CapsuleBox(pA, cA.direction, cA.radius, cA.half_height, pB, cB.extents, mtv);
		}
		else if (b.collider.type == COLLIDER_PLANE)
		{
			Plane cB = b.collider.plane;
			CapsulePlane(pA, cA.direction, cA.radius, cA.half_height, pB, cB.normal, mtv);
		}
	}

	// Only circles & capsules supported currently
	//if (a.collider.type == COLLIDER_PLANE)
	//{
	//	if (b.collider.type == COLLIDER_CIRCLE)
	//	{
	//
	//	}
	//	else if (b.collider.type == COLLIDER_CAPSULE)
	//	{
	//
	//	}
	//	else if (b.collider.type == COLLIDER_PLANE)
	//	{
	//
	//	}
	//	else if (b.collider.type == COLLIDER_BOX)
	//	{
	//
	//	}
	//}
	//
	//if (a.collider.type == COLLIDER_BOX)
	//{
	//	if (b.collider.type == COLLIDER_CIRCLE)
	//	{
	//
	//	}
	//	else if (b.collider.type == COLLIDER_CAPSULE)
	//	{
	//
	//	}
	//	else if (b.collider.type == COLLIDER_PLANE)
	//	{
	//
	//	}
	//	else if (b.collider.type == COLLIDER_BOX)
	//	{
	//
	//	}
	//}

	return collision;
}

// TODO -- Switch to 3D collision so I don't have to convert from Vector2 to Vector3 everywhere!?!?!?
// Simply set mtv.z to 0.0 and move on with my life??
/*
bool EntityCheckCollision2D(const Entity& a, const Entity& b, Vector2* mtv)
{
	assert(a.collider.type != COLLIDER_TYPE_COUNT && b.collider.type != COLLIDER_TYPE_COUNT);
	Vector2 pA = { a.pos.x, a.pos.y };
	Vector2 pB = { b.pos.x, b.pos.y };

	if (a.collider.type == COLLIDER_CIRCLE)
	{
		float rA = a.collider.circle.radius;
		
		if (b.collider.type == COLLIDER_CIRCLE)
		{
			CircleCircle(pA, rA, pB, b.collider.circle.radius, mtv);
		}
		else if (b.collider.type == COLLIDER_CAPSULE)
		{
			Vector2 dir = b.collider.capsule.direction;
			CircleCapsule(pA, rA, pB, dir, b.collider.capsule.radius, b.collider.capsule.half_height, mtv);
		}
		else if (b.collider.type == COLLIDER_PLANE)
		{
			CirclePlane(pA, rA, pB, b.collider.plane.normal, mtv);
		}
		else if (b.collider.type == COLLIDER_BOX)
		{
			CircleRectangle(pA, rA, pB, b.collider.box.extents, mtv);
		}
	}

	if (a.collider.type == COLLIDER_CAPSULE)
	{
		if (b.collider.type == COLLIDER_CIRCLE)
		{

		}
		else if (b.collider.type == COLLIDER_CAPSULE)
		{

		}
		else if (b.collider.type == COLLIDER_PLANE)
		{

		}
		else if (b.collider.type == COLLIDER_BOX)
		{

		}
	}

	if (a.collider.type == COLLIDER_PLANE)
	{
		if (b.collider.type == COLLIDER_CIRCLE)
		{

		}
		else if (b.collider.type == COLLIDER_CAPSULE)
		{

		}
		else if (b.collider.type == COLLIDER_PLANE)
		{

		}
		else if (b.collider.type == COLLIDER_BOX)
		{

		}
	}

	if (a.collider.type == COLLIDER_BOX)
	{
		if (b.collider.type == COLLIDER_CIRCLE)
		{

		}
		else if (b.collider.type == COLLIDER_CAPSULE)
		{

		}
		else if (b.collider.type == COLLIDER_PLANE)
		{

		}
		else if (b.collider.type == COLLIDER_BOX)
		{

		}
	}

	return false;
}
*/
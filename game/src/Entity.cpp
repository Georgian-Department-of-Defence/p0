#include "Entity.h"

bool EntityCheckCollision3D(const Entity& a, const Entity& b, Vector3* mtv)
{
	assert(a.collider.type != COLLIDER_TYPE_COUNT && b.collider.type != COLLIDER_TYPE_COUNT);
	if (a.collider.type == COLLIDER_SPHERE)
	{
		float rA = a.collider.sphere.radius;

		if (b.collider.type == COLLIDER_SPHERE)
		{
			SphereSphere(a.pos, rA, b.pos, b.collider.sphere.radius, mtv);
		}
		else if (b.collider.type == COLLIDER_SPHEROCYLINDER)
		{
			SphereCapsule(a.pos, rA, b.pos, b.collider.spherocylinder.direction, b.collider.spherocylinder.radius, b.collider.spherocylinder.half_height, mtv);
		}
		else if (b.collider.type == COLLIDER_PLANE)
		{
			SpherePlane(a.pos, rA, b.pos, b.collider.plane.normal, mtv);
		}
		else if (b.collider.type == COLLIDER_BOX)
		{
			SphereBox(a.pos, rA, b.pos, b.collider.box.extents, mtv);
		}
	}

	//if (a.collider.type == COLLIDER_CAPSULE)
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

	return false;
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
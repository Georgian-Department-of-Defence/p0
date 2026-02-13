#include <Collider3D.h>

CollisionFunction collision_table[4][4] =
{
	// SPHERE,           CAPSULE,              BOX,         PLANE
	{ SphereSphere,   SphereCapsule,    SphereBox,   SpherePlane},   // SPHERE
	{ CapsuleSphere,  CapsuleCapsule,   CapsuleBox,  CapsulePlane},  // CAPSULE
	{ BoxSphere,      BoxCapsule,       BoxBox,      BoxPlane},      // BOX
	{ PlaneSphere,    PlaneCapsule,     PlaneBox,    PlanePlane}     // PLANE
};

bool SphereSphere(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_SPHERE && b.type == COLLIDER_SPHERE);
	return SphereSphere(a.pos, a.sphere.radius, b.pos, b.sphere.radius, mtv);
}

bool SphereCapsule(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_SPHERE && b.type == COLLIDER_CAPSULE);
	return SphereCapsule(a.pos, a.sphere.radius, b.pos, b.capsule.direction, b.capsule.radius, b.capsule.half_height, mtv);
}

bool SphereBox(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_SPHERE && b.type == COLLIDER_BOX);
	return SphereBox(a.pos, a.sphere.radius, b.pos, b.box.extents, mtv);
}

bool SpherePlane(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_SPHERE && b.type == COLLIDER_PLANE);
	return SpherePlane(a.pos, a.sphere.radius, b.pos, b.plane.normal, mtv);
}

bool CapsuleSphere(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_CAPSULE && b.type == COLLIDER_SPHERE);
	bool collision = SphereCapsule(b.pos, b.sphere.radius, a.pos, a.capsule.direction, a.capsule.radius, a.capsule.half_height, mtv); 
	*mtv *= -1.0f;
	return collision;
}

bool CapsuleCapsule(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_CAPSULE && b.type == COLLIDER_CAPSULE);
	return CapsuleCapsule(a.pos, a.capsule.direction, a.capsule.radius, a.capsule.half_height, b.pos, b.capsule.direction, b.capsule.radius, b.capsule.half_height, mtv);
}

bool CapsuleBox(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_CAPSULE && b.type == COLLIDER_BOX);
	return CapsuleBox(a.pos, a.capsule.direction, a.capsule.radius, a.capsule.half_height, b.pos, b.box.extents, mtv);
}

bool CapsulePlane(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_CAPSULE && b.type == COLLIDER_PLANE);
	return CapsulePlane(a.pos, a.capsule.direction, a.capsule.radius, a.capsule.half_height, b.pos, b.plane.normal, mtv);
}

bool BoxSphere(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_BOX && b.type == COLLIDER_SPHERE);
	assert(false); // Unimplemented
	return false;
}

bool BoxCapsule(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_BOX && b.type == COLLIDER_CAPSULE);
	assert(false); // Unimplemented
	return false;
}

bool BoxBox(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_BOX && b.type == COLLIDER_BOX);
	assert(false); // Unimplemented
	return false;
}

bool BoxPlane(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_BOX && b.type == COLLIDER_PLANE);
	assert(false); // Unimplemented
	return false;
}

bool PlaneSphere(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_PLANE && b.type == COLLIDER_SPHERE);
	assert(false); // Unimplemented
	return false;
}

bool PlaneCapsule(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_PLANE && b.type == COLLIDER_CAPSULE);
	assert(false); // Unimplemented
	return false;
}

bool PlaneBox(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(a.type == COLLIDER_PLANE && b.type == COLLIDER_BOX);
	assert(false); // Unimplemented
	return false;
}

bool PlanePlane(const Collider& a, const Collider& b, Vector3* mtv)
{
	assert(false); // Shouldn't be possible
	return false;
}

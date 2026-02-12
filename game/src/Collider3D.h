#pragma once

struct Sphere
{
	float radius;
};

struct Capsule
{
	float radius;
	float half_height;
	Vector3 direction;
};

struct Box
{
	Vector3 extents;
};

struct Plane
{
	Vector3 normal;
};

struct Collider3D
{
	ColliderType3D type = COLLIDER_TYPE_COUNT;

	union
	{
		Sphere sphere;
		Capsule capsule;
		Box box;
		Plane plane;
	};

	Vector3 pos = Vector3Zeros;
	Color debug_color = MAGENTA;
};

using Collider = Collider3D;

inline Collider MakeCapsule(Vector3 a/*bottom*/, Vector3 b/*top*/, float r)
{
	Collider collider;
	collider.type = COLLIDER_CAPSULE;
	collider.pos = (a + b) * 0.5f;
	collider.capsule.half_height = Vector3Length(b - collider.pos) - r;
	collider.capsule.direction = Vector3Normalize(b - a);
	collider.capsule.radius = r;
	return collider;
}

// raylib DrawCapsule is takes in top & bottom of the cylinder rather than the end-points of the actual capsule...
// p0 API = "Draw from 0.0 to 16.0", matching raylib API = "radius of 2, so start at [0, 0, 2] and go till [0, 0, 14]" --> 0..16
/*
DrawCapsule(Vector3UnitZ * 2.0f + Vector3UnitX * 2.0f, Vector3UnitZ * 14.0f + Vector3UnitX * 2.0f, 2.0f, 8, 8, BLUE);
Collider cap = MakeCapsule(Vector3Zeros, Vector3UnitZ * 16.0f, 2.0f);
DrawCollider(cap, MAGENTA);
*/
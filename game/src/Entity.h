#pragma once
#include "raylib.h"
#include "Collision.h"
#include "WorldDef.h"

enum ColliderType : uint32_t
{
	COLLIDER_CIRCLE,
	COLLIDER_CAPSULE,
	COLLIDER_PLANE,
	COLLIDER_BOX,
	COLLIDER_TYPE_COUNT
};

struct Collider
{
	ColliderType type = COLLIDER_TYPE_COUNT;

	union
	{
		struct
		{
			float radius;
		} circle;

		struct
		{
			float radius;
			float half_height;
		} capsule;

		struct
		{
			Vector2 extents;
		} box;

		struct
		{
			Vector2 normal;
		} plane;
	};
};

struct Entity
{
	EntityType type = ENTITY_TYPE_COUNT;
	bool destroy_flag = false;

	Vector3 pos;
	Vector3 vel;
	Vector3 acc;

	Quaternion rot;

	Collider collider;

	virtual void OnLoad() {};
	virtual void OnUnload() {};
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnCollision(Entity& entity, HitInfo hit_info) = 0;
};

bool CheckCollision(const Entity& a, const Entity& b, HitInfo* hit_info);

// My collision functions are procedural so stick to unions for colliders.
// Adding an interface would just make things confusing

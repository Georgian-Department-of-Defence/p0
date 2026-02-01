#pragma once

struct Entity
{
	virtual void OnLoad() {};
	virtual void OnUnload() {};
	virtual void OnUpdate() = 0;
	virtual void OnDraw() const = 0;
	virtual void OnCollision(const Entity& entity, HitInfo hit_info) = 0;

	EntityType type = ENTITY_TYPE_COUNT;
	bool destroy_flag = false;

	Vector3 pos = Vector3Zeros;
	Vector3 vel = Vector3Zeros;
	Vector3 acc = Vector3Zeros;
	Quaternion rot = QuaternionIdentity();
	// Used to set direction of colliders

	Collider collider;
};

inline Vector3 EntityDirection(const Entity& entity)
{
	return Vector3RotateByQuaternion(Vector3UnitY, entity.rot);
}

bool EntityCheckCollision(const Entity& a, const Entity& b, HitInfo* hit_info);

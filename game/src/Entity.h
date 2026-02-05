#pragma once

struct Entity;
using Id = uint32_t;
using OnCollision = void(*)(Entity* a, Entity* b, HitInfo hit_info);

struct Entity
{
	Id id = 0;
	EntityType type = ENTITY_TYPE_COUNT;
	Team team = TEAM_NONE;
	bool destroy_flag = false;

	Vector3 pos = Vector3Zeros;
	Vector3 vel = Vector3Zeros;
	Vector3 acc = Vector3Zeros;
	Quaternion rot = QuaternionIdentity();

	Collider collider;
	OnCollision on_collision = nullptr;

	Mesh* mesh = nullptr;
	Color color = WHITE;
	ParticleEmitter emitter;
};

inline Id EntityGenId()
{
	static Id id = 0;
	return ++id;
}

inline Vector3 EntityGetDirection(const Entity& entity)
{
	return Vector3RotateByQuaternion(Vector3UnitY, entity.rot);
}

bool EntityCheckCollision(Entity* a, Entity* b, HitInfo* hit_info);

#pragma once

using Id = uint32_t;
struct World2;
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

	float gravity_scale = 0.0f;
	float mass_inverse = 0.0f;
	float restitution = 0.0f;
	float friction = 0.0f;
	Collider collider;
	virtual void OnCollisionPre(Entity* entity) { }
	virtual void OnCollisionPost(Entity* entity) { }

	Mesh* mesh = nullptr;
	Color color = WHITE;
	ParticleEmitter emitter;
};

struct EntityHit
{
	Entity* a = nullptr;
	Entity* b = nullptr;
	Vector2 mtv = Vector2Zeros;
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

bool EntityCheckCollision(const Entity& a, const Entity& b, Vector2* mtv);

// Collision notes:
// 1) mtv not necessary. Easier if mtv is used during physics resolution between on_pre & on_post.
// 2) world not necessary unless we need to spawn entities on-collision (currently nothing, but particles in the future)?
// 3) using virtual methods instead of function pointers because v-table will resolve entity_self
// (otherwise, A LOT of if-statements needed to figure out who is colliding with who, and potentially swapping A & B)

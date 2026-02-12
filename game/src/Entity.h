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
	Matrix rot = MatrixIdentity();

	float gravity_scale = 0.0f;
	float mass_inverse = 0.0f;
	float restitution = 0.0f;
	float friction = 0.0f;
	Collider collider;
	Vector3 collider_offset = Vector3Zeros;
	virtual void OnCollisionPre(Entity* entity) { }
	virtual void OnCollisionPost(Entity* entity) { }
	virtual void OnDestroy(World2& world) { }
	size_t collision_type_mask = 0;	// Collides with Mech/Building/Projectile
	size_t collision_team_mask = 0;	// Collides with Ally/Enemy/All

	Mesh* mesh = nullptr;
	Color color = WHITE;
	ParticleEmitter emitter;
	std::string name;
};

struct EntityHit3D
{
	Entity* a = nullptr;
	Entity* b = nullptr;
	Vector3 mtv = Vector3Zeros;
};

using EntityHit = EntityHit3D;

inline Id EntityGenId()
{
	static Id id = 0;
	return ++id;
}

// Misleading because this is only 2d.
//inline Vector3 EntityGetDirection(const Entity& entity)
//{
//	return Vector3RotateByQuaternion(Vector3UnitY, entity.rot);
//}

inline Vector3 EntityGetDirection(const Entity& entity)
{
	return MatrixColY(entity.rot);
}

bool EntityCheckCollision3D(const Entity& a, const Entity& b, Vector3* mtv);
//bool EntityCheckCollision2D(const Entity& a, const Entity& b, Vector2* mtv);
//struct EntityHit2D
//{
//	Entity* a = nullptr;
//	Entity* b = nullptr;
//	Vector2 mtv = Vector2Zeros;
//};

// Collision notes:
// 1) mtv not necessary. Easier if mtv is used during physics resolution between on_pre & on_post.
// 2) world not necessary unless we need to spawn entities on-collision (currently nothing, but particles in the future)?
// 3) using virtual methods instead of function pointers because v-table will resolve entity_self
// (otherwise, A LOT of if-statements needed to figure out who is colliding with who, and potentially swapping A & B)

#pragma once
#include "WorldDef.h"
#include "ParticleEmitter.h"
#include "Renderer.h"

struct ProjectileStraight
{
	float speed;
	float lifetime;
};

struct ProjectileGrenade
{
};

struct ProjectileMissile
{
	MissileState state;
	float move_speed;
	float turn_speed;

	float time;

	Vector3 target_position;	// Where to seek, state-dependent
	Vector3 launch_position;
	Vector3 launch_direction;	// Mech's torso direction at the time of launch
};

struct ProjectileMachineGun
{
	float speed;
	float lifetime;
};

struct ProjectileChainGun
{
	float speed;
	float lifetime;
};

struct Projectile
{
	ProjectileType type = PROJECTILE_TYPE_COUNT;
	Team team = TEAM_NONE;

	Vector3 pos = Vector3Zeros;
	Vector3 vel = Vector3Zeros;
	Vector3 acc = Vector3Zeros;

	float radius = 0.0f;
	float length = 0.0f;

	Color color = WHITE;
	Mesh* mesh = nullptr;

	float gravity_scale = 0.0f;
	float restitution = 0.0f;
	bool destroy = false;

	float damage = 0.0f;

	uint32_t owner_mech_id = 0;
	uint32_t target_mech_id = 0;

	bool mech_hit = false;
	// TODO - Add flags for none vs mech vs building hit??

	OnCollisionMechProjectile on_collision_mech = nullptr;
	OnCollisionProjectileBuilding on_collision_building = nullptr;

	ParticleEmitter trail;

#ifdef DEBUG
	bool debug_collion = false;
#endif

	union
	{
		ProjectileStraight straight;
		ProjectileGrenade grenade;
		ProjectileMissile missile;
		ProjectileMachineGun machineGun;
		ProjectileChainGun chainGun;

	};
};

void DestroyProjectile(Projectile* p);
void CreateProjectileRifle(Mech& mech, World& world, Vector3 base_pos);
void CreateProjectileShotgun(Mech& mech, World& world, Vector3 base_pos);
void CreateProjectileGrenade(Mech& mech, World& world, Vector3 base_pos);
void CreateProjectileMissile(Mech& mech, World& world, Vector3 base_pos, float roll);
void CreateProjectileMachineGun(Mech& mech, World& world, Vector3 base_pos);
void CreateProjectileChainGun(Mech& mech, World& world, Vector3 base_pos);

void UpdateProjectile(Projectile& p, World& world);
void DrawProjectile(const Projectile& p, Material material, const Renderer& renderer);
void DrawProjectileDebug(const Projectile& p, const Renderer& renderer);
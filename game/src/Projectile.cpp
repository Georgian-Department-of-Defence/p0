#include "Projectile.h"
#include "Meshes.h"
#include "Camera.h"
#include <cassert>
#include "DebugDraw.h"

#include "Mech.h"
#include "World.h"
#include "Audio.h"
#include "Steering.h"

constexpr float MISSILE_MAX_HEIGHT = 30.0f;
constexpr float MISSILE_SEEK_SPEED = 50.0f;

void UpdateProjectileMissile(Projectile& p, World& world);

void CreateParticleTrail(Projectile* p)
{
	// TODO - Make weapon-specifc particle effects -- ie increase particles on rifle, decrease particles on grenade (1 projectile vs 6 projectiles)
	// (Don't bother with this until gear physics [velocity] are finalized [if projectile velocity changes, particle effects must be updated accordingly)
	ParticleEmitter& pe = p->trail;
	pe.spawn_rate = 10.0f;
	pe.lifetime = 1.0f;
	pe.speed = 30.0f;
	pe.size = 1.0f;
	pe.color_src = p->color;
	pe.color_dst = ColorLerp(p->color, BLACK, 0.6f);
	pe.shape_type = PARTICLE_SHAPE_SPHERE;
	pe.shape.sphere.radius = 2.0f;
}

void DestroyProjectile(Projectile* p)
{
	p->mesh = nullptr;
	p->type = PROJECTILE_TYPE_COUNT;
	UnloadMaterial(p->material);

	DestroyParticleEmitter(&p->trail);
}

void CreateProjectileRifle(Mech& mech, World& world, Vector3 base_pos)
{
	Projectile p;
	p.pos = base_pos;
	p.vel = TorsoDirection(mech) * 75.0f;
	p.radius = 1.5f;
	p.team = mech.team;
	p.type = PROJECTILE_RIFLE;

	p.color = RED;
	p.mesh = g_meshes.prj_straight;
	p.material = LoadMaterialDefault();

	CreateParticleTrail(&p);

	world.projectiles.push_back(p);
	PlaySound(g_audio.fire_rifle);
}

void CreateProjectileShotgun(Mech& mech, World& world, Vector3 base_pos)
{
	Projectile projectiles[3];
	for (int i = 0; i < 3; i++)
	{
		Projectile& p = projectiles[i];
		Quaternion spread = QuaternionFromEuler(0.0f, 0.0f, (-30.0f + i * 30.0f) * DEG2RAD);
		Vector3 dir = Vector3RotateByQuaternion(TorsoDirection(mech), spread);

		p.pos = base_pos;
		p.vel = dir * 60.0f;
		p.radius = 2.0f;
		p.team = mech.team;
		p.type = PROJECTILE_SHOTGUN;

		p.color = GREEN;
		p.mesh = g_meshes.prj_straight;
		p.material = LoadMaterialDefault();

		CreateParticleTrail(&p);

		world.projectiles.push_back(p);
	}
	PlaySound(g_audio.fire_shotgun);
}

void CreateProjectileGrenade(Mech& mech, World& world, Vector3 base_pos)
{
	float pitch = (60.0f + Random(-15.0f, 15.0f)) * DEG2RAD;
	float roll = Random(-30.0f, 30.0f) * DEG2RAD;
	Vector3 dir = Vector3RotateByQuaternion(Vector3UnitY, QuaternionMultiply(mech.torso_rotation, QuaternionFromEuler(pitch, 0.0f, roll)));

	Projectile p;
	p.pos = base_pos;
	p.vel = dir * 50.0f;
	p.radius = 2.0f;
	p.gravity_scale = 6.0f;
	p.team = mech.team;
	p.type = PROJECTILE_GRENADE;

	p.color = BLUE;
	p.mesh = g_meshes.prj_grenade;
	p.material = LoadMaterialDefault();

	CreateParticleTrail(&p);

	world.projectiles.push_back(p);
	PlaySound(g_audio.fire_grenade);
}

void CreateProjectileMissile(Mech& mech, World& world, Vector3 base_pos, float roll)
{
	Vector3 dir = Vector3RotateByQuaternion(Vector3UnitY, QuaternionMultiply(mech.torso_rotation, QuaternionFromEuler(80.0f * DEG2RAD, 0.0f, roll)));

	Projectile p;
	p.pos = base_pos;
	p.vel = dir * 20.0f;
	p.radius = 2.0f;
	p.team = mech.team;
	p.type = PROJECTILE_MISSILE;

	p.color = GOLD;
	p.mesh = g_meshes.prj_missile;
	p.material = LoadMaterialDefault();

	Vector3 mech_dir = TorsoDirection(mech);
	p.missile.state = MISSILE_RISE;
	p.missile.time = 0;
	p.missile.target_id = 0;
	p.missile.launch_position = base_pos;
	p.missile.launch_direction = mech_dir;
	p.missile.target_position = base_pos + mech_dir * 25.0f;
	p.missile.target_position.z = MISSILE_MAX_HEIGHT;

	CreateParticleTrail(&p);

	world.projectiles.push_back(p);
}

void UpdateProjectile(Projectile& p, World& world)
{
	if (p.type == PROJECTILE_MISSILE)
		UpdateProjectileMissile(p, world);

	float dt = GetFrameTime();
	p.vel += GRAVITY * p.gravity_scale * dt;
	p.vel += p.acc * dt;
	p.pos += p.vel * dt;

	p.destroy |= !CheckCollisionBoxSphere(WorldBox(), p.pos, 1.0f);
	p.material.maps[MATERIAL_MAP_DIFFUSE].color = p.color;
}

void DrawProjectile(const Projectile& p)
{
	Matrix t = MatrixTranslate(p.pos.x, p.pos.y, p.pos.z);
	Matrix r = MatrixLookRotation(Vector3Normalize(p.vel));
	DrawMesh(*p.mesh, p.material, r * t);
}

void DrawProjectileDebug(const Projectile& p)
{
	Vector3 dir = Vector3Normalize(p.vel);
	Vector3 top = p.pos + dir * p.length;
	Vector3 bot = p.pos - dir * p.length;
	Color color = p.debug_collion ? VIOLET : p.material.maps[MATERIAL_MAP_DIFFUSE].color;
	color.a = 128;

	switch (p.type)
	{
	case PROJECTILE_RIFLE:
	case PROJECTILE_SHOTGUN:
	case PROJECTILE_GRENADE:
	case PROJECTILE_MISSILE:
		DrawSphere(p.pos, p.radius, color);
		break;
	}

	//DrawLineDebug(p.pos, p.pos + dir * 20.0f, YELLOW, 4.0f);
	DrawAxesDebug(p.pos, MatrixLookRotation(dir), 10.0f, 2.0f);
}

void UpdateProjectileMissile(Projectile& p, World& world)
{
	ProjectileMissile& m = p.missile;
	if (m.state == MISSILE_RISE)
	{
		// Seek is the best way to do this because its smooth, and physics-based!
		p.acc = Seek(p.pos, m.target_position, p.vel, MISSILE_SEEK_SPEED);

		m.time += GetFrameTime();
		if (m.time >= 1.0f)
		{
			m.time = 0.0f;
			m.state = MISSILE_SEEK;
			p.gravity_scale = 0.0f;

			// Acquire target on-transition
			float distance = FLT_MAX;
			for (Mech& mech : world.mechs)
			{
				if (p.team != mech.team)
				{
					Vector2 mech_pos_2d = { mech.pos.x, mech.pos.y };
					Vector2 missile_pos_2d = { p.pos.x, p.pos.y };
					Vector2 missile_dir_2d = { m.launch_direction.x, m.launch_direction.y };
					Vector2 target_dir_2d = Vector2Normalize(mech_pos_2d - missile_pos_2d);

					// TODO - Lerp the angle from 30-60 based on mech heat level (if I want heat-seeking to begin with)
					float target_angle = fabsf(Vector2Angle(missile_dir_2d, target_dir_2d));
					float target_distance = Vector2Distance(missile_pos_2d, mech_pos_2d);
					if (target_distance < distance && target_angle <= 45.0f * DEG2RAD)
					{
						distance = target_distance;
						m.target_id = mech.id;
					}
				}
			}
		}
	}
	else if (m.state == MISSILE_SEEK)
	{
		p.color = MAROON;

		m.time += GetFrameTime();
		if (m.time >= 2.5f)
		{
			m.time = 0.0f;
			m.state = MISSILE_DIVE;
		}

		// Seek towards enemy mech, fallback to seeking towards launch direction if no enemies in range
		Vector3 target_position = m.target_id != 0 ?
			GetMechById(m.target_id, world)->pos :
			m.launch_position + m.launch_direction * 1000.0f;

		target_position.z = MISSILE_MAX_HEIGHT;
		p.acc = Seek(p.pos, target_position, p.vel, MISSILE_SEEK_SPEED);
	}
	else if (m.state == MISSILE_DIVE)
	{
		p.color = PINK;

		m.time += GetFrameTime();
		p.destroy |= m.time >= 1.0f;

		Vector3 target_position = m.target_id != 0 ?
			GetMechById(m.target_id, world)->pos :
			m.launch_position + m.launch_direction * 1000.0f;
		target_position.z = 0.0f;

		// Proximity-based detonation is awkward. Easier to do on-collision (increase seek-force if need be)!
	}
}

// The original's projectiles don't take mech velocity into account when firing
// Might be better to take mech velocity into account!
// Will lead to better dodger combos, or even harpoon teammate + grenade for crazy velocity!
// Easiest to not implement for now so we don't have to correct vel.z
//float t = fmaxf(0.0f, Vector3DotProduct(mech_dir, Vector3Normalize(mech.vel)));
//float speed = Lerp(10.0f, 20.0f, t);

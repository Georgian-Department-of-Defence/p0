#include "Gear.h"

void Rifle::OnUse(Mech2& mech, World2& world)
{
	Bullet* b = new Bullet;
	b->type = ENTITY_PROJECTILE;

	b->pos = pos;
	b->vel = EntityGetDirection(mech) * 30.0f;
	b->gravity_scale = 0.0f;

	b->mesh = &assets.mesh.bullet;
	b->color = RED;

	b->collider.type = COLLIDER_SPHERE;
	b->collider.sphere.radius = 5.0f;

	b->collision_type_mask = ENTITY_MASK_MECH | ENTITY_MASK_BUILDING;
	b->collision_team_mask = TARGET_MASK_ENEMY;
	b->team = mech.team;

	world.projectiles.push_back(b);
	PlaySound(assets.audio.fire_rifle);
}

void Shotgun::OnUse(Mech2& mech, World2& world)
{
	Vector3 dir = EntityGetDirection(mech);
	for (size_t i = 0; i < 3; i++)
	{
		Bullet* b = new Bullet;
		b->type = ENTITY_PROJECTILE;

		b->pos = pos;
		b->vel = (dir * MatrixRotateZ(-20.0f * DEG2RAD + 20.0f * DEG2RAD * i)) * 20.0f;
		b->gravity_scale = 0.0f;

		b->mesh = &assets.mesh.bullet;
		b->color = GREEN;

		b->collider.type = COLLIDER_SPHERE;
		b->collider.sphere.radius = 3.0f;

		b->collision_type_mask = ENTITY_MASK_MECH | ENTITY_MASK_BUILDING;
		b->collision_team_mask = TARGET_MASK_ENEMY;
		b->team = mech.team;

		world.projectiles.push_back(b);
	}
	PlaySound(assets.audio.fire_shotgun);
}

void GrenadeLauncher::OnUse(Mech2& mech, World2& world)
{
	grenade_count = 6;
}

void GrenadeLauncher::OnUpdate(Mech2& mech, World2& world)
{
	if (grenade_count > 0)
	{
		launch_time += GetFrameTime();
		if (launch_time >= 0.1f)
		{
			launch_time = 0.0f;
			grenade_count--;

			float pitch = (60.0f + Random(-15.0f, 15.0f)) * DEG2RAD;
			float roll = Random(-30.0f, 30.0f) * DEG2RAD;
			Vector3 dir = EntityGetDirection(mech) * MatrixRotateZ(roll) * MatrixRotateX(pitch);

			Missile* m = new Missile;
			m->type = ENTITY_PROJECTILE;

			m->pos = pos;
			m->vel = dir * 50.0f;
			m->gravity_scale = 6.0f;

			m->collider = MakeCapsule(pos, pos + dir * 6.0f, 2.0f);
			m->collision_type_mask = ENTITY_MASK_MECH | ENTITY_MASK_BUILDING;
			m->collision_team_mask = TARGET_MASK_ENEMY;
			m->team = mech.team;

			m->color = BLUE;
			m->mesh = &assets.mesh.grenade;

			world.projectiles.push_back(m);
			PlaySound(assets.audio.fire_grenade);
		}
	}
}

void Dasher::OnUse(Mech2& mech, World2& world)
{
}

void Dasher::OnUpdate(Mech2& mech, World2& world)
{
}

Gear* GearLoadRifle()
{
	Gear* gear = new Rifle;
	gear->cooldown_total = 1.0f;
	return gear;
}

Gear* GearLoadShotgun()
{
	Gear* gear = new Shotgun;
	gear->cooldown_total = 0.75f;
	return gear;
}

Gear* GearLoadGrenadeLauncher()
{
	Gear* gear = new GrenadeLauncher;
	gear->cooldown_total = 1.5f;
	return gear;
}

Gear* GearLoadDasher()
{
	Gear* gear = new Dasher;
	gear->cooldown_total = 2.0f;
	return gear;
}

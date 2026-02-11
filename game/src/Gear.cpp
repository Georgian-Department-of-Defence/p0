#include "Gear.h"

void Rifle::OnUse(Mech2& mech, World2& world)
{
	Bullet* b = new Bullet;
	b->pos = pos;
	b->vel = EntityGetDirection(mech) * 20.0f;

	b->mesh = &assets.mesh.bullet;
	b->color = RED;

	b->collider.type = COLLIDER_SPHERE;
	b->collider.sphere.radius = 3.0f;

	world.projectiles.push_back(b);
}

void Shotgun::OnUse(Mech2& mech, World2& world)
{
}

void GrenadeLauncher::OnUse(Mech2& mech, World2& world)
{
}

void GrenadeLauncher::OnUpdate(Mech2& mech, World2& world)
{
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

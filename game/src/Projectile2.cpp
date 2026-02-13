#include "Projectile2.h"

void Projectile2::OnCollisionPost(Entity* entity)
{
	switch (entity->type)
	{
		// Damage mech
		case ENTITY_MECH:
		{
			Mech2& mech = *(Mech2*)entity;
			break;
		}

		// Damage building
		case ENTITY_BUILDING:
		{
			Building2& building = *(Building2*)entity;
			break;
		}
	}
}

void Projectile2::OnDestroy(World2& world)
{
	PlaySound(assets.audio.hit);
}

void Bullet::OnUpdate()
{
	float dt = GetFrameTime();
	acc = GRAVITY * gravity_scale;
	vel += acc * dt;
	pos += vel * dt;
	collider.pos = pos;
}

void Missile::OnUpdate()
{
	assert(collider.type == COLLIDER_CAPSULE);

	float dt = GetFrameTime();
	acc = GRAVITY * gravity_scale;
	vel += acc * dt;
	pos += vel * dt;

	Vector3 direction = Vector3Normalize(vel);
	rot = MatrixLookRotation(direction);
	collider.capsule.direction = direction;
	collider.pos = pos;
}

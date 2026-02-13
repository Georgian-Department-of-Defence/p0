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

void Projectile2::Update()
{
	float dt = GetFrameTime();
	acc = GRAVITY * gravity_scale;
	vel += acc * dt;
	pos += vel * dt;

	Vector3 direction = Vector3Normalize(vel);
	rot = MatrixLookRotation(direction);

	collider.pos = pos;
	if (collider.type == COLLIDER_CAPSULE)
	{
		collider.capsule.direction = direction;
	}
}

void ProjectileDraw(const Projectile2& p, Material material)
{
	material.maps[MATERIAL_MAP_DIFFUSE].color = p.color;
	DrawMesh(*p.mesh, material, p.rot * MatrixTranslate(p.pos.x, p.pos.y, p.pos.z));
}

void Bullet::OnUpdate()
{
	Update();
}

void Missile::OnUpdate()
{
	Update();
}

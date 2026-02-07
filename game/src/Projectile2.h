#pragma once
struct Projectile2 : public Entity
{
	float damage = 0.0f;

	void OnCollisionPost(Entity* entity) final;
};

#pragma once
struct Projectile2 : public Entity
{
	float damage = 0.0f;

	virtual void OnUpdate() = 0;

	void OnCollisionPost(Entity* entity) final;
	void OnDestroy(World2& world) override;
	// Worry about Bullet vs Grenade vs Missile collision callback later

protected:
	void Update();
};

void ProjectileDraw(const Projectile2& p, Material material);

struct Bullet : public Projectile2
{
	virtual void OnUpdate() final;
};

struct Missile : public Projectile2
{
	virtual void OnUpdate() final;
};

#pragma once
struct Mech2 : public Entity
{
	void OnLoad() final;
	void OnUnload() final;
	void OnUpdate() final;
	void OnDraw() const final;
	void OnCollision(const Entity& entity, HitInfo hit_info) final;

	Vector2 dir_torso_curr = Vector2UnitY;
	Vector2 dir_torso_goal = Vector2UnitY;

	Vector2 dir_legs_curr = Vector2UnitY;
	Vector2 dir_legs_goal = Vector2UnitY;
};

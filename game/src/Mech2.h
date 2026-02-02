#pragma once
struct Mech2 : public Entity
{
	void OnUpdate() final;
	void OnDraw(Material material) const final;
	void OnCollision(const Entity& entity, HitInfo hit_info) final;

	Vector2 dir_torso_curr = Vector2Zeros;
	Vector2 dir_torso_goal = Vector2Zeros;
	
	Vector2 dir_legs_curr = Vector2Zeros;
	Vector2 dir_legs_goal = Vector2Zeros;

	float move_speed = 0.0f;
	float turn_speed = 0.0f;

	int player_index = -1;
};

#pragma once
struct Gear;
struct Mech2 : public Entity
{
	Vector2 dir_torso_curr = Vector2Zeros;
	Vector2 dir_torso_goal = Vector2Zeros;
	
	Vector2 dir_legs_curr = Vector2Zeros;
	Vector2 dir_legs_goal = Vector2Zeros;

	Gear* gear[4];

	float move_speed = 0.0f;
	float turn_speed = 0.0f;

	uint8_t player_number = 0; // [1..4], 0 is invalid.

	bool debug_enabled = false;

	void OnCollisionPost(Entity* entity) final;
};

void MechLoad(size_t index, World2& world);
void MechUnload(size_t index, World2& world);
void MechUpdate(size_t index, World2& world);
void MechDraw(size_t index, Material material, const World2& world);
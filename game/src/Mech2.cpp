#include "Mech2.h"

void MechLoad(size_t index, World2& world)
{
	Mech2& mech = world.mechs[index];

	//if (index == 0) mech.debug_enabled = true;

	mech.id = EntityGenId();
	mech.type = ENTITY_MECH;
	mech.player_number = uint8_t(index + 1);
	mech.team = index < 2 ? TEAM_RED : TEAM_BLUE;
	mech.color = mech.team == TEAM_RED ? RED : BLUE;

	mech.collision_type_mask = ENTITY_MASK_ALL;
	mech.collision_team_mask = TARGET_MASK_ALL;

	Vector3 spawn_positions[4];
	spawn_positions[0] = { -20.0f, -40.0f, 0.0f };
	spawn_positions[1] = { -20.0f,  40.0f, 0.0f };
	spawn_positions[2] = { 20.0f, -40.0f, 0.0f };
	spawn_positions[3] = { 20.0f,  40.0f, 0.0f };
	mech.pos = spawn_positions[index];

	Vector2 dir = index % 2 == 0 ? Vector2UnitY : Vector2UnitY * -1.0f;
	mech.dir_torso_curr = mech.dir_torso_goal = dir;
	mech.dir_legs_curr = mech.dir_legs_goal = dir;

	mech.collider.type = COLLIDER_SPHERE;
	mech.collider.sphere.radius = 8.0f;
	mech.collider_offset = Vector3UnitZ * 8.0f;

	mech.move_speed = 100.0f;
	mech.turn_speed = 100.0f * DEG2RAD;

	mech.gear[0] = GearLoadRifle();
	mech.gear[1] = GearLoadShotgun();
	mech.gear[2] = GearLoadRifle();
	mech.gear[3] = GearLoadDasher();
}

void MechUnload(size_t index, World2& world)
{
	Mech2& mech = world.mechs[index];
	for (size_t i = 0; i < 4; i++)
		delete mech.gear[i];

	// Structure data such that 0 is a reasonable default value for all fields
	// ie player_number is 1-4 suggesting that 0 is incorrect (and therefore a reasonable default state)
	// ***NOTE*** type enums will require restructuring such that ENUM_TYPE_COUNT at the end is replaced with ENUM_NONE always has the value 0!
	memset(&world.mechs[index], 0, sizeof(Mech2));
}

void MechUpdate(size_t index, World2& world)
{
	Mech2& mech = world.mechs[index];

	const float dt = GetFrameTime();
	const float deadzone = 0.75f;

	Vector2 input_move = Vector2Zeros;
	input_move.x =  (GetGamepadAxisMovement(mech.player_number - 1, GAMEPAD_AXIS_LEFT_X));
	input_move.y = -(GetGamepadAxisMovement(mech.player_number - 1, GAMEPAD_AXIS_LEFT_Y));

	Vector2 input_turn = Vector2Zeros;
	input_turn.x =  (GetGamepadAxisMovement(mech.player_number - 1, GAMEPAD_AXIS_RIGHT_X));
	input_turn.y = -(GetGamepadAxisMovement(mech.player_number - 1, GAMEPAD_AXIS_RIGHT_Y));

	if (mech.debug_enabled)
	{
		Vector2 mouse = GetMousePosition();
		mouse.x = Remap(mouse.x, 0.0f, GetScreenWidth(), WORLD_MIN.x, WORLD_MAX.x);
		mouse.y = Remap(mouse.y, GetScreenHeight(), 0.0f, WORLD_MIN.y, WORLD_MAX.y);

		input_turn = Vector2Normalize(mouse - Vector2{ mech.pos.x, mech.pos.y });
		input_move = Vector2Zeros;
		if (IsKeyDown(KEY_W)) input_move += Vector2UnitY;
		if (IsKeyDown(KEY_S)) input_move -= Vector2UnitY;
		if (IsKeyDown(KEY_A)) input_move -= Vector2UnitX;
		if (IsKeyDown(KEY_D)) input_move += Vector2UnitX;
	}

	if (Vector2Length(input_move) >= deadzone)
	{
		input_move = Vector2Normalize(input_move);
		mech.dir_legs_goal = input_move;
		mech.vel += Vector3{ input_move.x, input_move.y, 0.0f } * mech.move_speed * dt;
	}

	if (Vector2Length(input_turn) >= deadzone)
	{
		input_turn = Vector2Normalize(input_turn);
		mech.dir_torso_goal = input_turn;
	}

	mech.dir_torso_curr = Vector2RotateTowards(mech.dir_torso_curr, mech.dir_torso_goal, mech.turn_speed * dt);
	mech.dir_legs_curr = Vector2RotateTowards(mech.dir_legs_curr, mech.dir_legs_goal, mech.turn_speed * dt);
	mech.rot = MatrixRotateZ(Vector2Angle(Vector2UnitY, mech.dir_torso_curr));
	Vector3 mech_dir = EntityGetDirection(mech);

	mech.vel *= powf(0.05f, dt);
	mech.pos += mech.vel * dt;
	mech.pos = Vector3Clamp(mech.pos, WORLD_MIN, WORLD_MAX);
	mech.collider.pos = mech.pos + mech.collider_offset;

	constexpr Vector3 GEAR_MOUNT_OFFSETS[4] =
	{
		{ -8.0f, 5.0f, 15.0f },
		{ -3.0f, 2.0f, 15.0f },
		{  3.0f, 2.0f, 15.0f },
		{  8.0f, 5.0f, 15.0f }
	};

	bool use_gear[4];
	use_gear[0] = IsGamepadButtonDown(mech.player_number - 1, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
	use_gear[1] = IsGamepadButtonDown(mech.player_number - 1, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
	use_gear[2] = IsGamepadButtonDown(mech.player_number - 1, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
	use_gear[3] = IsGamepadButtonDown(mech.player_number - 1, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);

	if (mech.debug_enabled)
	{
		use_gear[0] = IsKeyDown(KEY_ONE);
		use_gear[1] = IsKeyDown(KEY_TWO);
		use_gear[2] = IsKeyDown(KEY_THREE);
		use_gear[3] = IsKeyDown(KEY_FOUR);
	}

	for (size_t i = 0; i < 4; i++)
	{
		Gear& gear = *mech.gear[i];
		mech.gear[i]->pos_draw = mech.pos + GEAR_MOUNT_OFFSETS[i] * mech.rot;
		mech.gear[i]->pos = mech.gear[i]->pos_draw + mech_dir * 2.0f;

		gear.cooldown_current += dt;
		if (gear.cooldown_current >= gear.cooldown_total && use_gear[i])
		{
			gear.cooldown_current = 0.0f;
			gear.OnUse(mech, world);
		}
		gear.OnUpdate(mech, world);
	}
}

void MechDraw(size_t index, Material material, const World2& world)
{
	const Mech2& mech = world.mechs[index];

	Matrix t = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);
	Matrix rot_torso = MatrixRotateZ(Vector2Angle(Vector2UnitY, mech.dir_torso_curr));
	Matrix rot_legs = MatrixRotateZ(Vector2Angle(Vector2UnitY, mech.dir_legs_curr));

	material.maps[MATERIAL_MAP_DIFFUSE].color = mech.color;

	DrawMesh(assets.mesh.torso, material, rot_torso * t);
	DrawMesh(assets.mesh.legs, material, rot_legs * t);
}

void Mech2::OnCollisionPost(Entity* entity)
{
	switch (entity->type)
	{
		// Damage other mech
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
	// (Projectile::OnCollision handles projectile-mech collision)
}

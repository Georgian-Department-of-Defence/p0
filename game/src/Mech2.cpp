#include "Mech2.h"

void MechLoad(size_t index, World2& world)
{
	Mech2& mech = world.mechs[index];

	mech.id = EntityGenId();
	mech.type = ENTITY_MECH;
	mech.player_number = uint8_t(index + 1);
	mech.team = index < 2 ? TEAM_RED : TEAM_BLUE;
	mech.color = mech.team == TEAM_RED ? RED : BLUE;

	Vector3 spawn_positions[4];
	spawn_positions[0] = { -20.0f, -40.0f, 0.0f };
	spawn_positions[1] = { -20.0f,  40.0f, 0.0f };
	spawn_positions[2] = { 20.0f, -40.0f, 0.0f };
	spawn_positions[3] = { 20.0f,  40.0f, 0.0f };
	mech.pos = spawn_positions[index];

	Vector2 dir = index % 2 == 0 ? Vector2UnitY : Vector2UnitY * -1.0f;
	mech.dir_torso_curr = mech.dir_torso_goal = dir;
	mech.dir_legs_curr = mech.dir_legs_goal = dir;

	mech.collider.type = COLLIDER_CIRCLE;
	mech.collider.circle.radius = 25.0f;
}

void MechUnload(size_t index, World2& world)
{
	// Structure data such that 0 is a reasonable default value for all fields
	// ie player_number is 1-4 suggesting that 0 is incorrect (and therefore a reasonable default state)
	// ***NOTE*** type enums will require restructuring such that ENUM_TYPE_COUNT at the end is replaced with ENUM_NONE always has the value 0!
	memset(&world.mechs[index], 0, sizeof(Mech2));
}

void MechUpdate(size_t index, World2& world)
{
	Mech2& mech = world.mechs[index];

	mech.dir_torso_curr = Vector2RotateTowards(mech.dir_torso_curr, mech.dir_torso_goal, mech.turn_speed * GetFrameTime());
	mech.dir_legs_curr = Vector2RotateTowards(mech.dir_legs_curr, mech.dir_legs_goal, mech.turn_speed * GetFrameTime());

	mech.rot = QuaternionFromMatrix(MatrixRotateZ(Vector2Angle(Vector2UnitY, mech.dir_torso_curr)));
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

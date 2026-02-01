#include "Mech2.h"

void Mech2::OnLoad()
{
	move_speed = 100.0f;
	turn_speed = 10.0f * DEG2RAD;

	dir_torso_curr = Vector2Rotate(Vector2UnitY, 0.0f  * DEG2RAD);
	dir_torso_goal = Vector2Rotate(Vector2UnitY, 45.0f * DEG2RAD);
	
	dir_legs_curr = Vector2Rotate(Vector2UnitY, -30.0f * DEG2RAD);
	dir_legs_goal = Vector2Rotate(Vector2UnitY, -60.0f * DEG2RAD);
}

void Mech2::OnUnload()
{
	
}

void Mech2::OnUpdate()
{
	dir_torso_curr = Vector2RotateTowards(dir_torso_curr, dir_torso_goal, turn_speed * GetFrameTime());
	dir_legs_curr = Vector2RotateTowards(dir_legs_curr, dir_legs_goal, turn_speed * GetFrameTime());
	
	rot = QuaternionFromMatrix(MatrixRotateZ(Vector2Angle(Vector2UnitY, dir_torso_curr)));
}

void Mech2::OnDraw() const
{
	Matrix t = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix rot_torso = MatrixRotateZ(Vector2Angle(Vector2UnitY, dir_torso_curr));
	Matrix rot_legs = MatrixRotateZ(Vector2Angle(Vector2UnitY, dir_legs_curr));

	Material material = assets.material.flat;
	DrawAxesDebug(pos, QuaternionToMatrix(rot), 25.0f, 5.0f);
	DrawMesh(assets.mesh.torso, material, rot_torso * t);
	DrawMesh(assets.mesh.legs, material, rot_legs * t);

	Vector3 dir = EntityDirection(*this);
	DrawLineDebug(pos, pos + dir * 25.0f, GOLD, 8.0f);
}

void Mech2::OnCollision(const Entity& entity, HitInfo hit_info)
{

}

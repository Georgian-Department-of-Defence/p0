#include "Mech2.h"

void Mech2::OnLoad()
{
	dir_torso_curr = Vector2Rotate(Vector2UnitY, 0.0f);
	dir_torso_goal = Vector2Rotate(Vector2UnitY, 45.0f * DEG2RAD);
}

void Mech2::OnUnload()
{
	
}

void Mech2::OnUpdate()
{
	dir_torso_curr = Vector2RotateTowards(dir_torso_curr, dir_torso_goal, 10.0f * DEG2RAD * GetFrameTime());
}

void Mech2::OnDraw() const
{
	Matrix t = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix r_torso = MatrixRotateZ(Vector2Angle(Vector2UnitY, dir_torso_curr));
	Matrix r_legs = MatrixRotateZ(Vector2Angle(Vector2UnitY, dir_legs_curr));

	Material material = assets.material.flat;
	DrawMesh(assets.mesh.torso, material, r_torso * t);
	DrawMesh(assets.mesh.legs, material, r_legs * t);
}

void Mech2::OnCollision(const Entity& entity, HitInfo hit_info)
{

}

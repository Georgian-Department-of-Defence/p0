#include "World2.h"

void LoadWorld(World2& world)
{
	for (int i = 0; i < 4; i++)
	{
		Mech2* mech = new Mech2;
		mech->type = ENTITY_MECH;
		mech->player_index = i;
		mech->team = mech->player_index < 2 ? TEAM_RED : TEAM_BLUE;
		mech->color = mech->team == TEAM_RED ? RED : BLUE;

		Vector3 positions[4];
		positions[0] = { -20.0f, -40.0f, 0.0f };
		positions[1] = { -20.0f,  40.0f, 0.0f };
		positions[2] = { 20.0f, -40.0f, 0.0f };
		positions[3] = { 20.0f,  40.0f, 0.0f };
		mech->pos = positions[mech->player_index];

		Vector2 dir = mech->player_index % 2 == 0 ? Vector2UnitY : Vector2UnitY * -1.0f;
		mech->dir_torso_curr = mech->dir_torso_goal = dir;
		mech->dir_legs_curr = mech->dir_legs_goal = dir;

		world.entities.push_back(mech);
	}
}

void UnloadWorld(World2& world)
{
	for (size_t i = 0; i < world.entities.size(); i++)
		delete world.entities[i];
	world.entities.clear();
}

void UpdateWorld(World2& world)
{
	for (Entity* entity : world.entities)
		entity->OnUpdate();
}

void DrawWorld(const World2& world)
{
	{
		BeginTextureMode(assets.framebuffer.shadow_map);
			ClearBackground(ORANGE);
			rlEnableDepthTest();
			rlSetMatrixModelview(g_camera_system.light_view);
			rlSetMatrixProjection(g_camera_system.light_proj);
			for (const Entity* entity : world.entities)
				entity->OnDraw(assets.material.flat);
			EndMode3D();
		EndTextureMode();
	}
	DrawTextureDepth(assets.framebuffer.shadow_map);
	// Error 404 -- shadows not found xD xD xD

	//{
	//	BeginMode3D(*GetCamera());
	//	for (const Entity* entity : world.entities)
	//		entity->OnDraw(assets.material.flat);
	//	EndMode3D();
	//}
}

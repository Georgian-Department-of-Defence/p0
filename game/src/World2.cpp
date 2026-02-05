#include "World2.h"

void LoadWorld(World2& world)
{
	world.mechs.resize(4);
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechLoad(i, world);

	Light sun;
	LoadLightUniforms(sun, 0, assets.material.lighting.shader);
	sun.direction = Vector3Normalize(Vector3Zeros - g_camera_system.light_pos);
	sun.color = Vector3Ones;
	sun.ambient = 0.2f;
	sun.diffuse = 0.75f;
	sun.specular = 1.0f;
	sun.specular_exponent = 64.0f;
	world.lights.push_back(sun);
	assert(world.lights.size() == MAX_LIGHTS);
}

void UnloadWorld(World2& world)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechUnload(i, world);
}

void UpdateWorld(World2& world)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechUpdate(i, world);

	for (Light& light : world.lights)
		UpdateLightUniforms(light, assets.material.lighting.shader);
}

void DrawWorld(const World2& world)
{
	// Shadow pass
	BeginTextureMode(assets.framebuffer.shadow_map);
		ClearBackground(ORANGE);
		rlEnableDepthTest();
		rlSetMatrixModelview(g_camera_system.light_view);
		rlSetMatrixProjection(g_camera_system.light_proj);

		for (size_t i = 0; i < world.mechs.size(); i++)
			MechDraw(i, assets.material.flat, world);

		EndMode3D();
	EndTextureMode();

	// Scene pass
	Material material = assets.material.lighting;
	material.maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
	BeginTextureMode(assets.framebuffer.main_multisample);
		ClearBackground(BLACK);
		rlSetClipPlanes(0.1f, 500.0f);
		BeginMode3D(*GetCamera());
		SetShaderValue(material.shader, material.shader.locs[SHADER_LOC_VECTOR_VIEW], &GetCamera()->position, SHADER_UNIFORM_VEC3);
		SetShaderValueMatrix(material.shader, world.lights.back().loc_light_view_proj, g_camera_system.light_view * g_camera_system.light_proj);

		DrawMesh(assets.mesh.ground, material, MatrixRotateX(PI * 0.5f));

		for (size_t i = 0; i < world.mechs.size(); i++)
			MechDraw(i, material, world);

		//DrawParticles(world, renderer);
		EndMode3D();
	EndTextureMode();

	// Resolve MSAA
	{
		RenderTexture read = assets.framebuffer.main_multisample;
		RenderTexture draw = assets.framebuffer.main_resolve;
		rlBindFramebuffer(RL_READ_FRAMEBUFFER, read.id);
		rlBindFramebuffer(RL_DRAW_FRAMEBUFFER, draw.id);
		rlBlitFramebuffer(
			0, 0, read.texture.width, read.texture.height,
			0, 0, draw.texture.width, draw.texture.height,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		rlDisableFramebuffer();
	}

	// UI pass (billboards & debug)
	BeginTextureMode(assets.framebuffer.main_resolve);
	glClear(GL_DEPTH_BUFFER_BIT);
	BeginMode3D(*GetCamera());
	for (const Mech2& mech : world.mechs)
	{
		DrawAxesDebug(mech.pos + Vector3UnitZ, QuaternionToMatrix(mech.rot), 25.0f, 4.0f);
	
		Texture tex = assets.texture.gradient;
		Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
		DrawBillboardRec(*GetCamera(), tex, src, mech.pos + Vector3{ 0.0f, 10.0f, 20.0f }, { 16.0f, 4.0f }, WHITE);
	}
	EndMode3D();
	EndTextureMode();

	// Downsample
	{
		RenderTexture read = assets.framebuffer.main_resolve;
		RenderTexture draw = assets.framebuffer.downsample;
		rlBindFramebuffer(RL_READ_FRAMEBUFFER, read.id);
		rlBindFramebuffer(RL_DRAW_FRAMEBUFFER, draw.id);
		rlBlitFramebuffer(
			0, 0, read.texture.width, read.texture.height,
			0, 0, draw.texture.width, draw.texture.height,
			GL_COLOR_BUFFER_BIT);
		rlDisableFramebuffer();
	}

	// Present FSQ result!
	//DrawTextureDepth(assets.framebuffer.shadow_map);
	DrawTextureColor(assets.framebuffer.main_resolve);
	//DrawTextureColor(assets.framebuffer.downsample);
}

void MechLoad(size_t index, World2& world)
{
	Mech2& mech = world.mechs[index];

	mech.id = EntityGenId();
	mech.type = ENTITY_MECH;
	mech.player_number = index + 1;
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

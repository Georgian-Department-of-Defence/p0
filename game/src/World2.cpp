#include "World2.h"

void LoadWorld(World2& world)
{
	world.entities.reserve(1024);

	for (size_t i = 0; i < ENTITY_COUNT_MECHS; i++)
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
	for (size_t i = 0; i < world.entities.size(); i++)
		delete world.entities[i];
	world.entities.clear();
}

void UpdateWorld(World2& world)
{
	for (Entity* entity : world.entities)
		entity->OnUpdate();

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
		for (const Entity* entity : world.entities)
			entity->OnDraw(assets.material.flat);
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
		for (const Entity* entity : world.entities)
			entity->OnDraw(material);

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
	for (const Mech2* mech : WorldGetMechsConst(world))
	{
		DrawAxesDebug(mech->pos + Vector3UnitZ, QuaternionToMatrix(mech->rot), 25.0f, 4.0f);

		Texture tex = assets.texture.gradient;
		Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
		DrawBillboardRec(*GetCamera(), tex, src, mech->pos + Vector3{ 0.0f, 10.0f, 20.0f }, { 16.0f, 4.0f }, WHITE);
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

std::vector<Mech2*> WorldGetMechs(World2& world)
{
	std::vector<Mech2*> mechs;
	mechs.reserve(4);
	for (size_t i = 0; i < ENTITY_COUNT_MECHS; i++)
	{
		if (world.entities[i] != nullptr && world.entities[i]->type == ENTITY_MECH)
			mechs.push_back((Mech2*)world.entities[i]);
	}
	return mechs;
}

std::vector<const Mech2*> WorldGetMechsConst(const World2& world)
{
	std::vector<const Mech2*> mechs;
	mechs.reserve(4);
	for (size_t i = 0; i < ENTITY_COUNT_MECHS; i++)
	{
		if (world.entities[i] != nullptr && world.entities[i]->type == ENTITY_MECH)
			mechs.push_back((const Mech2*)world.entities[i]);
	}
	return mechs;
}

// Better to query objects by-category on a case-by-case basis.
// Performing n if-statements where n < 1000 is insignificant.
// Optimize this by switching to a borderline relational database as an ECS if necessary!
/*
void UpdateWorldFrame(World2& world)
{
	// This will fail the moment an object is created mid-frame...
	// Best to just partition world.entities

	auto remove_start = std::remove_if(world.entities.begin(), world.entities.end(), [](Entity* entity)
	{
		return entity->destroy_flag;
	});

	for (size_t i = std::distance(world.entities.begin(), remove_start); i < world.entities.size(); i++)
	{
		delete world.entities[i];
		world.entities[i] = nullptr;
	}

	world.entities.erase(remove_start, world.entities.end());

	world.frame.mechs.clear();
	//world.frame.buildings.clear();
	//world.frame.projectiles.clear();

	for (Entity* entity : world.entities)
	{
		switch (entity->type)
		{
		case ENTITY_MECH:
			world.frame.mechs.push_back((Mech2*)entity);
			break;
		case ENTITY_BUILDING:
			//world.frame.buildings.push_back((Building2*)entity);
			break;
		case ENTITY_PROJECTILE:
			//world.frame.projectiles.push_back((Projectile2*)entity);
			break;
		case ENTITY_TYPE_COUNT:
			assert(false);
			break;
		}
	}
}
*/

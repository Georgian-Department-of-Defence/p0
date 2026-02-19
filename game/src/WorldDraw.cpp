#include "World2.h"

void WorldDraw(const World2& world)
{
	// Shadow pass
	BeginTextureMode(assets.framebuffer.shadow_map);
	{
		ClearBackground(ORANGE);
		rlEnableDepthTest();
		rlSetMatrixModelview(g_camera_system.light_view);
		rlSetMatrixProjection(g_camera_system.light_proj);
		Material material = assets.material.flat;

		for (size_t i = 0; i < world.mechs.size(); i++)
			MechDraw(i, material, world);

		for (const Building2& b : world.buildings)
			BuildingDraw(b, material);

		for (const Projectile2* p : world.projectiles)
			ProjectileDraw(*p, material);

		EndMode3D();
	}
	EndTextureMode();

	// Scene pass
	BeginTextureMode(assets.framebuffer.main_multisample);
	{
		ClearBackground(BLACK);
		rlSetClipPlanes(0.1f, 500.0f);
		BeginMode3D(*GetCamera());

		Material material = assets.material.lighting;
		material.maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
		SetShaderValue(material.shader, material.shader.locs[SHADER_LOC_VECTOR_VIEW], &GetCamera()->position, SHADER_UNIFORM_VEC3);
		SetShaderValueMatrix(material.shader, world.lights.back().loc_light_view_proj, g_camera_system.light_view * g_camera_system.light_proj);

		DrawMesh(assets.mesh.ground, material, MatrixRotateX(PI * 0.5f));

		for (size_t i = 0; i < world.mechs.size(); i++)
			MechDraw(i, material, world);

		for (const Building2& b : world.buildings)
			BuildingDraw(b, material);

		for (const Projectile2* p : world.projectiles)
			ProjectileDraw(*p, material);

		//DrawParticles(world, renderer);
		EndMode3D();
	}
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

	// UI & debug
	BeginTextureMode(assets.framebuffer.main_resolve);
	BeginMode3D(*GetCamera());
	{
		std::vector<Entity*> entities = WorldGetEntities(world);

		// UI
		for (const Mech2& mech : world.mechs)
		{
			Texture tex = assets.texture.gradient;
			Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
			DrawBillboardRec(*GetCamera(), tex, src, mech.pos + Vector3{ 0.0f, 10.0f, 20.0f }, { 16.0f, 4.0f }, WHITE);
		}

		// Mech debug
		for (const Mech2& mech : world.mechs)
		{
			DrawAxesDebug(mech.pos + Vector3UnitZ, mech.rot, 25.0f, 4.0f);
			//DrawSphere(mech.pos + mech.collider_offset, 8.0f, ColorFromNormalized({ 0.0f, 1.0f, 0.0f, 0.75f }));

			for (size_t i = 0; i < 4; i++)
			{
				Gear& gear = *mech.gear[i];
				DrawSphere(gear.pos_draw, 0.5f, DARKGREEN);
				DrawSphere(gear.pos, 0.5f, DARKBLUE);
			}
		}

		// Collider debug
		for (Entity* entity : entities)
		{
			DrawCollider(entity->collider, entity->collider.debug_color);
		}
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

#include "World2.h"

void WorldLoad(World2& world)
{
	world.mechs.resize(4);
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechLoad(i, world);

	Light sun;
	LightLoadUniforms(sun, 0, assets.material.lighting.shader);
	sun.direction = Vector3Normalize(Vector3Zeros - g_camera_system.light_pos);
	sun.color = Vector3Ones;
	sun.ambient = 0.2f;
	sun.diffuse = 0.75f;
	sun.specular = 1.0f;
	sun.specular_exponent = 64.0f;
	world.lights.push_back(sun);
	assert(world.lights.size() == MAX_LIGHTS);
}

void WorldUnload(World2& world)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechUnload(i, world);
}

void WorldUpdate(World2& world)
{
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechUpdate(i, world);

	for (Light& light : world.lights)
		LightUpdateUniforms(light, assets.material.lighting.shader);

	std::vector<EntityHit> hits;
	WorldCheckCollisions(world, &hits);
	WorldResolveCollisions(world, hits);
}

void WorldDraw(const World2& world)
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
		Texture tex = assets.texture.gradient;
		Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
		DrawBillboardRec(*GetCamera(), tex, src, mech.pos + Vector3{ 0.0f, 10.0f, 20.0f }, { 16.0f, 4.0f }, WHITE);
	}

	for (const Mech2& mech : world.mechs)
	{
		DrawAxesDebug(mech.pos + Vector3UnitZ, QuaternionToMatrix(mech.rot), 25.0f, 4.0f);

		Color color = ColorFromNormalized({ 0.0f, 1.0f, 0.0f, 0.75f });
		DrawSphere(mech.pos + mech.collider_offset, 8.0f, color);

		for (size_t i = 0; i < 4; i++)
			DrawSphere(mech.gear_mount_positions[i], 0.5f, DARKGREEN);
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

std::vector<Entity*> WorldGetEntities(const World2& world)
{
	size_t i = 0;
	std::vector<Entity*> entities;
	entities.resize(world.mechs.size() + world.buildings.size() + world.projectiles.size());

	for (const Mech2& mech : world.mechs)
	{
		entities[i] = (Entity*)&mech;
		i++;
	}

	for (const Building2& building : world.buildings)
	{
		entities[i] = (Entity*)&building;
		i++;
	}

	for (const Projectile2& projectile : world.projectiles)
	{
		entities[i] = (Entity*)&projectile;
		i++;
	}

	return entities;
}

void WorldCheckCollisions(const World2& world, std::vector<EntityHit>* hits)
{
	std::vector<Entity*> entities = WorldGetEntities(world);
	for (size_t i = 0; i < entities.size(); i++)
	{
		for (size_t j = i + 1; j < entities.size(); j++)
		{
			Vector3 mtv = Vector3Zeros;
			Entity* a = entities[i];
			Entity* b = entities[j];
			if (EntityCheckCollision3D(*a, *b, &mtv))
			{
				EntityHit hit;
				hit.a = a;
				hit.b = b;
				hit.mtv = mtv;
				hits->push_back(hit);
			}
		}
	}
}

void WorldResolveCollisions(World2& world, std::vector<EntityHit> hits)
{
	// Pre-pass to ensure mtvs resolve A from B
	//for (EntityHit& hit : hits)
	//{
	//	Vector2 pA = { hit.a->pos.x, hit.a->pos.y };
	//	Vector2 pB = { hit.b->pos.x, hit.b->pos.y };
	//	Vector2 BA = pA - pB;
	//	if (Vector2DotProduct(BA, hit.mtv) < 0.0f)
	//		hit.mtv *= -1.0f;
	//}
	// Update: easier to swap the direction of MTV in the case of B??

	for (const EntityHit& hit : hits)
	{
		Vector3 mtv_a = hit.mtv *  1.0f;
		Vector3 mtv_b = hit.mtv * -1.0f;
		hit.a->OnCollisionPre(hit.b);
		hit.b->OnCollisionPre(hit.a);
	}

	// *Insert impulse & friction code here*
	// *Insert position code here*

	for (const EntityHit& hit : hits)
	{
		Vector3 mtv_a = hit.mtv *  1.0f;
		Vector3 mtv_b = hit.mtv * -1.0f;
		hit.a->OnCollisionPost(hit.b);
		hit.b->OnCollisionPost(hit.a);
	}
}

#include "World2.h"

void WorldLoad(World2& world)
{
	world.mechs.resize(4);
	for (size_t i = 0; i < world.mechs.size(); i++)
		MechLoad(i, world);

	for (float x = WORLD_MIN.x + 10.0f; x < WORLD_MAX.x - 10.0f; x += 25.0f)
	{
		Building2 building = MakeBuilding({ x, 0.0f, 0.0f }, BUILDING_BMO);
		world.buildings.push_back(building);
	}

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

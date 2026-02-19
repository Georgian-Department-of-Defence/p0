#include "World2.h"
void MapLoad(MapType map, World2& world);

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

	// Success!
	//MapLoad(MAP_MINTY_AFTERSHAVE, world);

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

void MapLoad(MapType map, World2& world)
{
	const char* path = nullptr;
	switch (map)
	{
	case MAP_LOBSTER_DINNER:
		path = "./assets/maps/lobster_dinner.p0_map";
		break;

	case MAP_AFTERNOON_DRIVE:
		path = "./assets/maps/afternoon_drive.p0_map";
		break;

	case MAP_MINTY_AFTERSHAVE:
		// Generate with 2d loop
		break;

	case MAP_BUTTON_UP:
		path = "./assets/maps/button_up.p0_map";
		break;

	case MAP_FOX_ONLY_NO_ITEMS:
		// Don't load anything xD
		break;

	case MAP_TEST_1:
		path = "./assets/maps/test_1.p0_map";
		break;

	case MAP_TEST_2:
		path = "./assets/maps/test_2.p0_map";
		break;

	case MAP_TEST_3:
		path = "./assets/maps/test_3.p0_map";
		break;
	}

	if (map == MAP_MINTY_AFTERSHAVE)
	{
		const float step = 20.0f;
		for (float y = -40.0f; y <= 40.0f; y += step)
		{
			for (float x = -80.0f; x <= 80.0f; x += step)
			{
				Building2 building = MakeBuilding({ x, y, 0.0f }, BUILDING_TD);
				world.buildings.push_back(building);
			}
		}
	}
	else if (map == MAP_FOX_ONLY_NO_ITEMS)
	{

	}
	else
	{
		assert(FileExists(path));       // <-- Comment this out when creating a new map
		MapImport(path, &world.buildings);
	}
}

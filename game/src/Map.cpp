#include "Map.h"
#include "World.h"
#include <fstream>
#include <cassert>

struct BuildingBinary
{
    BuildingType type;
    Vector3 pos;
};

void LoadMap(MapType map, World& world)
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

    // Comment this out when creating a new map
	if (map != MAP_FOX_ONLY_NO_ITEMS && map != MAP_MINTY_AFTERSHAVE)
		assert(FileExists(path));

    Load(path, world.buildings);
}

void Load(const char* path, std::vector<Building>& buildings)
{
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);

    int building_count = 0;
    file.read((char*)&building_count, sizeof(building_count));

    std::vector<BuildingBinary> data;
    data.resize(building_count);
    file.read((char*)data.data(), sizeof(BuildingBinary) * building_count);

    for (size_t i = 0; i < building_count; i++)
    {
        Building building;
        CreateBuilding(&building, data[i].type);

        building.type = data[i].type;
        building.pos = data[i].pos;

#if DEBUG
        building.edt_id = GenId();
        building.edt_color = GRAY;
#endif

        buildings.push_back(building);
    }

    file.close();
}

void Save(const char* path, std::vector<Building>& buildings)
{
    std::ofstream file;
    file.open(path, std::ios::binary | std::ios::out | std::ios::trunc);

    int building_count = buildings.size();
    std::vector<BuildingBinary> data;
    data.resize(building_count);

    for (size_t i = 0; i < building_count; i++)
    {
        data[i].type = buildings[i].type;
        data[i].pos = buildings[i].pos;
    }

    file.write((const char*)&building_count, sizeof(building_count));
    file.write((const char*)data.data(), sizeof(BuildingBinary) * data.size());
    file.close();
}

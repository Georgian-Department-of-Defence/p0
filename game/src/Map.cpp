#include "Map.h"

struct BuildingFileData
{
    BuildingType building_type;
    Vector3 pos;
};

void MapImport(const char* path, std::vector<Building2>* buildings)
{
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);

    int building_count = 0;
    file.read((char*)&building_count, sizeof(building_count));

    std::vector<BuildingFileData> data;
    data.resize(building_count);
    file.read((char*)data.data(), sizeof(BuildingFileData) * building_count);

    for (size_t i = 0; i < building_count; i++)
    {
        Building2 building = MakeBuilding(data[i].pos, data[i].building_type);
        buildings->push_back(building);
    }

    file.close();
}

void MapExport(const char* path, const std::vector<Building2>& buildings)
{
    std::ofstream file;
    file.open(path, std::ios::binary | std::ios::out | std::ios::trunc);

    int building_count = buildings.size();
    std::vector<BuildingFileData> data;
    data.resize(building_count);

    for (size_t i = 0; i < building_count; i++)
    {
        data[i].building_type = buildings[i].building_type;
        data[i].pos = buildings[i].pos;
    }

    file.write((const char*)&building_count, sizeof(building_count));
    file.write((const char*)data.data(), sizeof(BuildingFileData) * data.size());
    file.close();
}

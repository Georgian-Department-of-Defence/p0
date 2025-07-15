#pragma once
#include "WorldDef.h"
#include "Building.h"
#include <vector>

void LoadMap(MapType map, World& world);

void Save(const char* path, std::vector<Building>& buildings);
void Load(const char* path, std::vector<Building>& buildings);

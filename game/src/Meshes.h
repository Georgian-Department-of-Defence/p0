#pragma once
#include "raylib.h"
#include "Types.h"

struct Meshes
{
	// TODO - Load mech animations from gltf file
	Mesh* mech_torso;
	Mesh* mech_legs;

	Mesh* bld_td;
	Mesh* bld_bmo;
	Mesh* bld_condo;
	//Mesh* bld_cn_tower;

	Mesh* prj_straight;
	Mesh* prj_grenade;
	Mesh* prj_missile;

	// TODO - Load these once world-systems work ;)
	//Mesh* gear_rifle;
	//Mesh* gear_shotgun;
	//Mesh* gear_grenade;
	
	// Test colliders using 3d models instead of debug geometry
	// so we have a feel for the in-game scale
};

extern Meshes g_meshes;

void LoadMeshes();
void UnloadMeshes();

Vector3 BuildingExtents(BuildingType type);
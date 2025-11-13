#pragma once
#include "raylib.h"

struct Audio
{
	Sound fire_rifle;
	Sound fire_shotgun;
	Sound fire_grenade;
	Sound fire_missile;
	Sound fire_dasher;

	Sound hit;
	Sound hit_mech;

	Sound heat_overheat;
	Sound heat_restore;
};

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

void LoadAudio();
void UnloadAudio();

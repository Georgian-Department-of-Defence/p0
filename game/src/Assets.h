#pragma once
#include <raylib.h>
// An "Asset" is a resource loaded from outside the program.
// In-memory resources such as Camera, Framebuffer, Material, etc belong in elsewhere

struct Meshes
{
	Mesh torso;
	Mesh legs;

	Mesh td;
	Mesh bmo;
	Mesh condo;
	//Mesh cn_tower;

	Mesh bullet;
	Mesh grenade;
	Mesh missile;

	//Mesh gear_rifle;
	//Mesh gear_shotgun;
	//Mesh gear_grenade;
};

struct Audios
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

	//Music never_gonna_give_you_up_rick_astley;
};

struct Shaders
{
	Shader skinning;
	Shader lighting;
	Shader depth;
};

// Move this to renderer, as well as Camera and Framebuffer?
//struct Materials
//{
//	Material flat;
//	Material lighting;
//};

struct Textures
{
	Texture2D white;
	Texture2D gradient;
};

struct Assets
{
	Audios audio;
	Meshes mesh;
	Shaders shader;
	Textures texture;
};

extern Assets assets;

void LoadAssets();
void UnloadAssets();

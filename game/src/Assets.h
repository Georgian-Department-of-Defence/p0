#pragma once
// An "Asset" is a resource loaded from outside the program.
// In-memory resources such as Camera, Framebuffer, Material, etc belong in elsewhere

struct Meshes
{
	Mesh torso;
	Mesh legs;

	Mesh ground;
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

struct Materials
{
	Material flat;
	Material depth;
	Material lighting;
	Material skinning;
};

struct Textures
{
	Texture2D white;
	Texture2D gradient;
};

struct Framebuffers
{
	RenderTexture shadow_map;
	RenderTexture main_multisample;
	RenderTexture main_resolve;
	RenderTexture downsample;
};

struct Assets
{
	Audios audio;
	Meshes mesh;
	Textures texture;
	Materials material;
	Framebuffers framebuffer;
};

extern Assets assets;

void InitAssets();
void QuitAssets();

// Materials are shaders + uniform data such as colours & textures.
// Easier to work at the material level than the shader level.
// Just load whichever shaders are reused internally, then make said materials share them!

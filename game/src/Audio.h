#pragma once
#include "raylib.h"

struct Audio
{
	Sound fire_rifle;
	Sound fire_shotgun;
	Sound fire_grenade;
	Sound fire_dasher;

	Sound hit;
	Sound hit_mech;
};

extern Audio g_audio;

void LoadAudio();
void UnloadAudio();

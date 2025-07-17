#pragma once
#include "raylib.h"

struct Textures
{
	Texture2D white;

	// Mechs look better untextured
	//Texture2D mech_red;
	//Texture2D mech_blue;
};

extern Textures g_textures;

void LoadTextures();
void UnloadTextures();

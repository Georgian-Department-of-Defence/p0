#pragma once
#include "raylib.h"

struct Textures
{
	Texture2D white;
	Texture2D gradient;
};

extern Textures g_textures;

void LoadTextures();
void UnloadTextures();

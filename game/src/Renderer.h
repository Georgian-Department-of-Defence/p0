#pragma once
#include "raylib.h"

struct Renderer
{
	RenderTexture rt_downsample;
	int flags;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

#pragma once
#include "raylib.h"

struct Renderer
{
	RenderTexture rt_downsample;
	RenderTexture rt_shadowmap;
	int flags;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

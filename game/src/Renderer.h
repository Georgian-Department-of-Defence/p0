#pragma once
#include "raylib.h"

constexpr int RENDER_WIDTH = 3840 / 2;
constexpr int RENDER_HEIGHT = 2160 / 2;
// 2k vs 8k has no significant difference.
// Opting to render at 2k MSAA 4x.

struct Renderer
{
	unsigned int fbo;
	int tex_color;
	int tex_depth;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

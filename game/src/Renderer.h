#pragma once
#include "raylib.h"

constexpr int RENDER_WIDTH = 3840 * 2;
constexpr int RENDER_HEIGHT = 2160 * 2;

struct Renderer
{
	unsigned int fbo;
	int tex_color;
	int tex_depth;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

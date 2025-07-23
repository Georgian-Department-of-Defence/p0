#pragma once
#include "raylib.h"

struct Renderer
{
	unsigned int fbo;
	int tex_color;
	int tex_depth;
	int flags;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

int FramebufferWidth();
int FramebufferHeight();

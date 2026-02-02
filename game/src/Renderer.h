#pragma once

struct RTMS
{
	unsigned int fbo;
	unsigned int color;
	unsigned int depth;
};

// TODO -- move this to assets. This isn't a "Renderer" -- its just a bunch of render textures...
struct Renderer
{
	RenderTexture rt_shadowmap;
	RenderTexture rt_main_multisample;
	RenderTexture rt_main_resolve;
	RenderTexture rt_downsample;
	int flags;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

void DrawColor(RenderTexture rt);
void DrawDepth(RenderTexture rt);

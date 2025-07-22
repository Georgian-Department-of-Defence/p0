#include "Renderer.h"
#include "rlgl.h"
#include "raymathext.h"
#include "Shaders.h"
#include <cassert>

void LoadRenderer(Renderer& r)
{
	r.fbo = rlLoadFramebuffer();
	r.tex_color = rlLoadTexture(NULL, RENDER_WIDTH, RENDER_HEIGHT, RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
	r.tex_depth = rlLoadTextureDepth(RENDER_WIDTH, RENDER_HEIGHT, true);
	rlFramebufferAttach(r.fbo, r.tex_color, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(r.fbo, r.tex_depth, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
	assert(rlFramebufferComplete(r.fbo));
}

void UnloadRenderer(Renderer& r)
{
	rlUnloadTexture(r.tex_depth);
	rlUnloadTexture(r.tex_color);
	rlUnloadFramebuffer(r.fbo);
}

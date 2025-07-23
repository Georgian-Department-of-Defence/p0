#include "Renderer.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"
#include "Shaders.h"
#include <cassert>

static const float framebuffer_scale = 0.5f;

void LoadRenderer(Renderer& r)
{
	r.fbo = rlLoadFramebuffer();
	r.tex_color = rlLoadTexture(NULL, FramebufferWidth(), FramebufferHeight(), RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
	r.tex_depth = rlLoadTextureDepth(FramebufferWidth(), FramebufferHeight(), true);
	rlFramebufferAttach(r.fbo, r.tex_color, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(r.fbo, r.tex_depth, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
	assert(rlFramebufferComplete(r.fbo));

	if (r.flags & FLAG_MSAA_4X_HINT)
	{
		int sample_buffers, samples;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
		glGetIntegerv(GL_SAMPLES, &samples);
		assert(sample_buffers > 0 && samples == 4);
	}
}

void UnloadRenderer(Renderer& r)
{
	rlUnloadTexture(r.tex_depth);
	rlUnloadTexture(r.tex_color);
	rlUnloadFramebuffer(r.fbo);
}

int FramebufferWidth()
{
	return int(1920.0f * framebuffer_scale);
}

int FramebufferHeight()
{
	return int(1080.0f * framebuffer_scale);
}

#include "Renderer.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"
#include "Shaders.h"
#include <cassert>

static const float framebuffer_scale = 0.5f;

void LoadRenderer(Renderer& r)
{
	if (r.flags & FLAG_MSAA_4X_HINT)
	{
		int sample_buffers, samples;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
		glGetIntegerv(GL_SAMPLES, &samples);
		assert(sample_buffers > 0 && samples == 4);
	}

	const float rt_scale = 0.5f;
	r.rt_downsample = LoadRenderTexture(GetScreenWidth() * rt_scale, GetScreenHeight() * rt_scale);
	// LoadRenderTexture loads depth attachment as a renderbuffer instead of a texture (can't sample from rbo)
}

void UnloadRenderer(Renderer& r)
{
	UnloadRenderTexture(r.rt_downsample);
}

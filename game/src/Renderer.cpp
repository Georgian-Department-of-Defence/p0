#include "Renderer.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"
#include "Shaders.h"
#include <cassert>

static const float framebuffer_scale = 0.5f;

RenderTexture2D LoadShadowmapRenderTexture(int width, int height);

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

    r.rt_shadowmap = LoadShadowmapRenderTexture(GetScreenWidth(), GetScreenHeight());
}

void UnloadRenderer(Renderer& r)
{
    rlUnloadFramebuffer(r.rt_shadowmap.id);
	UnloadRenderTexture(r.rt_downsample);
}

RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
{
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(); // Load an empty framebuffer
    target.texture.width = width;
    target.texture.height = height;

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create depth texture
        // We don't need a color texture for the shadowmap
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach depth texture to FBO
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

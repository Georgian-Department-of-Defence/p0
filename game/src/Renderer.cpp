#include "Renderer.h"
#include "rlgl.h"
#include "glad.h"
#include <cassert>

Texture LoadColorBuffer(int width, int height, int format);
Texture LoadDepthBuffer(int width, int height, bool use_render_buffer = false);

void LoadRenderer(Renderer& r)
{
    // MSAA check
	if (r.flags & FLAG_MSAA_4X_HINT)
	{
		int sample_buffers, samples;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
		glGetIntegerv(GL_SAMPLES, &samples);
		assert(sample_buffers > 0 && samples == 4);
	}

    // Downsample, 360p
    {
        int rt_width = 640;
        int rt_height = 360;

        RenderTexture& rt = r.rt_downsample;
        rt.texture = LoadColorBuffer(rt_width, rt_height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        rt.depth = LoadDepthBuffer(rt_width, rt_height);

        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(rt.id, rt.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));
    }

    // Shadowmap, 4k
    {
        int rt_width = 3840;
        int rt_height = 2160;

        RenderTexture& rt = r.rt_shadowmap;
        rt.texture.width = rt_width;
        rt.texture.height = rt_height;
        rt.depth = LoadDepthBuffer(rt_width, rt_height);
        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));
    }
}

void UnloadRenderer(Renderer& r)
{
    UnloadRenderTexture(r.rt_shadowmap);
	UnloadRenderTexture(r.rt_downsample);
}

Texture LoadColorBuffer(int width, int height, int format)
{
    Texture texture;
    texture.width = width;
    texture.height = height;
    texture.format = format;
    texture.mipmaps = 1;
    texture.id = rlLoadTexture(nullptr, texture.width, texture.height, texture.format, texture.mipmaps);
    return texture;
}

Texture LoadDepthBuffer(int width, int height, bool use_render_buffer)
{
    Texture texture;
    texture.width = width;
    texture.height = height;
    texture.format = 19;
    texture.mipmaps = 1;
    texture.id = rlLoadTextureDepth(texture.width, texture.height, use_render_buffer);
    return texture;
}

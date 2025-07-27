#include "Renderer.h"
#include "raymathext.h"
#include "rlgl.h"
#include "glad.h"
#include "Shaders.h"
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
        int rt_scale = 1;
        int rt_width = 640 * rt_scale;
        int rt_height = 360 * rt_scale;

        RenderTexture& rt = r.rt_downsample;
        rt.texture = LoadColorBuffer(rt_width, rt_height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        rt.depth = LoadDepthBuffer(rt_width, rt_height);

        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(rt.id, rt.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));
    }

    // Shadowmap, same resolution as above buffer for no monkey business till we get shadowmapping first draft working
    {
        int rt_width = 640;//3840;
        int rt_height = 360;//2160;

        RenderTexture& rt = r.rt_shadowmap;
        rt.texture.width = rt_width;
        rt.texture.height = rt_height;
        rt.depth = LoadDepthBuffer(rt_width, rt_height);
        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));

        // Proof of concept:
        //Image img = GenImageGradientLinear(512, 512, 0, RED, BLUE);
        //Texture tex = LoadTextureFromImage(img);
        //g_materials.lighting.maps[MATERIAL_MAP_SPECULAR].texture = tex;
        g_materials.lighting.maps[MATERIAL_MAP_SPECULAR].texture = rt.depth;
    }
}

void UnloadRenderer(Renderer& r)
{
    UnloadRenderTexture(r.rt_shadowmap);
	UnloadRenderTexture(r.rt_downsample);
}

void DrawColor(RenderTexture rt)
{
    Rectangle src_rec;
    src_rec.x = 0;
    src_rec.y = 0;
    src_rec.width = rt.texture.width;
    src_rec.height = -rt.texture.height;

    Rectangle dst_rec;
    dst_rec.x = 0;
    dst_rec.y = 0;
    dst_rec.width = GetScreenWidth();
    dst_rec.height = GetScreenHeight();

    DrawTexturePro(rt.texture, src_rec, dst_rec, Vector2Zeros, 0.0f, WHITE);
}

void DrawDepth(RenderTexture rt)
{
    Rectangle src_rec;
    src_rec.x = 0;
    src_rec.y = 0;
    src_rec.width = rt.depth.width;
    src_rec.height = -rt.depth.height;

    Rectangle dst_rec;
    dst_rec.x = 0;
    dst_rec.y = 0;
    dst_rec.width = GetScreenWidth();
    dst_rec.height = GetScreenHeight();

    BeginShaderMode(g_shaders.depth);
        int loc_depth_tex = GetShaderLocation(g_shaders.depth, "tex_depth");
        int loc_z_near = GetShaderLocation(g_shaders.depth, "z_near");
        int loc_z_far = GetShaderLocation(g_shaders.depth, "z_far");
        float z_near = rlGetCullDistanceNear();
        float z_far = rlGetCullDistanceFar();
        SetShaderValue(g_shaders.depth, loc_z_near, &z_near, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValue(g_shaders.depth, loc_z_far, &z_far, RL_SHADER_UNIFORM_FLOAT);
        SetShaderValueTexture(g_shaders.depth, loc_depth_tex, rt.depth);
        DrawTexturePro(rt.depth, src_rec, dst_rec, Vector2Zeros, 0.0f, WHITE);
    EndShaderMode();
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

#include "Assets.h"

Assets assets;

Texture LoadColorBuffer(int width, int height, int format);
Texture LoadDepthBuffer(int width, int height, bool use_render_buffer = false);

void LoadAudio();
void LoadMeshes();
void LoadTextures();
void LoadMaterials();
void LoadFramebuffers();

void UnloadFramebuffers();
void UnloadMaterials();
void UnloadTextures();
void UnloadMeshes();
void UnloadAudio();

void LoadAssets()
{
    LoadAudio();
    LoadMeshes();
    LoadTextures();

    LoadMaterials();
}

void UnloadAssets()
{
    UnloadMaterials();
    UnloadTextures();
    UnloadMeshes();
    UnloadAudio();
}

void LoadAudio()
{
    Audios& audio = assets.audio;
    audio.fire_rifle = LoadSound("./assets/audio/fire_rifle.wav");
    audio.fire_shotgun = LoadSound("./assets/audio/fire_shotgun.wav");
    audio.fire_grenade = LoadSound("./assets/audio/fire_grenade.wav");
    audio.fire_missile = LoadSound("./assets/audio/fire_missile.wav");
    audio.fire_dasher = LoadSound("./assets/audio/fire_dasher.wav");

    audio.hit = LoadSound("./assets/audio/hit.wav");
    audio.hit_mech = LoadSound("./assets/audio/hit_mech.wav");

    audio.heat_overheat = LoadSound("./assets/audio/heat_overheat.wav");
    audio.heat_restore = LoadSound("./assets/audio/heat_restore.wav");
}

void UnloadAudio()
{
    Audios& audio = assets.audio;
    UnloadSound(audio.heat_restore);
    UnloadSound(audio.heat_overheat);

    UnloadSound(audio.hit_mech);
    UnloadSound(audio.hit);

    UnloadSound(audio.fire_dasher);
    UnloadSound(audio.fire_missile);
    UnloadSound(audio.fire_grenade);
    UnloadSound(audio.fire_shotgun);
    UnloadSound(audio.fire_rifle);
}

void LoadMeshes()
{
    auto LoadMesh = [](const char* file) -> Mesh
    {
        Model model = LoadModel(file);
        assert(model.meshCount == 1);
        Mesh mesh = model.meshes[0];
        model.meshCount = 0;
        UnloadModel(model);
        return mesh;
    };

    Meshes& mesh = assets.mesh;

    // Mech
    mesh.torso = LoadMesh("./assets/meshes/mech_torso.obj");
    mesh.legs = LoadMesh("./assets/meshes/mech_legs.obj");

    // Buildings
    mesh.td = LoadMesh("./assets/meshes/bld_td.obj");
    mesh.bmo = LoadMesh("./assets/meshes/bld_bmo.obj");
    mesh.condo = LoadMesh("./assets/meshes/bld_condo.obj");

    // Projectiles
    mesh.bullet = LoadMesh("./assets/meshes/prj_straight.obj");
    mesh.grenade = LoadMesh("./assets/meshes/prj_grenade.obj");
    mesh.missile = LoadMesh("./assets/meshes/prj_missile.obj");

    // Gear
    //Model gear_rifle;
    //Model gear_shotgun;
    //Model gear_grenade;
}

void UnloadMeshes()
{
    Meshes& mesh = assets.mesh;
    UnloadMesh(mesh.torso);
    UnloadMesh(mesh.legs);

    UnloadMesh(mesh.td);
    UnloadMesh(mesh.bmo);
    UnloadMesh(mesh.condo);

    UnloadMesh(mesh.bullet);
    UnloadMesh(mesh.grenade);
    UnloadMesh(mesh.missile);
}

void LoadMaterials()
{
    // "texture0", "texture1", and "texture2" are queried by default on-shader load.
    // ie if I want to sample a texture for shadow-mapping, add a uniform called texture1 and shader.locs[SHADER_LOC_MAP_SPECULAR].texture
    //shader.locs[SHADER_LOC_MAP_DIFFUSE] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);  // SHADER_LOC_MAP_ALBEDO / DIFFUSE
    //shader.locs[SHADER_LOC_MAP_SPECULAR] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1); // SHADER_LOC_MAP_METALNESS / SPECULAR
    //shader.locs[SHADER_LOC_MAP_NORMAL] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2);   // SHADER_LOC_MAP_NORMAL

    Shader skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
    Shader lighting = LoadShader("./assets/shaders/base.vs", "./assets/shaders/lighting.fs");
    Shader depth = LoadShader("./assets/shaders/base.vs", "./assets/shaders/depth.fs");

    Materials& material = assets.material;
    material.flat = LoadMaterialDefault();
    
    material.skinning = LoadMaterialDefault();
    material.skinning.shader = skinning;

    material.lighting = LoadMaterialDefault();
    material.lighting.shader = lighting;
    material.lighting.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(material.lighting.shader, "viewPos");
}

void UnloadMaterials()
{
    // TODO -- Test this (I'm confused about UnloadMaterial passing by-value, and if its valid to double-delete an OpenGL shader)...
    //auto UnloadMaterialSafe = [](Material material)
    //{
    //    if (IsMaterialValid(material))
    //        UnloadMaterial(material);
    //};

    Materials& material = assets.material;
    UnloadMaterial(material.lighting);
    UnloadMaterial(material.depth);
    UnloadMaterial(material.flat);
}

void LoadTextures()
{
    // White 1x1 texture
    {
        Image image = GenImageColor(1, 1, WHITE);
        assets.texture.white = LoadTextureFromImage(image);
        UnloadImage(image);

        // Alternatives:
        //  -LoadImageRaw() to read from char* of pixels
        //  -rlLoadTexture() to manually specify texture information
        //  -rlGetTextureIdDefault() contains a handle to a 1x1 white texture generated by raylib during startup
    }

    // Gradient test
    {
        Color src = RED;
        Color dst = ORANGE;
        src.a = dst.a = 128;

        Image image = GenImageGradientLinear(64, 64, 0, src, dst);
        assets.texture.gradient = LoadTextureFromImage(image);
        UnloadImage(image);
    }
}

void UnloadTextures()
{
    Textures& texture = assets.texture;
    UnloadTexture(texture.white);
    UnloadTexture(texture.gradient);
}

void LoadFramebuffers()
{
    // Shadow map
    {
        int rt_width = 4096;
        int rt_height = 4096;

        RenderTexture& rt = assets.framebuffer.shadow_map;
        rt.texture.width = rt_width;
        rt.texture.height = rt_height;
        rt.depth = LoadDepthBuffer(rt_width, rt_height);
        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));

        assets.material.lighting.maps[MATERIAL_MAP_SPECULAR].texture = rt.depth;
    }

    // Main multisample
    {
        int rt_width = 3840;
        int rt_height = 2160;
        int rt_samples = 8;
        RenderTexture& rt = assets.framebuffer.main_multisample;

        glGenTextures(1, &rt.texture.id);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rt.texture.id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, rt_samples, GL_RGBA, rt_width, rt_height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        glGenRenderbuffers(1, &rt.depth.id);
        glBindRenderbuffer(GL_RENDERBUFFER, rt.depth.id);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, rt_samples, GL_DEPTH_COMPONENT, rt_width, rt_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glGenFramebuffers(1, &rt.id);
        glBindFramebuffer(GL_FRAMEBUFFER, rt.id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, rt.texture.id, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rt.depth.id);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        assert(rlFramebufferComplete(rt.id));
        rt.texture.width = rt.depth.width = rt_width;
        rt.texture.height = rt.depth.height = rt_height;
    }

    // Main resolve
    {
        int rt_width = 3840;
        int rt_height = 2160;

        RenderTexture& rt = assets.framebuffer.main_resolve;
        rt.texture = LoadColorBuffer(rt_width, rt_height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        rt.depth = LoadDepthBuffer(rt_width, rt_height);

        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(rt.id, rt.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));
    }

    // Downsample
    {
        int rt_width = 640;
        int rt_height = 360;

        RenderTexture& rt = assets.framebuffer.downsample;
        rt.texture = LoadColorBuffer(rt_width, rt_height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        rt.id = rlLoadFramebuffer();
        rlFramebufferAttach(rt.id, rt.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        assert(rlFramebufferComplete(rt.id));
    }
}

void UnloadFramebuffers()
{
    Framebuffers& fb = assets.framebuffer;
    UnloadRenderTexture(fb.shadow_map);
    UnloadRenderTexture(fb.main_resolve);
    UnloadRenderTexture(fb.main_multisample);
    UnloadRenderTexture(fb.downsample);
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

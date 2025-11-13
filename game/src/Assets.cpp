#include "Assets.h"
#include "rlgl.h"
#include <cassert>

Assets assets;

static void LoadAudio();
static void LoadMeshes();
static void LoadShaders();
static void LoadTextures();

static void UnloadTextures();
static void UnloadShaders();
static void UnloadMeshes();
static void UnloadAudio();

void LoadAssets()
{
    LoadAudio();
    LoadMeshes();
    LoadShaders();
    LoadTextures();
}

void UnloadAssets()
{
    UnloadTextures();
    UnloadShaders();
    UnloadMeshes();
    UnloadAudio();
}

void LoadAudio()
{
    assets.audio.fire_rifle = LoadSound("./assets/audio/fire_rifle.wav");
    assets.audio.fire_shotgun = LoadSound("./assets/audio/fire_shotgun.wav");
    assets.audio.fire_grenade = LoadSound("./assets/audio/fire_grenade.wav");
    assets.audio.fire_missile = LoadSound("./assets/audio/fire_missile.wav");
    assets.audio.fire_dasher = LoadSound("./assets/audio/fire_dasher.wav");

    assets.audio.hit = LoadSound("./assets/audio/hit.wav");
    assets.audio.hit_mech = LoadSound("./assets/audio/hit_mech.wav");

    assets.audio.heat_overheat = LoadSound("./assets/audio/heat_overheat.wav");
    assets.audio.heat_restore = LoadSound("./assets/audio/heat_restore.wav");
}

void UnloadAudio()
{
    UnloadSound(assets.audio.heat_restore);
    UnloadSound(assets.audio.heat_overheat);

    UnloadSound(assets.audio.hit_mech);
    UnloadSound(assets.audio.hit);

    UnloadSound(assets.audio.fire_dasher);
    UnloadSound(assets.audio.fire_missile);
    UnloadSound(assets.audio.fire_grenade);
    UnloadSound(assets.audio.fire_shotgun);
    UnloadSound(assets.audio.fire_rifle);
}

void LoadMeshes()
{
    // Mech
	Model torso = LoadModel("./assets/meshes/mech_torso.obj");
	Model legs = LoadModel("./assets/meshes/mech_legs.obj");

    // Buildings
	Model td = LoadModel("./assets/meshes/bld_td.obj");
	Model bmo = LoadModel("./assets/meshes/bld_bmo.obj");
	Model condo = LoadModel("./assets/meshes/bld_condo.obj");

    // Projectiles
	Model bullet = LoadModel("./assets/meshes/prj_straight.obj");
	Model grenade = LoadModel("./assets/meshes/prj_grenade.obj");
	Model missile = LoadModel("./assets/meshes/prj_missile.obj");

    auto mesh_from_model = [](Model model) -> Mesh
    {
        assert(model.meshCount == 1);
        model.meshCount = 0;
        UnloadModel(model);
        return model.meshes[0];
    };

    // Gear
    //Model gear_rifle;
    //Model gear_shotgun;
    //Model gear_grenade;

    assets.mesh.torso = mesh_from_model(torso);
    assets.mesh.legs = mesh_from_model(legs);

    assets.mesh.td = mesh_from_model(td);
    assets.mesh.bmo = mesh_from_model(bmo);
    assets.mesh.condo = mesh_from_model(condo);

    assets.mesh.bullet = mesh_from_model(bullet);
    assets.mesh.grenade = mesh_from_model(grenade);
    assets.mesh.missile = mesh_from_model(missile);
}

void UnloadMeshes()
{
    UnloadMesh(assets.mesh.torso);
    UnloadMesh(assets.mesh.legs);

    UnloadMesh(assets.mesh.td);
    UnloadMesh(assets.mesh.bmo);
    UnloadMesh(assets.mesh.condo);

    UnloadMesh(assets.mesh.bullet);
    UnloadMesh(assets.mesh.grenade);
    UnloadMesh(assets.mesh.missile);
}

void LoadShaders()
{
    // "texture0", "texture1", and "texture2" are queried by default on-shader load.
    // ie if I want to sample a texture for shadow-mapping, add a uniform called texture1 and shader.locs[SHADER_LOC_MAP_SPECULAR].texture
    //shader.locs[SHADER_LOC_MAP_DIFFUSE] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);  // SHADER_LOC_MAP_ALBEDO / DIFFUSE
    //shader.locs[SHADER_LOC_MAP_SPECULAR] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1); // SHADER_LOC_MAP_METALNESS / SPECULAR
    //shader.locs[SHADER_LOC_MAP_NORMAL] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2);   // SHADER_LOC_MAP_NORMAL

    assets.shader.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
    assets.shader.lighting = LoadShader("./assets/shaders/base.vs", "./assets/shaders/lighting.fs");
    assets.shader.depth = LoadShader("./assets/shaders/base.vs", "./assets/shaders/depth.fs");

    g_materials.flat = LoadMaterialDefault();
    g_materials.lighting = LoadMaterialDefault();
    g_materials.lighting.shader = assets.shader.lighting;

    assets.shader.lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(assets.shader.lighting, "viewPos");
}

void UnloadShaders()
{
    UnloadShader(assets.shader.depth);
    UnloadShader(assets.shader.lighting);
    UnloadShader(assets.shader.skinning);

    // Prevent material unload from trying to unload associated shader
    g_materials.lighting.shader.id = rlGetShaderIdDefault();

    UnloadMaterial(g_materials.lighting);
    UnloadMaterial(g_materials.flat);
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
    UnloadTexture(assets.texture.white);
    UnloadTexture(assets.texture.gradient);
}

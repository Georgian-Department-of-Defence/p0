#include "Shaders.h"
#include "rlgl.h"

Shaders g_shaders;
Materials g_materials;

void LoadShaders()
{
	// "texture0", "texture1", and "texture2" are queried by default on-shader load.
	// ie if I want to sample a texture for shadow-mapping, add a uniform called texture1 and shader.locs[SHADER_LOC_MAP_SPECULAR].texture!
	//shader.locs[SHADER_LOC_MAP_DIFFUSE] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);  // SHADER_LOC_MAP_ALBEDO / DIFFUSE
	//shader.locs[SHADER_LOC_MAP_SPECULAR] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1); // SHADER_LOC_MAP_METALNESS / SPECULAR
	//shader.locs[SHADER_LOC_MAP_NORMAL] = rlGetLocationUniform(shader.id, RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2);   // SHADER_LOC_MAP_NORMAL

	g_shaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
	g_shaders.lighting = LoadShader("./assets/shaders/base.vs", "./assets/shaders/lighting.fs");
	g_shaders.shadow = LoadShader("./assets/shaders/base.vs", "./assets/shaders/shadowmap.fs");
	g_shaders.depth = LoadShader("./assets/shaders/base.vs", "./assets/shaders/depth.fs");

	g_materials.flat = LoadMaterialDefault();
	g_materials.lighting = LoadMaterialDefault();
	g_materials.lighting.shader = g_shaders.lighting;

	Image img = GenImageGradientLinear(512, 512, 0, RED, BLUE);
	Texture tex = LoadTextureFromImage(img);

	g_shaders.lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(g_shaders.lighting, "viewPos");
	g_materials.lighting.maps[MATERIAL_MAP_SPECULAR].texture = tex;
}

void UnloadShaders()
{
	UnloadShader(g_shaders.depth);
	UnloadShader(g_shaders.shadow);
	UnloadShader(g_shaders.lighting);
	UnloadShader(g_shaders.skinning);

	// Prevent material unload from trying to unload associated shader
	g_materials.lighting.shader.id = rlGetShaderIdDefault();
	
	UnloadMaterial(g_materials.lighting);
	UnloadMaterial(g_materials.flat);
}

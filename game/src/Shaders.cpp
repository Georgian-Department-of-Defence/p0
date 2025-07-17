#include "Shaders.h"
#include "rlgl.h"

Shaders g_shaders;
Materials g_materials;

void LoadShaders()
{
	g_shaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
	g_shaders.lighting = LoadShader("./assets/shaders/lighting.vs", "./assets/shaders/lighting.fs");
	g_shaders.shadow = LoadShader("./assets/shaders/shadowmap.vs", "./assets/shaders/shadowmap.fs");

	g_materials.flat = LoadMaterialDefault();
	g_materials.lighting = LoadMaterialDefault();
	g_materials.lighting.shader = g_shaders.lighting;

	g_shaders.lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(g_shaders.lighting, "viewPos");
}

void UnloadShaders()
{
	UnloadShader(g_shaders.shadow);
	UnloadShader(g_shaders.lighting);
	UnloadShader(g_shaders.skinning);

	// Prevent material unload from trying to unload associated shader
	g_materials.lighting.shader.id = rlGetShaderIdDefault();
	
	UnloadMaterial(g_materials.lighting);
	UnloadMaterial(g_materials.flat);
}

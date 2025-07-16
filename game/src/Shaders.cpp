#include "Shaders.h"
#include "rlgl.h"

Shaders g_shaders;
Materials g_materials;

void LoadShaders()
{
	g_shaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
	g_shaders.lighting = LoadShader("./assets/shaders/lighting.vs", "./assets/shaders/lighting.fs");
	g_shaders.shadow = LoadShader("./assets/shaders/shadowmap.vs", "./assets/shaders/shadowmap.fs");

	g_materials.mech = LoadMaterialDefault();
	g_materials.building = LoadMaterialDefault();
	g_materials.projectile = LoadMaterialDefault();

	//g_materials.building.shader = g_shaders.lighting;
}

void UnloadShaders()
{
	UnloadShader(g_shaders.shadow);
	UnloadShader(g_shaders.lighting);
	UnloadShader(g_shaders.skinning);

	// Prevent material unload from trying to unload associated shader
	g_materials.mech.shader.id = rlGetShaderIdDefault();
	g_materials.building.shader.id = rlGetShaderIdDefault();
	g_materials.projectile.shader.id = rlGetShaderIdDefault();

	UnloadMaterial(g_materials.mech);
	UnloadMaterial(g_materials.building);
	UnloadMaterial(g_materials.projectile);
}

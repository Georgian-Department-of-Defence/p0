#include "Shaders.h"

Shaders g_shaders;

void LoadShaders()
{
	g_shaders.skinning = LoadShader("./assets/shaders/skinning.vs", "./assets/shaders/skinning.fs");
	g_shaders.lighting = LoadShader("./assets/shaders/lighting.vs", "./assets/shaders/lighting.fs");
	g_shaders.shadow = LoadShader("./assets/shaders/shadowmap.vs", "./assets/shaders/shadowmap.fs");
}

void UnloadShaders()
{
	UnloadShader(g_shaders.shadow);
	UnloadShader(g_shaders.lighting);
	UnloadShader(g_shaders.skinning);
}

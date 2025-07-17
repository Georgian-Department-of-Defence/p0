#include "Renderer.h"
#include "raymathext.h"
#include "Shaders.h"

void LoadRenderer(Renderer& renderer)
{
    renderer.ambient = Vector4Ones * 0.25f;
    renderer.loc_ambient = GetShaderLocation(g_shaders.lighting, "ambient");
    SetShaderValue(g_shaders.lighting, renderer.loc_ambient, (float*)&renderer.ambient.x, SHADER_UNIFORM_VEC4);
}

void UnloadRenderer(Renderer& renderer)
{
}

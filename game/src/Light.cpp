#include "Light.h"
#include <cassert>

static int f_light_count = 0;

Light CreateLight(LightType type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    Light light;

    light.type = type;
    light.position = position;
    light.target = target;
    light.color = color;
    light.enabled = true;

    // NOTE: Lighting shader naming must be the provided ones
    light.loc_type = GetShaderLocation(shader, TextFormat("lights[%i].type", f_light_count));
    light.loc_position = GetShaderLocation(shader, TextFormat("lights[%i].position", f_light_count));
    light.loc_target = GetShaderLocation(shader, TextFormat("lights[%i].target", f_light_count));
    light.loc_color = GetShaderLocation(shader, TextFormat("lights[%i].color", f_light_count));
    light.loc_enabled = GetShaderLocation(shader, TextFormat("lights[%i].enabled", f_light_count));

    f_light_count++;
    assert(f_light_count <= MAX_LIGHTS);
    return light;
}

void UpdateLight(Light light, Shader shader)
{
    SetShaderValue(shader, light.loc_type, &light.type, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.loc_position, &light.position, SHADER_UNIFORM_VEC3);
    SetShaderValue(shader, light.loc_target, &light.target, SHADER_UNIFORM_VEC3);

    Vector4 color = ColorNormalize(light.color);
    SetShaderValue(shader, light.loc_color, &color, SHADER_UNIFORM_VEC4);

    SetShaderValue(shader, light.loc_enabled, &light.enabled, SHADER_UNIFORM_INT);
}

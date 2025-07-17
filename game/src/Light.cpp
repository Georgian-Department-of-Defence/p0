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

void GetLightUniforms(Light2& light, int light_index, Shader shader)
{
    light.loc_position = GetShaderLocation(shader, TextFormat("mechLights[%i].position", light_index));
    light.loc_color = GetShaderLocation(shader, TextFormat("mechLights[%i].color", light_index));
    
    light.loc_ambient = GetShaderLocation(shader, TextFormat("mechLights[%i].ambient", light_index));
    light.loc_diffuse = GetShaderLocation(shader, TextFormat("mechLights[%i].diffuse", light_index));
    light.loc_specular = GetShaderLocation(shader, TextFormat("mechLights[%i].specular", light_index));
    light.loc_specular_exponent = GetShaderLocation(shader, TextFormat("mechLights[%i].specularExponent", light_index));
    
    light.loc_radius = GetShaderLocation(shader, TextFormat("mechLights[%i].radius", light_index));
    light.loc_cutoff = GetShaderLocation(shader, TextFormat("mechLights[%i].cutoff", light_index));
    light.loc_direction = GetShaderLocation(shader, TextFormat("mechLights[%i].direction", light_index));
}

void UpdateLight2(Light2 light, Shader shader)
{
    SetShaderValue(shader, light.loc_position, &light.position, SHADER_UNIFORM_VEC3);
    SetShaderValue(shader, light.loc_color, &light.color, SHADER_UNIFORM_VEC3);

    SetShaderValue(shader, light.loc_ambient, &light.ambient, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.loc_diffuse, &light.diffuse, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.loc_specular, &light.specular, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.loc_specular_exponent, &light.specular_exponent, SHADER_UNIFORM_FLOAT);

    SetShaderValue(shader, light.loc_radius, &light.radius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.loc_cutoff, &light.cutoff, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, light.loc_direction, &light.direction, SHADER_UNIFORM_VEC3);
}

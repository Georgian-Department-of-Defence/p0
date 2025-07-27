#include "Light.h"
#include <cassert>

void LoadLightUniforms(Light& light, int light_index, Shader shader)
{
    light.loc_position = GetShaderLocation(shader, TextFormat("lights[%i].position", light_index));
    light.loc_color = GetShaderLocation(shader, TextFormat("lights[%i].color", light_index));
    
    light.loc_ambient = GetShaderLocation(shader, TextFormat("lights[%i].ambient", light_index));
    light.loc_diffuse = GetShaderLocation(shader, TextFormat("lights[%i].diffuse", light_index));
    light.loc_specular = GetShaderLocation(shader, TextFormat("lights[%i].specular", light_index));
    light.loc_specular_exponent = GetShaderLocation(shader, TextFormat("lights[%i].specularExponent", light_index));
    
    light.loc_radius = GetShaderLocation(shader, TextFormat("lights[%i].radius", light_index));
    light.loc_cutoff = GetShaderLocation(shader, TextFormat("lights[%i].cutoff", light_index));
    light.loc_direction = GetShaderLocation(shader, TextFormat("lights[%i].direction", light_index));

    light.loc_light_view_proj = GetShaderLocation(shader, "lightViewProj");
}

void UpdateLightUniforms(Light light, Shader shader)
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

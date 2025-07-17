#pragma once
#include "raylib.h"

enum LightType : int
{
    LIGHT_POINT,
    LIGHT_DIRECTIONAL,
    LIGHT_SPOTLIGHT,
    LIGHT_TYPE_COUNT
};

struct Light
{
    LightType type;
    Vector3 position;
    Vector3 color;

    float ambient;
    float diffuse;
    float specular;
    float specular_exponent;

    float radius;
    float cutoff;
    Vector3 direction;

    int loc_position;
    int loc_color;

    int loc_ambient;
    int loc_diffuse;
    int loc_specular;
    int loc_specular_exponent;

    int loc_radius;
    int loc_cutoff;
    int loc_direction;
};

void LoadLightUniforms(Light& light, int light_index, Shader shader);
void UpdateLightUniforms(Light light, Shader shader);

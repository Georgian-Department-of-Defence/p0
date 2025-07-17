#pragma once
#include "raylib.h"

constexpr int MAX_LIGHTS = 4;

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
    Vector3 target;
    Color color;
    int enabled;

    int loc_enabled;
    int loc_type;
    int loc_position;
    int loc_target;
    int loc_color;
};

struct Light2
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

Light CreateLight(LightType type, Vector3 position, Vector3 target, Color color, Shader shader);
void UpdateLight(Light light, Shader shader);

void GetLightUniforms(Light2& light, int light_index, Shader shader);
void UpdateLight2(Light2 light2, Shader shader);

// Better to create without constructors. Only uniform locations are necessary
//Light2 CreatePointLight()
//Light2 CreateDirectionLight();
//Light2 CreateSpotLight();

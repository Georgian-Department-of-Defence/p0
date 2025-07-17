#pragma once
#include "raylib.h"

constexpr int MAX_LIGHTS = 4;

enum LightType : int
{
    LIGHT_DIRECTIONAL,
    LIGHT_POINT
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

Light CreateLight(LightType type, Vector3 position, Vector3 target, Color color, Shader shader);
void UpdateLight(Light light, Shader shader);

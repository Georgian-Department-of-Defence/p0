#pragma once
#include "raylib.h"

struct Shaders
{
    Shader skinning;
    Shader lighting;
    Shader shadow;
};

struct Materials
{
    Material mech;
    Material building;
};

extern Shaders g_shaders;
extern Materials g_materials;

void LoadShaders();
void UnloadShaders();
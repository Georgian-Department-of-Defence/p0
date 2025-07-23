#pragma once
#include "raylib.h"

struct Shaders
{
    Shader skinning;
    Shader lighting;
    Shader shadow;
    Shader depth;
};

struct Materials
{
    Material flat;
    Material lighting;
};

extern Shaders g_shaders;
extern Materials g_materials;

void LoadShaders();
void UnloadShaders();
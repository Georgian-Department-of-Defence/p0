#pragma once
#include "raylib.h"

struct Shaders
{
    Shader skinning;
    Shader lighting;
    Shader shadow;
};

extern Shaders g_shaders;

void LoadShaders();
void UnloadShaders();
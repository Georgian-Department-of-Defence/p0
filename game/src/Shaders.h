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
    Material projectile;
};

// Make some sort of Renderer object that stores all uniform locations and maybe some methods to set all the data 
struct BuildingUniformData
{

};

extern Shaders g_shaders;
extern Materials g_materials;

void LoadShaders();
void UnloadShaders();
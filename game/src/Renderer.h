#pragma once
#include "raylib.h"

struct Renderer
{
    int loc_ambient;
    Vector4 ambient;
};

void LoadRenderer(Renderer& renderer);
void UnloadRenderer(Renderer& renderer);

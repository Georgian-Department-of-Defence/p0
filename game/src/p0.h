#pragma once

// 1) Include libc headers
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

// 2) Include raylib headers (raymath.h needs math.h so best to include libc before raylib)
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <glad.h>

// 3) Include STL headers
#include <vector>

// 4) Include user-defined headers
#include "MathExt.h"
#include "Constants.h"
#include "Assets.cpp"
#include "Camera.cpp"
#include "Renderer.cpp"
#include "DebugDraw.cpp"

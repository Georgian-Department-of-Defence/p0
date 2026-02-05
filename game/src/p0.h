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
#include <algorithm>

// 4) Include common headers (constants, math, collision, etc)
#include "Constants.h"
#include "Types.h"
#include "MathExt.h"
#include "Collision.h"
#include "Collider.h"

// 5) Include assets & tools
#include "Assets.cpp"
#include "Camera.cpp"
#include "Renderer.cpp"
#include "DebugDraw.cpp"
#include "ParticleEmitter.cpp"
#include "Light.cpp"

// 6) Include entities
#include "Entity.cpp"
#include "Mech2.h"

// 7) Include world
#include "World2.cpp"

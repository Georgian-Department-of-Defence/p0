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
#include "Collision2D.h"
#include "Collision3D.h"
#include "Collider.h"

// 5) Include assets & tools
#include "Assets.cpp"
#include "Camera.cpp"
#include "Renderer.cpp"
#include "DebugDraw.cpp"
#include "ParticleEmitter.cpp"
#include "Light.cpp"

// 6) Include entity declarations
#include "Entity.h"
#include "Mech2.h"
#include "Building2.h"
#include "Projectile2.h"

// 7) Include world
#include "World2.cpp"

// 8) Include entity definitions (now that world exists)
#include "Entity.cpp"
#include "Mech2.cpp"
#include "Building2.cpp"
#include "Projectile2.cpp"

#pragma once
#include "raylib.h"
#include "raymathext.h"
#include <cstdint>

constexpr float WORLD_MIN_X = -100.0f;
constexpr float WORLD_MAX_X = 100.0f;
constexpr float WORLD_MIN_Y = -50.0f;
constexpr float WORLD_MAX_Y = 50.0f;
constexpr float WORLD_MIN_Z = 0.0f;
constexpr float WORLD_MAX_Z = 1000.0f;
constexpr Vector3 GRAVITY = { 0.0f, 0.0f, -9.81f };

constexpr float MECH_GEAR_Z = 15.0f;

using Id = uint32_t;
inline Id GenId()
{
	static Id id = 0;
	return ++id;
}

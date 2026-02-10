#pragma once
constexpr Vector3 WORLD_MIN = { -100.0f, -50.0f, 0.0f };
constexpr Vector3 WORLD_MAX = {  100.0f,  50.0f, 1000.0f };
constexpr Vector3 GRAVITY = { 0.0f, 0.0f, -9.81f };

constexpr float MECH_GEAR_Z = 15.0f;

constexpr size_t MAX_MECHS = 4;
constexpr size_t MAX_BUILDINGS = 64;
constexpr size_t MAX_PROJECTILES = 256;
constexpr int MAX_LIGHTS = 1;

constexpr size_t ENTITY_COUNT_MECHS = 4;

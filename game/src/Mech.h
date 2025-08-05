#pragma once
#include "WorldDef.h"
#include "Gear.h"
#include "ParticleEmitter.h"
#include "Renderer.h"

constexpr size_t GEAR_COUNT = 4;

struct Mech
{
    uint32_t id = 0;
    int player = 0;

    Team team = TEAM_NONE;
    bool destroy = false;

    float health = 100.0f;

    float heat = 0.0f;
    float heat_max = 100.0f;
    float heat_dissipation = 20.0f;
    bool overheated = false;
    bool dashing = false;

    Vector3 pos = Vector3Zeros;
    Vector3 vel = Vector3Zeros;

    float turn_speed = 100.0f * DEG2RAD;
    float move_speed = 100.0f;
    float drag = 0.05f; // 0.0 = full drag, 1.0 = no drag

    float radius = 8.0f;
    Color color = WHITE;

    Quaternion legs_rotation;
    Quaternion torso_rotation;
    Quaternion legs_rotation_goal;
    Quaternion torso_rotation_goal;

    OnCollisionMechMech on_collision_mech = nullptr;
    OnCollisionMechBuilding on_collision_building = nullptr;
    OnCollisionMechProjectile on_collision_projectile = nullptr;

    Gear gear[GEAR_COUNT];
    Vector3 gear_positions[GEAR_COUNT];

    ParticleEmitter trail;

#ifdef DEBUG
    bool debug_collion = false;
    bool debug_poll_input = false;
#endif
};

void CreateMech(Mech* mech, int player);
void DestroyMech(Mech* mech);

void UpdateMech(Mech& mech, World& world);

void DrawMech(const Mech& mech, Material material, const Renderer& renderer);
void DrawMechDebug(const Mech& mech, const Renderer& renderer);

inline Vector3 TorsoDirection(const Mech& mech)
{
    return Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);
}

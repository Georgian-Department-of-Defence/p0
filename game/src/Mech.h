#pragma once
#include "WorldDef.h"
#include "Gear.h"
#include "ParticleEmitter.h"

struct Mech
{
    uint32_t id = 0;
    int player = 0;

    Team team = TEAM_NONE;
    bool destroy = false;

    float health = 100.0f;

    Vector3 pos = Vector3Zeros;
    Vector3 vel = Vector3Zeros;

    float turn_speed = 100.0f * DEG2RAD;
    float move_speed = 100.0f;
    float drag = 0.05f; // 0.0 = full drag, 1.0 = no drag

    float radius = 8.0f;

    Quaternion legs_rotation;
    Quaternion torso_rotation;
    Quaternion legs_rotation_goal;
    Quaternion torso_rotation_goal;

    Material material;

    OnCollisionMechMech on_collision_mech = nullptr;
    OnCollisionMechBuilding on_collision_building = nullptr;
    OnCollisionMechProjectile on_collision_projectile = nullptr;

    Gear gear[5];
    Vector3 gear_positions[4];

    ParticleEmitter trail;

#ifdef DEBUG
    bool debug_collion = false;
    bool debug_poll_input = false;
#endif
};

void CreateMech(Mech* mech, int player);
void DestroyMech(Mech* mech);

void UpdateMech(Mech& mech, World& world);

void DrawMech(const Mech& mech);
void DrawMechDebug(const Mech& mech);

inline Vector3 TorsoDirection(const Mech& mech)
{
    return Vector3RotateByQuaternion(Vector3UnitY, mech.torso_rotation);
}

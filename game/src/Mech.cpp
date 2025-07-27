#include "Mech.h"
#include "DebugDraw.h"
#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Textures.h"
#include "Audio.h"

#include "World.h"
#include <cassert>

void UpdateInputMove(Mech& mech);
void UpdateInputAim(Mech& mech);
void UpdateInputFire(Mech& mech, World& world);
void UpdateInput(Mech& mech, World& world);

void FireGear(Mech& mech, World& world, int slot);
void UpdateGear(Mech& mech, World& world, int slot);

void UpdateHeat(Mech& mech);
void UpdateMotion(Mech& mech);

void CreateMech(Mech* mech, int player)
{
    mech->id = GenId();
    mech->player = player;
    mech->team = player < 2 ? TEAM_RED : TEAM_BLUE;
    mech->color = mech->team == TEAM_RED ? RED : BLUE;

    float roll = player % 2 == 0 ? 0.0f : PI;
    Quaternion rotation = QuaternionFromEuler(0.0f, 0.0f, roll);

    mech->legs_rotation = rotation;
    mech->torso_rotation = rotation;
    mech->legs_rotation_goal = rotation;
    mech->torso_rotation_goal = rotation;

    // Default loadout / testing
    mech->gear[0] = CreateGearChainGun();
    mech->gear[1] = CreateGearShotgun();
    mech->gear[2] = CreateGearGrenadeLauncher();
    mech->gear[3] = CreateGearMissileLauncher();

    ParticleEmitter& pe = mech->trail;
    pe.spawn_rate = 10.0f;
    pe.lifetime = 2.0f;
    pe.speed = 10.0f;
    pe.size = 2.0f;
    pe.shape_type = PARTICLE_SHAPE_SPHERE;
    pe.shape.sphere.radius = 5.0f;

    Vector3 positions[GEAR_COUNT];
    positions[0] = { -20.0f, -40.0f, 0.0f };
    positions[1] = { -20.0f,  40.0f, 0.0f };
    positions[2] = {  20.0f, -40.0f, 0.0f };
    positions[3] = {  20.0f,  40.0f, 0.0f };
    mech->pos = positions[player];

#ifdef DEBUG
    mech->debug_poll_input = player == 0;
#endif
}

void DestroyMech(Mech* mech)
{
    mech->id = 0;
    mech->team = TEAM_NONE;
}

void UpdateMech(Mech& mech, World& world)
{
    assert(mech.id != 0);
    assert(mech.team != TEAM_NONE);

    UpdateInput(mech, world);
    
    for (int i = 0; i < GEAR_COUNT; i++)
        UpdateGear(mech, world, i);

    UpdateHeat(mech);
    UpdateMotion(mech);
}

void DrawMech(const Mech& mech, const Renderer& renderer)
{
    Matrix translation = MatrixTranslate(mech.pos.x, mech.pos.y, mech.pos.z);

    Matrix torso_rotation = QuaternionToMatrix(mech.torso_rotation);
    Matrix legs_rotation = QuaternionToMatrix(mech.legs_rotation);

    Matrix torso_world = torso_rotation * translation;
    Matrix legs_world = legs_rotation * translation;

    Material& material = g_materials.lighting;
    MaterialMap& map = material.maps[MATERIAL_MAP_DIFFUSE];
    map.color = mech.color;
    //map.color = WHITE;//mech.color;
    //map.texture = mech.team == TEAM_RED ? g_textures.mech_red : g_textures.mech_blue;

    DrawMesh(*g_meshes.mech_torso, material, torso_world);
    DrawMesh(*g_meshes.mech_legs, material, legs_world);
}

void DrawMechDebug(const Mech& mech, const Renderer& renderer)
{
#if DEBUG
    Color color = mech.debug_collion ? SKYBLUE : mech.color;
    color.a = 128;

    //DrawAxesDebug(mech.pos, QuaternionToMatrix(mech.torso_rotation), 25.0f, 10.0f);
    for (int i = 0; i < 4; i++)
        DrawSphere(mech.gear_positions[i], 0.5f, DARKGREEN);

    //DrawSphere(mech.pos, 25.0f, DARKBLUE);
#endif
}

void UpdateInputAim(Mech& mech)
{
    const float deadzone = 0.1f;
    Vector2 input = Vector2Zeros;
    input.x = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_RIGHT_X));
    input.y = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_RIGHT_Y));
    input.x = fabsf(input.x) >= deadzone ? input.x : 0.0f;
    input.y = fabsf(input.y) >= deadzone ? input.y : 0.0f;
    input.y *= -1.0f;

    if (Vector2Length(input) >= deadzone)
    {
        Vector2 dir = Vector2Normalize(input);
        float roll = Vector2Angle(Vector2UnitY, dir);
        mech.torso_rotation_goal = QuaternionFromEuler(0.0f, 0.0f, roll);
        mech.torso_rotation = QuaternionRotateTowards(mech.torso_rotation, mech.torso_rotation_goal, 100.0f * DEG2RAD * GetFrameTime());
    }
}

void UpdateInput(Mech& mech, World& world)
{
    bool poll_input = true;
#ifdef DEBUG
    poll_input = mech.debug_poll_input;
#endif
    if (poll_input)
    {
        if (IsGamepadAvailable(mech.player))
        {
            UpdateInputAim(mech);
            UpdateInputMove(mech);
            UpdateInputFire(mech, world);
        }
        else
        {
            TraceLog(LOG_WARNING, "Player %i gamepad polled but not connected", mech.player);
        }
    }
}

void UpdateInputMove(Mech& mech)
{
    const float deadzone = 0.1f;
    Vector2 input = Vector2Zeros;
    input.x = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_LEFT_X));
    input.y = (GetGamepadAxisMovement(mech.player, GAMEPAD_AXIS_LEFT_Y));
    input.x = fabsf(input.x) >= deadzone ? input.x : 0.0f;
    input.y = fabsf(input.y) >= deadzone ? input.y : 0.0f;
    input.y *= -1.0f;

    if (Vector2Length(input) >= deadzone)
    {
        float dt = GetFrameTime();
        Vector2 dir = Vector2Normalize(input);
        float roll = Vector2Angle(Vector2UnitY, dir);
        mech.legs_rotation_goal = QuaternionFromEuler(0.0f, 0.0f, roll);
        mech.legs_rotation = QuaternionRotateTowards(mech.legs_rotation, mech.legs_rotation_goal, 250.0f * DEG2RAD * dt);

        mech.vel += Vector3{ dir.x, dir.y, 0.0f } * mech.move_speed * dt;
        mech.vel = Vector3Clamp(mech.vel, { -10.0f, -10.0f, 0.0f }, { 10.0f, 10.0f, 0.0f });
    }
}

void UpdateInputFire(Mech& mech, World& world)
{
    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        FireGear(mech, world, 0);
    }

    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
    {
        FireGear(mech, world, 1);
    }

    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    {
        FireGear(mech, world, 2);
    }

    if (IsGamepadButtonDown(mech.player, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        FireGear(mech, world, 3);
    }
}

void FireGear(Mech& mech, World& world, int slot)
{
    Gear& gear = mech.gear[slot];
    Vector3 gear_position = mech.gear_positions[slot];

    if (gear.cooldown <= 0.0f && !mech.overheated)
    {
        gear.cooldown = gear.cooldown_max;
        mech.heat += gear.heat;

        switch (gear.type)
        {
        case GEAR_RIFLE:
            CreateProjectileRifle(mech, world, gear_position);
            break;

        case GEAR_SHOTGUN:
            CreateProjectileShotgun(mech, world, gear_position);
            break;

        case GEAR_GRENADE_LAUNCHER:
            gear.grenade_launcher.grenades = 6;
            break;

        case GEAR_MISSILE_LAUNCHER:
            gear.missile_launcher.missiles = 3;
            gear.missile_launcher.launch_roll = 20.0f * DEG2RAD;
            break;

        case GEAR_MACHINEGUN:
            CreateProjectileMachineGun(mech, world, gear_position);
            break;

        case GEAR_CHAINGUN:
            gear.chain_gun.ramp_up += 0.08f;
            gear.cooldown -= gear.chain_gun.ramp_up;
            CreateProjectileChainGun(mech, world, gear_position);
            break;

        case GEAR_TYPE_COUNT:
            assert(false, "Invalid gear type!");
            break;
        }
    }
}

void UpdateGear(Mech& mech, World& world, int slot)
{
    Gear& gear = mech.gear[slot];
    Vector3 gear_position = mech.gear_positions[slot];

    float dt = GetFrameTime();
    gear.cooldown -= dt;

    if (gear.type == GEAR_GRENADE_LAUNCHER)
    {
        GearGrenadeLauncher& g = gear.grenade_launcher;
        g.launch_cooldown -= dt;
        if (g.launch_cooldown <= 0.0f && g.grenades > 0)
        {
            CreateProjectileGrenade(mech, world, gear_position);
            g.launch_cooldown = g.launch_cooldown_max;
            g.grenades--;
        }
    }
    else if (gear.type == GEAR_MISSILE_LAUNCHER)
    {
        GearMissileLauncher& g = gear.missile_launcher;
        g.launch_cooldown -= dt;
        if (g.launch_cooldown <= 0.0f && g.missiles > 0)
        {
            CreateProjectileMissile(mech, world, gear_position, g.launch_roll);
            g.launch_roll -= 20.0f * DEG2RAD; // 20, 0, -20 
            g.launch_cooldown = g.launch_cooldown_max;
            g.missiles--;
        }
    }

    else if (gear.type == GEAR_CHAINGUN)
    {
        if (gear.chain_gun.ramp_up >= 0) 
        {
            gear.chain_gun.ramp_up -= dt;
            Clamp(gear.chain_gun.ramp_up, 0.0f, 1.0f);
        }
        
    }

}

void UpdateHeat(Mech& mech)
{
    if (!mech.overheated && mech.heat >= mech.heat_max)
    {
        mech.overheated = true;
        PlaySound(g_audio.heat_overheat);

    }
    else if (mech.overheated && mech.heat <= 0.0f)
    {
        mech.overheated = false;
        PlaySound(g_audio.heat_restore);
    }

    mech.heat -= mech.heat_dissipation * GetFrameTime();
    mech.heat = Clamp(mech.heat, 0.0f, mech.heat_max);
}

void UpdateMotion(Mech& mech)
{
    // Physics update
    float dt = GetFrameTime();
    mech.vel *= powf(mech.drag, dt);
    mech.pos += mech.vel * dt;

    BoundingBox world_box = WorldBox();
    mech.pos = Vector3Clamp(mech.pos, world_box.min, world_box.max);

    // Gear position update (after mech moves)
    {
        Vector3 mount_offsets[GEAR_COUNT];
        mount_offsets[0] = { -8.0f, 5.0f, MECH_GEAR_Z };
        mount_offsets[1] = { -3.0f, 2.0f, MECH_GEAR_Z };
        mount_offsets[2] = { 3.0f, 2.0f, MECH_GEAR_Z };
        mount_offsets[3] = { 8.0f, 5.0f, MECH_GEAR_Z };

        Matrix rotation = QuaternionToMatrix(mech.torso_rotation);
        for (int i = 0; i < GEAR_COUNT; i++)
            mech.gear_positions[i] = mech.pos + Vector3Transform(mount_offsets[i], rotation);
    }
}

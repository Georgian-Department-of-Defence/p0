#include "World.h"
#include "rlgl.h"
#include "Camera.h"
#include "Meshes.h"
#include "Shaders.h"
#include "Audio.h"
#include "Map.h"

#include "Collision.h"
#include "Collision3D.h"
#include <algorithm>
#include <cassert>

constexpr size_t MAX_MECHS = 4;
constexpr size_t MAX_BUILDINGS = 64;
constexpr size_t MAX_PROJECTILES = 256;

void UpdateDebug(World& world);

void UpdateEntities(World& world);
void UpdateParticles(World& world);

void DrawEntities(const World& world);
void DrawParticles(const World& world);

void UpdateCollisionsMechMech(Mechs& mechs);
void UpdateCollisionsMechBuilding(Mechs& mechs, Buildings& buildings);
void UpdateCollisionsMechProjectile(Mechs& mechs, Projectiles& projectiles);
void UpdateCollisionsProjectileBuilding(Projectiles& projectiles, Buildings& buildings);

static void OnCollisionMechMechDefault(Mech& a, Mech& b, HitInfo hi);
static void OnCollisionMechBuildingDefault(Mech& mech, Building& building, HitInfo hi);
static void OnCollisionMechProjectileDefault(Mech& mech, Projectile& projectile, HitInfo hi);
static void OnCollisionProjectileBuildingDefault(Projectile& projectile, Building& building, HitInfo hi);

static void OnDestroyMech(Mech& mech, World& world);
static void OnDestroyBuilding(Building& building, World& world);
static void OnDestroyProjectile(Projectile& projectile, World& world);

void DestroyEntities(World& world);

void LoadWorld(World& world)
{
	world.mechs.reserve(MAX_MECHS);
	world.buildings.reserve(MAX_BUILDINGS);
	world.projectiles.reserve(MAX_PROJECTILES);

    for (int i = 0; i < 4; i++)
    {
        Mech mech;
        CreateMech(&mech, i);
        world.mechs.push_back(mech);
    }

    LoadMap(MAP_TEST_1, world);

    // Successful uniform test
    Vector4 col = { 1.0f, 0.2f, 1.0f, 1.0f };
    int loc = GetShaderLocation(g_shaders.lighting, "test");
    SetShaderValue(g_shaders.lighting, loc, (float*)&col.x, SHADER_UNIFORM_VEC4);
}

void UnloadWorld(World& world)
{
    for (Mech& mech : world.mechs)
        mech.destroy |= true;

    for (Building& building : world.buildings)
        building.destroy |= true;

    for (Projectile& projectile : world.projectiles)
        projectile.destroy |= true;

    DestroyEntities(world);
    assert(world.mechs.empty());
    assert(world.buildings.empty());
    assert(world.projectiles.empty());
}

void UpdateWorld(World& world)
{
    UpdateDebug(world);

    UpdateCollisionsMechMech(world.mechs);
    UpdateCollisionsMechBuilding(world.mechs, world.buildings);
    UpdateCollisionsMechProjectile(world.mechs, world.projectiles);
    UpdateCollisionsProjectileBuilding(world.projectiles, world.buildings);

    UpdateEntities(world);
    UpdateParticles(world);

    DestroyEntities(world);
}

void DrawWorld(const World& world)
{
    BeginMode3D(*GetCamera());

        rlPushMatrix();
        rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        rlTranslatef(50.0f, 0.0f, 0.0f);
        DrawGrid(100, 1.0f);
        rlPopMatrix();

        rlPushMatrix();
        rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        rlTranslatef(-50.0f, 0.0f, 0.0f);
        DrawGrid(100, 1.0f);
        rlPopMatrix();
        
        DrawEntities(world);
        DrawParticles(world);

    EndMode3D();
}

void DrawWorldDebug(const World& world)
{
    // TODO - 2D vs 3D debug draw for lines vs text
    BeginMode3D(*GetCamera());

        for (const Mech& mech : world.mechs)
            DrawMechDebug(mech);

        for (const Building& building : world.buildings)
            DrawBuildingDebug(building);

        for (const Projectile& projectile : world.projectiles)
            DrawProjectileDebug(projectile);

    EndMode3D();
}

void DrawEntities(const World& world)
{
    for (const Mech& mech : world.mechs)
        DrawMech(mech);

    for (const Building& building : world.buildings)
        DrawBuilding(building);

    for (const Projectile& projectile : world.projectiles)
        DrawProjectile(projectile);
}

void DrawParticles(const World& world)
{
    for (const Mech& mech : world.mechs)
        DrawParticleEmitter(mech.trail, *GetCamera());

    for (const Building& building : world.buildings)
    {
    }

    for (const Projectile& projectile : world.projectiles)
        DrawParticleEmitter(projectile.trail, *GetCamera());
}

void UpdateDebug(World& world)
{
#ifdef DEBUG
    for (Mech& mech : world.mechs)
    {
        mech.debug_collion = false;
    }

    for (Building& building : world.buildings)
    {
        building.debug_collion = false;
    }

    for (Projectile& projectile : world.projectiles)
    {
        projectile.debug_collion = false;
    }
#endif
}

void UpdateEntities(World& world)
{
    for (Mech& mech : world.mechs)
        UpdateMech(mech, world);

    for (Building& building : world.buildings)
        UpdateBuilding(building);

    for (Projectile& projectile : world.projectiles)
        UpdateProjectile(projectile, world);
}

void UpdateParticles(World& world)
{
    for (Mech& mech : world.mechs)
    {
        ParticleEmitter& pe = mech.trail;
        pe.position = mech.pos;
        pe.direction = TorsoDirection(mech) * -1.0f;
        UpdateParticleEmitter(pe);
    }

    for (Building& building : world.buildings)
    {

    }

    for (Projectile& projectile : world.projectiles)
    {
        ParticleEmitter& pe = projectile.trail;
        pe.position = projectile.pos;
        pe.direction = Vector3Normalize(projectile.vel) * -1.0f;
        UpdateParticleEmitter(pe);
    }
}

void UpdateCollisionsMechMech(Mechs& mechs)
{
    for (size_t i = 0; i < mechs.size(); i++)
    {
        for (size_t j = 0; j < mechs.size(); j++)
        {
            if (i == j) continue;
            Mech& self = mechs[i];
            Mech& other = mechs[j];

            HitInfo hi;
            Vector3 mtv = Vector3Zeros;

            Vector2* pA = (Vector2*)&self.pos;
            Vector2* pB = (Vector2*)&other.pos;

            // TODO - Respond to mech-mech collision
            bool collision = CircleCircle(*pA, self.radius, *pB, other.radius, (Vector2*)&mtv);
            if (collision)
            {
                OnCollisionMechMechDefault(self, other, hi);

                if (self.on_collision_mech != nullptr)
                    self.on_collision_mech(self, other, hi);
            }

#ifdef DEBUG
            self.debug_collion |= collision;
            other.debug_collion |= collision;
#endif
        }
    }
}

void UpdateCollisionsMechBuilding(Mechs& mechs, Buildings& buildings)
{
    for (Mech& mech : mechs)
    {
        for (Building& building : buildings)
        {
            HitInfo hi;
            Vector3 mtv = Vector3Zeros;

            Vector2* pA = (Vector2*)&mech.pos;
            Vector2* pB = (Vector2*)&building.pos;

            bool collision = CircleCircle(*pA, mech.radius, *pB, building.radius, (Vector2*)&mtv);
            //bool collision = CircleRectangle(*pA, mech.radius, *pB, *(Vector2*)&building.extents.x, (Vector2*)&mtv);
            mech.pos += mtv;
            if (collision)
            {
                OnCollisionMechBuildingDefault(mech, building, hi);

                if (mech.on_collision_building != nullptr)
                    mech.on_collision_building(mech, building, hi);

                if (building.on_collision_mech != nullptr)
                    building.on_collision_mech(mech, building, hi);
            }

#ifdef DEBUG
            mech.debug_collion |= collision;
            building.debug_collion |= collision;
#endif
        }
    }
}

void UpdateCollisionsMechProjectile(Mechs& mechs, Projectiles& projectiles)
{
    for (Mech& mech : mechs)
    {
        for (Projectile& projectile : projectiles)
        {
            HitInfo hi;
            Vector3 mtv = Vector3Zeros;

            Vector2* pA = (Vector2*)&mech.pos;
            Vector2* pB = (Vector2*)&projectile.pos;

            bool collision = CircleCircle(*pA, mech.radius, *pB, projectile.radius, (Vector2*)&mtv) && projectile.pos.z <= MECH_GEAR_Z;
            if (collision)
            {
                OnCollisionMechProjectileDefault(mech, projectile, hi);
                
                if (mech.on_collision_projectile != nullptr)
                    mech.on_collision_projectile(mech, projectile, hi);
                
                if (projectile.on_collision_mech != nullptr)
                    projectile.on_collision_mech(mech, projectile, hi);
            }

#ifdef DEBUG
            mech.debug_collion |= collision;
            projectile.debug_collion |= collision;
#endif
        }
    }
}

void UpdateCollisionsProjectileBuilding(Projectiles& projectiles, Buildings& buildings)
{
    for (Projectile& projectile : projectiles)
    {
        for (Building& building : buildings)
        {
            HitInfo hi;
            Vector3 mtv = Vector3Zeros;

            Vector2* pA = (Vector2*)&projectile.pos;
            Vector2* pB = (Vector2*)&building.pos;

            //bool collision =
            //    CircleRectangle(*pA, projectile.radius, *pB, *(Vector2*)&building.extents.x, (Vector2*)&mtv) &&
            //    projectile.pos.z <= building.extents.z;

            bool collision = CircleCircle(*pA, projectile.radius, *pB, building.radius, (Vector2*)&mtv) && projectile.pos.z <= building.height;
            if (collision)
            {
                OnCollisionProjectileBuildingDefault(projectile, building, hi);

                if (building.on_collision_projectile != nullptr)
                    building.on_collision_projectile(projectile, building, hi);

                if (projectile.on_collision_building != nullptr)
                    projectile.on_collision_building(projectile, building, hi);
            }

#ifdef DEBUG
            projectile.debug_collion |= collision;
            building.debug_collion |= collision;
#endif
        }
    }
}

void OnCollisionMechMechDefault(Mech& self, Mech& other, HitInfo hi)
{

}

void OnCollisionMechBuildingDefault(Mech& mech, Building& building, HitInfo hi)
{

}

void OnCollisionMechProjectileDefault(Mech& mech, Projectile& projectile, HitInfo hi)
{
    if (projectile.team != mech.team)
    {
        projectile.target_mech_id = mech.id;
        projectile.mech_hit = true;
        projectile.destroy |= true;
    }
    // Note - Move all destruction logic to OnDestroyProjectile.
    // Collision isn't the only case for projectile destruction (ie proximity-based missile), so simply flag for destruction here and handle in OnDestroy
    // Example: no longer playing mech hit sound in on-collision, deferring till projectile on-destroy with target-hit
}

void OnCollisionProjectileBuildingDefault(Projectile& projectile, Building& building, HitInfo hi)
{
    projectile.destroy |= true;
    building.durability -= 25.0f;
    PlaySound(g_audio.hit);
}

void OnDestroyMech(Mech& mech, World& world)
{
    // TODO -- Spawn particles
}

void OnDestroyBuilding(Building& building, World& world)
{
    // TODO -- Spawn particles
}

void OnDestroyProjectile(Projectile& projectile, World& world)
{
    Mech* hit_mech = GetMechById(projectile.target_mech_id, world);

    if (hit_mech != nullptr)
    {
        hit_mech->health -= projectile.damage;
        PlaySound(g_audio.hit_mech);
    }
    else
    {
        PlaySound(g_audio.hit);
    }
}

void DestroyEntities(World& world)
{
    // Separate between alive entities vs entities pending destruction
    Mechs::iterator mech_partition_point = std::partition(world.mechs.begin(), world.mechs.end(), [](Mech& mech) { return !mech.destroy; });
    Buildings::iterator building_partition_point = std::partition(world.buildings.begin(), world.buildings.end(), [](Building& building) { return !building.destroy; });
    Projectiles::iterator projectile_partition_point = std::partition(world.projectiles.begin(), world.projectiles.end(), [](Projectile& projectile) { return !projectile.destroy; });

    // Determine alive entities count
    size_t mech_enabled_count = std::distance(world.mechs.begin(), mech_partition_point);
    size_t building_enabled_count = std::distance(world.buildings.begin(), building_partition_point);
    size_t projectile_enabled_count = std::distance(world.projectiles.begin(), projectile_partition_point);

    // Perform game destroy-logic before deleting dynamic memory
    for (size_t i = mech_enabled_count; i < world.mechs.size(); i++)
        OnDestroyMech(world.mechs[i], world);
    for (size_t i = building_enabled_count; i < world.buildings.size(); i++)
        OnDestroyBuilding(world.buildings[i], world);
    for (size_t i = projectile_enabled_count; i < world.projectiles.size(); i++)
        OnDestroyProjectile(world.projectiles[i], world);

    // Delete dynamic memory
    for (size_t i = mech_enabled_count; i < world.mechs.size(); i++)
        DestroyMech(&world.mechs[i]);
    for (size_t i = building_enabled_count; i < world.buildings.size(); i++)
        DestroyBuilding(&world.buildings[i]);
    for (size_t i = projectile_enabled_count; i < world.projectiles.size(); i++)
        DestroyProjectile(&world.projectiles[i]);

    // Remove entities from pools
    world.mechs.erase(mech_partition_point, world.mechs.end());
    world.buildings.erase(building_partition_point, world.buildings.end());
    world.projectiles.erase(projectile_partition_point, world.projectiles.end());
}

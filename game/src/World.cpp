#include "World.h"
#include "rlgl.h"
#include "Camera.h"
#include "Collision3D.h"
#include <algorithm>

constexpr size_t MAX_MECHS = 4;
constexpr size_t MAX_BUILDINGS = 64;
constexpr size_t MAX_PROJECTILES = 256;

void LoadMechs(Mechs& mechs);
void LoadBuildings(Buildings& buildings);
void LoadProjectiles(Projectiles& projectiles);

void UnloadProjectiles(Projectiles& projectiles);
void UnloadBuildings(Buildings& buildings);
void UnloadMechs(Mechs& mechs);

void UpdateDebug(World& world);

void UpdateParticles(World& world);
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

void LoadWorld(World& world)
{
	world.mechs.reserve(MAX_MECHS);
	world.buildings.reserve(MAX_BUILDINGS);
	world.projectiles.reserve(MAX_PROJECTILES);

    LoadMechs(world.mechs);
    LoadBuildings(world.buildings);
    LoadProjectiles(world.projectiles);
}

void UnloadWorld(World& world)
{
    UnloadProjectiles(world.projectiles);
    UnloadBuildings(world.buildings);
    UnloadMechs(world.mechs);

	world.projectiles.clear();
	world.buildings.clear();
	world.mechs.clear();
}

void UpdateWorld(World& world)
{
    // 1. Detect collisions and dispatch collision callbacks
    // 2. Update entities
    // 3. Destroy entities, then remove them from world
    UpdateDebug(world);

    UpdateCollisionsMechMech(world.mechs);
    UpdateCollisionsMechBuilding(world.mechs, world.buildings);
    UpdateCollisionsMechProjectile(world.mechs, world.projectiles);
    UpdateCollisionsProjectileBuilding(world.projectiles, world.buildings);

    for (Mech& mech : world.mechs)
        UpdateMech(mech, world);

    for (Building& building : world.buildings)
        UpdateBuilding(building);

    for (Projectile& projectile : world.projectiles)
        UpdateProjectile(projectile, world);

    UpdateParticles(world);

    for (Mech& mech : world.mechs)
    {
        if (mech.destroy)
        {
            OnDestroyMech(mech, world);             // Game logic
            DestroyMech(&mech);                     // Delete mech-specific dynamic memory
        }
    }

    for (Building& building : world.buildings)
    {
        if (building.destroy)
        {
            OnDestroyBuilding(building, world);     // Game logic
            DestroyBuilding(&building);             // Delete building-specific dynamic memory
        }
    }

    for (Projectile& projectile : world.projectiles)
    {
        if (projectile.destroy)
        {
            OnDestroyProjectile(projectile, world); // Game logic
            DestroyProjectile(&projectile);         // Delete projectile-specific dynamic memory
        }
    }

    world.mechs.erase
    (
        std::remove_if(world.mechs.begin(), world.mechs.end(), [](Mech& mech) { return mech.destroy; }),
        world.mechs.end()
    );

    world.buildings.erase
    (
        std::remove_if(world.buildings.begin(), world.buildings.end(), [](Building& building) { return building.destroy; }),
        world.buildings.end()
    );

    world.projectiles.erase
    (
        std::remove_if(world.projectiles.begin(), world.projectiles.end(), [](Projectile& projectile) { return projectile.destroy; }),
        world.projectiles.end()
    );

    // TODO - Time-based projectile destruction
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

        for (const Mech& mech : world.mechs)
            DrawMech(mech);

        for (const Building& building : world.buildings)
            DrawBuilding(building);

        for (const Projectile& projectile : world.projectiles)
            DrawProjectile(projectile);

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

void LoadMechs(Mechs& mechs)
{
    for (int i = 0; i < 4; i++)
    {
        Mech mech;
        CreateMech(&mech, i);
        mechs.push_back(mech);
    }
}

void LoadBuildings(Buildings& buildings)
{
    //float step = 20.0f; <-- changed to 40 for missile testing
    float step = 40.0f;
    for (float y = -40.0f; y <= 40.0f; y += step)
    {
        for (float x = -80.0f; x <= 80.0f; x += step)
        {
            // Temporarily omit centre building for visibility
            //if (x == 0.0f && y == 0.0f) continue;

            Building building;
            building.pos = { x, y, 0.0f };
            CreateBuilding(&building, BUILDING_CONDO);
            buildings.push_back(building);
        }
    }
}

void LoadProjectiles(Projectiles& projectiles)
{
}

void UnloadProjectiles(Projectiles& projectiles)
{
}

void UnloadBuildings(Buildings& buildings)
{
    for (Building& building : buildings)
        DestroyBuilding(&building);
}

void UnloadMechs(Mechs& mechs)
{
    for (Mech& mech : mechs)
        DestroyMech(&mech);
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
            bool collision = false;

            if (collision)
            {
                Vector3 mtv = Vector3Zeros;
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
            Vector3 mtv;

            bool collision = SphereCapsule(
                mech.pos, mech.radius,
                building.pos + Vector3UnitZ * building.length * 0.5f, Vector3UnitZ, building.radius, building.length * 0.5f - building.radius, &mtv);

            if (collision)
            {
                mtv.z = 0.0f;
                mech.pos += mtv;

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
            bool collision = SphereSphere(
                mech.pos, mech.radius,
                projectile.pos, projectile.radius);

            if (collision)
            {
                Vector3 mtv = Vector3Zeros;
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
            bool collision = SphereCapsule(
                projectile.pos, projectile.radius,
                building.pos + Vector3UnitZ * building.length * 0.5f, Vector3UnitZ, building.radius, building.length * 0.5f - building.radius);

            if (collision)
            {
                Vector3 mtv = Vector3Zeros;
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
    
}

void OnCollisionProjectileBuildingDefault(Projectile& projectile, Building& building, HitInfo hi)
{
    building.durability -= 25.0f;
    projectile.destroy |= true;
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
    // TODO -- Spawn particles
    if (projectile.type == PROJECTILE_MISSILE)
    {
        // Damage mech if in range
    }
}

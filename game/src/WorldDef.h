#pragma once
#include "Constants.h"
#include "Types.h"

struct World;
struct Mech;
struct Building;
struct Projectile;

struct HitInfo
{
	Vector3 mtv = Vector3Zeros;
};

using OnCollisionMechMech = void(*)(Mech& self, Mech& other, HitInfo hi);
using OnCollisionMechBuilding = void(*)(Mech& mech, Building& building, HitInfo hi);
using OnCollisionMechProjectile = void(*)(Mech& mech, Projectile& projectile, HitInfo hi);
using OnCollisionProjectileBuilding = void(*)(Projectile& projectile, Building& building, HitInfo hi);

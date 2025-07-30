#pragma once
#include <cstdint>

enum BuildingType : uint32_t
{
	BUILDING_TD,
	BUILDING_BMO,
	BUILDING_CONDO,
	BUILDING_TYPE_COUNT
};

enum ProjectileType : uint32_t
{
	PROJECTILE_RIFLE,
	PROJECTILE_SHOTGUN,
	PROJECTILE_GRENADE,
	PROJECTILE_MISSILE,
	PROJECTILE_MACHINEGUN,
	PROJECTILE_CHAINGUN,
	PROJECTILE_TYPE_COUNT
};

enum GearType : uint32_t
{
	GEAR_RIFLE,
	GEAR_SHOTGUN,
	GEAR_GRENADE_LAUNCHER,
	GEAR_MISSILE_LAUNCHER,
	GEAR_MACHINEGUN,
	GEAR_CHAINGUN,
	GEAR_TYPE_COUNT
};

enum Team : uint32_t
{
	TEAM_NONE,
	TEAM_RED,
	TEAM_BLUE
};

enum MissileState : uint32_t
{
	MISSILE_RISE,
	MISSILE_SEEK,
	MISSILE_DIVE
};

enum MapType : uint32_t
{
	MAP_LOBSTER_DINNER,
	MAP_AFTERNOON_DRIVE,
	MAP_MINTY_AFTERSHAVE,
	MAP_BUTTON_UP,
	MAP_FOX_ONLY_NO_ITEMS,

	MAP_TEST_1,	// Connor's test map
	MAP_TEST_2,	// Josh's test map
	MAP_TEST_3, // Jesse's test map (NSFW warning)
	// Nick and Regan feel free to add test maps 4 & 5 if you emerge from the shadows

	MAP_TYPE_COUNT
};

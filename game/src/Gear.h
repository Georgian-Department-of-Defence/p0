#pragma once
#include "Gear.h"
#include "Types.h"

struct GearRifle
{

};

struct GearShotgun
{

};

struct GearGrenadeLauncher
{
	int grenades;
	float launch_cooldown;
	float launch_cooldown_max;
};

struct GearMissileLauncher
{
	int missiles;
	float launch_cooldown;
	float launch_cooldown_max;
	float launch_roll;
};

struct GearMachineGun
{

};

struct GearChainGun
{
	float ramp_up;
};

struct Gear
{
	GearType type = GEAR_TYPE_COUNT;

	float cooldown;
	float cooldown_max;
	float heat;

	union
	{
		GearRifle rifle;
		GearShotgun shotgun;
		GearGrenadeLauncher grenade_launcher;
		GearMissileLauncher missile_launcher;
		GearMachineGun machine_gun;
		GearChainGun chain_gun;
	};
};

inline Gear CreateGearRifle()
{
	Gear g;
	g.type = GEAR_RIFLE;
	g.cooldown_max = 1.0f;
	g.cooldown = 0.0f;
	g.heat = 20; 
	return g;
}

inline Gear CreateGearShotgun()
{
	Gear g;
	g.type = GEAR_SHOTGUN;
	g.cooldown_max = 0.75f;
	g.cooldown = 0.0f;
	g.heat = 20;
	return g;
}

inline Gear CreateGearGrenadeLauncher()
{
	Gear g;
	g.type = GEAR_GRENADE_LAUNCHER;
	g.cooldown_max = 2.0f;
	g.cooldown = 0.0f;
	g.heat = 40;

	g.grenade_launcher.grenades = 0;
	g.grenade_launcher.launch_cooldown_max = 0.1f;
	g.grenade_launcher.launch_cooldown = 0.0f;
	return g;
}

inline Gear CreateGearMissileLauncher()
{
	Gear g;
	g.type = GEAR_MISSILE_LAUNCHER;
	g.cooldown = g.cooldown_max = 1.5f;
	g.heat = 30;

	g.missile_launcher.missiles = 0;
	g.missile_launcher.launch_cooldown_max = 0.05f;
	g.missile_launcher.launch_cooldown = 0.0f;
	return g;
}

inline Gear CreateGearMachineGun()
{
	Gear g;
	g.type = GEAR_MACHINEGUN;
	g.cooldown_max = 0.12f;
	g.cooldown = 0.0f;
	g.heat = 10;
	return g;
}

inline Gear CreateGearChainGun()
{
	Gear g;
	g.type = GEAR_CHAINGUN;
	g.cooldown_max = 0.05f;
	g.cooldown = 0.0f;
	g.heat = 5;
	g.chain_gun.ramp_up = 0;
	return g;
}

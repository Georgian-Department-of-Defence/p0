#include "Meshes.h"

Meshes g_meshes;

static Model f_mech_torso;		// 17.9 x 14.5 x 8.24
static Model f_mech_legs;		// 10.9 x 12.2 x 11.2

static Model f_bld_td;			// 10 x 10 x 50
static Model f_bld_bmo;			// 5 x 5 x 25
static Model f_bld_condo;		// 8.1 x 10.6 x 25.6

static Model f_prj_straight;	// 2 x 30 x 2, 4.5 from origin to tip
static Model f_prj_grenade;		// 1.94 x 4.45 x 2.24, origin in centre
static Model f_prj_missile;		// 2.27 x 9.00 x 2.62, origin in centre

void LoadMeshes()
{
	f_mech_torso = LoadModel("./assets/meshes/mech_torso.obj");
	f_mech_legs = LoadModel("./assets/meshes/mech_legs.obj");

	f_bld_td = LoadModel("./assets/meshes/bld_td.obj");
	f_bld_bmo = LoadModel("./assets/meshes/bld_bmo.obj");
	f_bld_condo = LoadModel("./assets/meshes/bld_condo.obj");

	f_prj_straight = LoadModel("./assets/meshes/prj_straight.obj");
	f_prj_grenade = LoadModel("./assets/meshes/prj_grenade.obj");
	f_prj_missile = LoadModel("./assets/meshes/prj_missile.obj");

	g_meshes.mech_torso = &f_mech_torso.meshes[0];
	g_meshes.mech_legs = &f_mech_legs.meshes[0];

	g_meshes.bld_td = &f_bld_td.meshes[0];
	g_meshes.bld_bmo = &f_bld_bmo.meshes[0];
	g_meshes.bld_condo = &f_bld_condo.meshes[0];

	g_meshes.prj_straight = &f_prj_straight.meshes[0];
	g_meshes.prj_grenade = &f_prj_grenade.meshes[0];
	g_meshes.prj_missile = &f_prj_missile.meshes[0];
}

void UnloadMeshes()
{
	g_meshes.prj_missile = nullptr;
	g_meshes.prj_grenade = nullptr;
	g_meshes.prj_straight = nullptr;

	g_meshes.bld_condo = nullptr;
	g_meshes.bld_bmo = nullptr;
	g_meshes.bld_td = nullptr;

	g_meshes.mech_legs = nullptr;
	g_meshes.mech_torso = nullptr;

	UnloadModel(f_prj_missile);
	UnloadModel(f_prj_grenade);
	UnloadModel(f_prj_straight);

	UnloadModel(f_bld_condo);
	UnloadModel(f_bld_bmo);
	UnloadModel(f_bld_td);

	UnloadModel(f_mech_legs);
	UnloadModel(f_mech_torso);
}

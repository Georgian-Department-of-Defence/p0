#pragma once
#include "raymathext.h"
#include "WorldDef.h"

struct Entity
{
	EntityType type = ENTITY_TYPE_COUNT;
	bool destroy_flag = false;

	Vector3 pos;
	Vector3 vel;
	Vector3 acc;

	virtual void OnLoad() {};
	virtual void OnUnload() {};
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnCollision(Entity& entity, HitInfo hit_info) = 0;
	
	// Simpler to make OnCollision a virtual method instead of a function-pointer
	// (Mechs, Buildings, and Projectiles can all collide with eachother)
	//OnCollision on_collision = nullptr;
};

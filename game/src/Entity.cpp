#include "Entity.h"

bool EntityCheckCollision3D(const Entity& a, const Entity& b, Vector3* mtv)
{
	assert(mtv != nullptr);
	assert(a.collider.type != COLLIDER_TYPE_COUNT && b.collider.type != COLLIDER_TYPE_COUNT);
	{
		bool can_types_collide = a.collision_type_mask & b.collision_type_mask;
		bool can_teams_collide = false;
		
		auto can_target = [](const Entity& src, const Entity& dst) 
		{
			bool result = false;
			result |= (src.collision_team_mask & TARGET_MASK_ALLY) && src.team == dst.team;
			result |= (src.collision_team_mask & TARGET_MASK_ENEMY) && src.team != dst.team;
			return result;
		};

		can_teams_collide = can_target(a, b) && can_target(b, a);
		if (!(can_types_collide && can_teams_collide)) return false;
	}

	bool collision = collision_table[a.collider.type][b.collider.type](a.collider, b.collider, mtv);
	return collision;
}

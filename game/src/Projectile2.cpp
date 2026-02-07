#include "Projectile2.h"

void Projectile2::OnCollisionPost(Entity* entity)
{
	switch (entity->type)
	{
		// Damage mech
		case ENTITY_MECH:
		{
			Mech2& mech = *(Mech2*)entity;
			break;
		}

		// Damage building
		case ENTITY_BUILDING:
		{
			Building2& building = *(Building2*)entity;
			break;
		}
	}
}

#pragma once

struct Collider2D
{
	ColliderType2D type = COLLIDER_TYPE2D_COUNT;

	union
	{
		struct
		{
			float radius;
		} circle;

		struct
		{
			float radius;
			float half_height;
			Vector2 direction; // Each entity must update capsule directions accordingly!
		} capsule;

		struct
		{
			Vector2 extents;
		} box;

		struct
		{
			Vector2 normal;
		} plane;
	};
};

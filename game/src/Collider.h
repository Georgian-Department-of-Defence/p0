#pragma once
struct Collider
{
	ColliderType type = COLLIDER_TYPE_COUNT;

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

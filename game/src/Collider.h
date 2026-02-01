#pragma once

struct HitInfo
{
	Vector2 mtv = Vector2Zeros;
};

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

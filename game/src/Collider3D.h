#pragma once
struct Collider3D
{
	ColliderType3D type = COLLIDER_TYPE_COUNT;

	union
	{
		struct
		{
			float radius;
		} sphere;

		struct
		{
			float radius;
			float half_height;
			Vector3 direction;
		} spherocylinder;

		struct
		{
			Vector3 extents;
		} box;

		struct
		{
			Vector3 normal;
		} plane;
	};

	Vector3 pos;
};

using Collider = Collider3D;

#pragma once

struct Collider3D
{
	ColliderType3D type = COLLIDER_TYPE_COUNT;

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
			Vector3 direction;
		} capsule;

		struct
		{
			Vector3 extents;
		} box;

		struct
		{
			Vector3 normal;
		} plane;
	};
};

using Collider = Collider3D;

//struct Collider2D
//{
//	ColliderType2D type = COLLIDER_TYPE2D_COUNT;
//
//	union
//	{
//		struct
//		{
//			float radius;
//		} circle;
//
//		struct
//		{
//			float radius;
//			float half_height;
//			Vector2 direction; // Each entity must update capsule directions accordingly!
//		} capsule;
//
//		struct
//		{
//			Vector2 extents;
//		} box;
//
//		struct
//		{
//			Vector2 normal;
//		} plane;
//	};
//};

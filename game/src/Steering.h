#pragma once
#include "raymathext.h"

inline Vector3 Seek(Vector3 seeker_position, Vector3 target_position, Vector3 seeker_velocity, float speed)
{
	Vector3 desired_velocity = Vector3Normalize(target_position - seeker_position) * speed;
	return desired_velocity - seeker_velocity;
}

inline Vector3 Flee(Vector3 seeker_position, Vector3 target_position, Vector3 seeker_velocity, float speed)
{
	Vector3 desired_velocity = Vector3Normalize(seeker_position - target_position) * speed;
	return desired_velocity - seeker_velocity;
}

inline float ProjectileHeight(float launch_speed, float launch_angle, float gravity)
{
	float v2 = launch_speed * launch_speed;
	float s2 = powf(sinf(launch_angle), 2.0f);
	float h = (v2 * s2) / (gravity * 2.0f);
	return h;
}

inline float ProjectileRange(float launch_speed, float launch_angle, float gravity)
{
	float v2 = launch_speed * launch_speed;
	float r = (v2 * sinf(launch_angle * 2.0f)) / gravity;
	return r;
}

inline float ProjectileTime(float launch_speed, float launch_angle, float gravity)
{
	float t = 2.0f * launch_speed * sinf(launch_angle) / gravity;
	return t;
}

inline float ProjectileAngle(float launch_speed, float height, float gravity)
{
	float v2 = launch_speed * launch_speed;
	float s = sqrtf(2.0f * gravity * height / v2);
	return asinf(s);
}

// Tests:
//const float gravity = 9.81f;
//const float speed = 30.0f;
//float angle = 35.0f * DEG2RAD;
//float range = ProjectileRange(speed, gravity, gravity);
//float time = ProjectileTime(speed, angle, gravity);
//float height = ProjectileHeight(speed, angle, gravity);
//angle = ProjectileAngle(speed, height, gravity) * RAD2DEG;

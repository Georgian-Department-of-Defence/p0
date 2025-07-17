#include "PhysicsScene.h"
#include "Collision.h"
#include "Collision3D.h"
#include "Camera.h"

static Vector3 p0 = Vector3UnitX * -10.0f;
static Vector3 p1 = Vector3UnitX *  10.0f;
static Vector3 p2 = Vector3UnitX * 10.0f;
static float radius = 5.0f;
static bool collision = false;
static float width = 5.0f;
static float height = 5.0f;
static float length = 5.0f;
static Camera camera;

void PhysicsScene::OnLoad(Game& game)
{
	// fovy is screen top / bot when orthographic
	// (yMin = -50, yMax = 50, xMin = -50 * aspect, xMax = 50 * aspect)
	camera.position = Vector3UnitZ * 50.0f;
	camera.target = Vector3Zeros;
	camera.up = Vector3UnitY;
	camera.fovy = 100.0f;
	camera.projection = CAMERA_ORTHOGRAPHIC;
}

void PhysicsScene::OnUnload(Game& game)
{
}

void PhysicsScene::OnStart(Game& game)
{
}

void PhysicsScene::OnStop(Game& game)
{
}

void PhysicsScene::OnUpdate(Game& game)
{
	p0 = Vector3UnitY * sinf(GetTime()) * 20.0f;
	p1 = Vector3UnitY * cosf(GetTime()) * 20.0f;
	p2 = { p1.x, p1.y + 5.0f, p1.z };
	Vector3 v0{ p0.x, p0.y , p0.z};
	Vector3 v1{ p1.x, p1.y , p0.z};
	//Vector3 extents{ length / 2, width / 2, height / 2};
	//collision = SphereBox(v0, radius, v1, extents);

	// Note: Fix the capsule in-place, and control sphere with mouse to make test easier
	// Investigate ScreenToWorld conversion (raylib.h line 1061), otherwise just control both objects with keyboard if it doesn't work
	// See World.cpp line 160 for sphere-capsule example of projectile-building collisions
	collision = SphereCapsule(v0, radius, v1, Vector3Zero(), radius, p2.y - p1.y);
}

void PhysicsScene::OnDraw(Game& game)
{
	Color color = collision ? RED : GREEN;
	BeginMode3D(camera);
	DrawSphere(p0, radius, color);
	//DrawCube(p1, width, height, length, color);
	DrawCapsule(p1, p2, radius, 6, 6, color);
	EndMode3D();
}

void PhysicsScene::OnDrawDebug(Game& game)
{
}

void PhysicsScene::OnDrawGui(Game& game)
{

}

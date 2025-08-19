#include "TeamSelectScene.h"
#include "Camera.h"

void TeamSelectScene::OnLoad(Game& game)
{

}

void TeamSelectScene::OnUnload(Game& game)
{
}

void TeamSelectScene::OnUpdate(Game& game)
{
}

void TeamSelectScene::OnDraw(Game& game)
{
	// TODO - Do we want a 2D camera or a 3D camera?
	// 2d rendering functions like DrawRectangle only work in 2D.
	// 3d rendering functions like DrawCube only work in 3D.
	// Would it make more sense to render in screen-space (2d) to a fixed-size fbo (ie 4k) then resolve to the monitor, or render everything in 3d world-space?
	// Answer: it's probably best to non-battle UI in 2d for convenient text-rendering and intuitive primitives, and battle-ui in 3d for billboarding.
	// Rule of thumb -- if mapping between 2d & 3d is needed, we must render in 3d (ie mech UI), otherwise prefer entirely 2d (team & loadout UI).
	//

	ClearBackground(GREEN);

	// Proof of 3d UI (don't use)
	//BeginMode3D(g_camera_system.camera_ui_3d);
	//DrawCube(Vector3Zeros, 10.0f, 10.0f, 10.0f, RED);
	//EndMode3D();

	BeginMode2D(g_camera_system.camera_ui_2d);

	Rectangle rec;
	rec.x = 0.0f;
	rec.y = 0.0f;
	rec.width = 100.0f;
	rec.height = 400.0f;
	DrawRectanglePro(rec, { rec.width * 0.5f, rec.height * 0.5f }, 0.0f, ORANGE);	// Rendered relative to origin [half-width, half-height] = relative to centre
	DrawRectangle(0, 0, 100, 400, RED);												// Rendered relative to top-left (see rtextures.c line 4516 for more information)
	DrawCircle(0, 0, 10.0f, PURPLE);												// Circle about the camera's origin for reference
	DrawText("Test", 20, 20, 20, BLUE);
	//MeasureText() <-- use to centre text
	EndMode2D();
}

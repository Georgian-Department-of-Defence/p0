#include "MapScene.h"
#include "Camera.h"
#include "World.h"

static World f_world;

void MapScene::OnLoad(Game& game)
{
	LoadWorld(f_world);
}

void MapScene::OnUnload(Game& game)
{
	UnloadWorld(f_world);
}

void MapScene::OnStart(Game& game)
{
}

void MapScene::OnStop(Game& game)
{
}

void MapScene::OnUpdate(Game& game)
{
	UpdateCamera();
	UpdateWorld(f_world);
}

void MapScene::OnDraw(Game& game)
{
	DrawWorld(f_world);
}

void MapScene::OnDrawDebug(Game& game)
{
	DrawWorldDebug(f_world);
}

void MapScene::OnDrawGui(Game& game)
{
	// TODO - Add global 2d camera for UI
	DrawFPS(10, 10);
}

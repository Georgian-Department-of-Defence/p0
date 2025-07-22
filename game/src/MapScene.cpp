#include "MapScene.h"
#include "Camera.h"

void MapScene::OnLoad(Game& game)
{
	LoadWorld(game.world);
}

void MapScene::OnUnload(Game& game)
{
	UnloadWorld(game.world);
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
	UpdateWorld(game.world);
}

void MapScene::OnDraw(Game& game)
{
	DrawWorld(game.world, game.renderer);
}

void MapScene::OnDrawDebug(Game& game)
{
	DrawWorldDebug(game.world, game.renderer);
}

void MapScene::OnDrawGui(Game& game)
{
}

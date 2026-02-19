#include "MapScene.h"

void MapScene::OnLoad(Game& game)
{
	WorldLoad(game.world);
}

void MapScene::OnUnload(Game& game)
{
	WorldUnload(game.world);
}

void MapScene::OnUpdate(Game& game)
{
	WorldUpdate(game.world);
}

void MapScene::OnDraw(Game& game)
{
	WorldDraw(game.world);
}

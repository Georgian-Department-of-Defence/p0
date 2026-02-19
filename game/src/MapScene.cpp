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
	if (IsKeyPressed(KEY_G))
		GameChangeScene(game, SCENE_DEV_GEAR);

	if (IsKeyPressed(KEY_K) && !game.world.mechs.empty())
		game.world.mechs.back().destroy_flag |= true;

	WorldUpdate(game.world);
}

void MapScene::OnDraw(Game& game)
{
	WorldDraw(game.world);
}

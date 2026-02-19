#include "GearScene.h"

void GearScene::OnUpdate(Game& game)
{
	if (IsKeyPressed(KEY_G))
		GameChangeScene(game, SCENE_DEV_MAP);
}

void GearScene::OnDraw(Game& game)
{
}

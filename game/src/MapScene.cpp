#include "MapScene.h"

void MapScene::OnLoad(World2& world)
{
	WorldLoad(world);
}

void MapScene::OnUnload(World2& world)
{
	WorldUnload(world);
}

void MapScene::OnUpdate(World2& world)
{
	WorldUpdate(world);
}

void MapScene::OnDraw(World2& world)
{
	WorldDraw(world);
}

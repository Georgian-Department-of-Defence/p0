#pragma once
#include "Scene.h"
class TeamSelectScene :
    public Scene
{
	void OnLoad(Game& game) final;
	void OnUnload(Game& game) final;

	//void OnStart(Game& game) final;
	//void OnStop(Game& game) final;

	void OnUpdate(Game& game)final;
	void OnDraw(Game& game) final;
};

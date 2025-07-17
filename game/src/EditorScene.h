#pragma once
#include "Scene.h"

class EditorScene : public Scene
{
public:
	void OnLoad(Game& game) final;
	void OnUnload(Game& game) final;

	void OnUpdate(Game& game)final;
	void OnDraw(Game& game) final;

	void OnDrawDebug(Game& game) final;
	void OnDrawGui(Game& game) final;
};

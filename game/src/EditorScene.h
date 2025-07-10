#pragma once
#include "Scene.h"

class EditorScene : public Scene
{
public:
	void OnLoad() final;
	void OnUnload() final;

	void OnStart() final;

	void OnUpdate()final;
	void OnDraw() final;

	void OnDrawDebug() final;
	void OnDrawGui() final;
};

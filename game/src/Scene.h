#pragma once
#include "Game.h"

enum SceneType : size_t
{
	SCENE_SPLASH_SCREEN,
	SCENE_TEAM_SELECT,
	SCENE_LOADOUT_SELECT,
	SCENE_BATTLE,
	
	SCENE_DEV_ANIMATIONS,
	SCENE_DEV_GEAR,
	SCENE_DEV_MAP,
	SCENE_DEV_PARTICLES,
	SCENE_DEV_PHYSICS,
	SCENE_DEV_WIDGETS,
	SCENE_DEV_EDITOR,

	SCENE_TYPE_COUNT
};

class Scene
{
public:
	virtual void OnLoad() {}
	virtual void OnUnload() {}

	virtual void OnStart() {}
	virtual void OnStop() {}

	virtual void OnUpdate() {}
	virtual void OnDraw() {}

	virtual void OnDrawDebug() {}
	virtual void OnDrawGui() {}

	static void Change(SceneType scene);
	static SceneType Current();

	static void Load(SceneType scene);
	static void Unload();

	static void Update();
	static void Draw();
	static void DrawDebug();
	static void DrawGui();

private:
	static Scene* s_scenes[SCENE_TYPE_COUNT];
	static SceneType s_current;
};

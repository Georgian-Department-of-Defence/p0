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
	virtual void OnLoad(Game& game) {}
	virtual void OnUnload(Game& game) {}

	virtual void OnStart(Game& game) {}
	virtual void OnStop(Game& game) {}

	virtual void OnUpdate(Game& game) {}
	virtual void OnDraw(Game& game) {}

	virtual void OnDrawDebug(Game& game) {}
	virtual void OnDrawGui(Game& game) {}

	static void Change(Game& game, SceneType scene);
	static SceneType Current();

	static void Load(Game& game, SceneType scene);
	static void Unload(Game& game);

	static void Update(Game& game);
	static void Draw(Game& game);
	static void DrawDebug(Game& game);
	static void DrawGui(Game& game);

private:
	static Scene* s_scenes[SCENE_TYPE_COUNT];
	static SceneType s_current;
};

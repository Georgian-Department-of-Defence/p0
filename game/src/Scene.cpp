#include "Scene.h"

// Game scenes
#include "SplashScreenScene.h"
#include "TeamSelectScene.h"
#include "LoadoutSelectScene.h"
#include "BattleScene.h"

// Development scenes
#include "AnimationsScene.h"
#include "GearScene.h"
#include "MapScene.h"
#include "ParticlesScene.h"
#include "PhysicsScene.h"
#include "WidgetsScene.h"
#include "EditorScene.h"

Scene* Scene::s_scenes[SCENE_TYPE_COUNT]{};
SceneType Scene::s_current = SCENE_TYPE_COUNT;

void Scene::Load(Game& game, SceneType scene)
{
	s_scenes[SCENE_SPLASH_SCREEN] = new SplashScreenScene;
	s_scenes[SCENE_TEAM_SELECT] = new TeamSelectScene;
	s_scenes[SCENE_LOADOUT_SELECT] = new LoadoutSelectScene;
	s_scenes[SCENE_BATTLE] = new BattleScene;

	s_scenes[SCENE_DEV_ANIMATIONS] = new AnimationsScene;
	s_scenes[SCENE_DEV_GEAR] = new GearScene;
	s_scenes[SCENE_DEV_MAP] = new MapScene;
	s_scenes[SCENE_DEV_PARTICLES] = new ParticlesScene;
	s_scenes[SCENE_DEV_PHYSICS] = new PhysicsScene;
	s_scenes[SCENE_DEV_WIDGETS] = new WidgetsScene;
	s_scenes[SCENE_DEV_EDITOR] = new EditorScene;

	for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
		s_scenes[i]->OnLoad(game);

	s_current = scene;
	s_scenes[s_current]->OnStart(game);
}

void Scene::Unload(Game& game)
{
	s_scenes[s_current]->OnStop(game);
	for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
	{
		s_scenes[i]->OnUnload(game);
		delete s_scenes[i];
		s_scenes[i] = nullptr;
	}
	s_current = SCENE_TYPE_COUNT;
}

void Scene::Change(Game& game, SceneType scene)
{
	// TODO - Add transitions (ie draw fading TV static for 1 second)
	s_scenes[s_current]->OnStop(game);
	s_current = scene;
	s_scenes[s_current]->OnStart(game);
}

SceneType Scene::Current()
{
	return s_current;
}

void Scene::Update(Game& game)
{
	s_scenes[s_current]->OnUpdate(game);
}

void Scene::Draw(Game& game)
{
	s_scenes[s_current]->OnDraw(game);
}

void Scene::DrawDebug(Game& game)
{
	s_scenes[s_current]->OnDrawDebug(game);
}

void Scene::DrawGui(Game& game)
{
	s_scenes[s_current]->OnDrawGui(game);
}

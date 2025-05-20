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

Scene* Scene::s_scenes[SCENE_TYPE_COUNT]{};
SceneType Scene::s_current = SCENE_TYPE_COUNT;

void Scene::Load(SceneType scene)
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

	for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
		s_scenes[i]->OnLoad();

	s_current = scene;
	s_scenes[s_current]->OnStart();
}

void Scene::Unload()
{
	s_scenes[s_current]->OnStop();
	for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
	{
		s_scenes[i]->OnUnload();
		delete s_scenes[i];
		s_scenes[i] = nullptr;
	}
	s_current = SCENE_TYPE_COUNT;
}

void Scene::Change(SceneType scene)
{
	// TODO - Add transitions (ie draw fading TV static for 1 second)
	s_scenes[s_current]->OnStop();
	s_current = scene;
	s_scenes[s_current]->OnStart();
}

SceneType Scene::Current()
{
	return s_current;
}

void Scene::Update()
{
	s_scenes[s_current]->OnUpdate();
}

void Scene::Draw()
{
	s_scenes[s_current]->OnDraw();
}

void Scene::DrawDebug()
{
	s_scenes[s_current]->OnDrawDebug();
}

void Scene::DrawGui()
{
	s_scenes[s_current]->OnDrawGui();
}

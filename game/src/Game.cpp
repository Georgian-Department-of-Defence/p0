#include "Game.h"
#include "MapScene.cpp"
#include "GearScene.cpp"

void GameInit(Game& game, SceneType scene)
{
	//game.scenes[SCENE_SPLASH_SCREEN] = new SplashScreenScene;
	//game.scenes[SCENE_TEAM_SELECT] = new TeamSelectScene;
	//game.scenes[SCENE_LOADOUT_SELECT] = new LoadoutSelectScene;
	//game.scenes[SCENE_BATTLE] = new BattleScene;
	//
	//game.scenes[SCENE_DEV_ANIMATIONS] = new AnimationsScene;
	game.scenes[SCENE_DEV_GEAR] = new GearScene;
	game.scenes[SCENE_DEV_MAP] = new MapScene;
	//game.scenes[SCENE_DEV_PARTICLES] = new ParticlesScene;
	//game.scenes[SCENE_DEV_PHYSICS] = new PhysicsScene;
	//game.scenes[SCENE_DEV_WIDGETS] = new WidgetsScene;
	//game.scenes[SCENE_DEV_EDITOR] = new EditorScene;

	//for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
	//	game.scenes[i]->OnInit(game.world);

	game.current = scene;
	game.scenes[game.current]->OnLoad(game);
}

void GameQuit(Game& game)
{
	game.scenes[game.current]->OnUnload(game);

	//for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
	//	game.scenes[i]->OnQuit(game.world);
	//
	//for (size_t i = 0; i < SCENE_TYPE_COUNT; i++)
	//	delete game.scenes[i];
}

void GameUpdate(Game& game)
{
	game.scenes[game.current]->OnUpdate(game);
}

void GameDraw(Game& game)
{
	game.scenes[game.current]->OnDraw(game);
}

void GameChangeScene(Game& game, SceneType scene)
{
	game.scenes[game.current]->OnUnload(game);
	game.current = scene;
	game.scenes[game.current]->OnLoad(game);
}

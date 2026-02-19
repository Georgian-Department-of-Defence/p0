#pragma once

struct Game
{
    World2 world;
    Scene2* scenes[SCENE_TYPE_COUNT];
    SceneType current = SCENE_TYPE_COUNT;
};

void GameInit(Game& game, SceneType scene);
void GameQuit(Game& game);

void GameUpdate(Game& game);
void GameDraw(Game& game);

void GameChangeScene(Game& game, SceneType scene);

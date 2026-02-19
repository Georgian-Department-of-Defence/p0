#pragma once
struct Game;

struct Scene2
{
    virtual void OnInit(Game& game) {}   // Game enter
    virtual void OnQuit(Game& game) {}   // Game exit

    virtual void OnLoad(Game& game) {}   // Scene enter
    virtual void OnUnload(Game& game) {} // Scene exit

    virtual void OnUpdate(Game& game) = 0;
    virtual void OnDraw(Game& game) = 0;
};

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

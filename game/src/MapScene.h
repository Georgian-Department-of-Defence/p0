#pragma once

class MapScene : public Scene2
{
public:
    void OnLoad(Game& game) final;
    void OnUnload(Game& game) final;

    void OnUpdate(Game& game) final;
    void OnDraw(Game& game)final;
};

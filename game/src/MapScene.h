#pragma once

class MapScene : public Scene2
{
public:
    void OnLoad(World2& world) final;
    void OnUnload(World2& world) final;

    void OnUpdate(World2& world) final;
    void OnDraw(World2& world)final;
};

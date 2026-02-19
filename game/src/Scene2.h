#pragma once
struct Scene2
{
    virtual void OnInit(World2& world) {}   // Game enter
    virtual void OnQuit(World2& world) {}   // Game exit

    virtual void OnLoad(World2& world) {}   // Scene enter
    virtual void OnUnload(World2& world) {} // Scene exit

    virtual void OnUpdate(World2& world) = 0;
    virtual void OnDraw(World2& world) = 0;
};
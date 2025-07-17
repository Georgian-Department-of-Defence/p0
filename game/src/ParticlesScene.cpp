#include "ParticlesScene.h"
#include "ParticleEmitter.h"
#include "Camera.h"

static Camera3D camera = { 0 };
static ParticleEmitter emitter;

void ParticlesScene::OnLoad(Game& game)
{
    camera.position = { 0.0f, 0.0f, 10.0f };
    camera.target = { 0.0f, -4.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 110.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    emitter.position = { 0.0f, -1.0f, -2.0f };
    emitter.direction = Vector3Normalize({ 0.5f, 1.0f, 0.0f } );

    emitter.spawn_rate = 20.0f;
    emitter.speed = 1.5f;
    emitter.size = 0.3f;
    emitter.lifetime = 3.0f;
    emitter.color_src = WHITE;
    emitter.color_dst = BLACK;

    emitter.shape_type = PARTICLE_SHAPE_SPHERE;
    switch (emitter.shape_type)
    {
    case PARTICLE_SHAPE_SPHERE:
        emitter.shape.sphere.radius = 4.0f;
        break;
    case PARTICLE_SHAPE_BOX:
        emitter.shape.box.size = Vector3Ones * 4.0f;
        break;
    case PARTICLE_SHAPE_CONE:
        emitter.shape.cone.angle = 45.0f * DEG2RAD;
        break;
    }
}

void ParticlesScene::OnUnload(Game& game)
{
}

void ParticlesScene::OnStart(Game& game)
{
}

void ParticlesScene::OnStop(Game& game)
{
}

void ParticlesScene::OnUpdate(Game& game)
{
    UpdateParticleEmitter(emitter);
}

void ParticlesScene::OnDraw(Game& game)
{
    BeginMode3D(camera);
    DrawParticleEmitter(emitter, camera);
    EndMode3D();
}

void ParticlesScene::OnDrawDebug(Game& game)
{
}

void ParticlesScene::OnDrawGui(Game& game)
{
}

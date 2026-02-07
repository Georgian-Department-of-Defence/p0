#include "ParticleEmitter.h"

inline Vector3 RandomParticlePosition(ParticleShape shape, ParticleShapeType type)
{
    Vector3 position = Vector3Zeros;
    switch (type)
    {
    case PARTICLE_SHAPE_SPHERE:
        position = RandomSpherePosition(shape.sphere.radius);
        break;

    case PARTICLE_SHAPE_BOX:
        position = RandomBoxPosition(shape.box.size);
        break;

    case PARTICLE_SHAPE_CONE:
        // TODO - Implement RandomConePosition function in raymathext.h
        // See bottom of file for more information
        break;

    case PARTICLE_SHAPE_TYPE_COUNT:
        assert(false);
        break;
    }
    return position;
}

Particle CreateParticle(const ParticleEmitter& emitter)
{
    Particle p;

    p.position = emitter.position + RandomParticlePosition(emitter.shape, emitter.shape_type);
    p.velocity = emitter.direction * emitter.speed;

    p.color = emitter.color_src;
    p.size = emitter.size;
    p.life = emitter.lifetime;
    p.life_max = emitter.lifetime;

    return p;
}

void DestroyParticleEmitter(ParticleEmitter* emitter)
{
    emitter->shape_type = PARTICLE_SHAPE_TYPE_COUNT;
    emitter->particles.clear();
}

void UpdateParticleEmitter(ParticleEmitter& emitter)
{
    emitter.particles.erase
    (
        std::remove_if(emitter.particles.begin(), emitter.particles.end(), [](const Particle& p) { return p.life <= 0.0f; }),
        emitter.particles.end()
    );
    
    float dt = GetFrameTime();
    for (Particle& p : emitter.particles)
    {
        p.position += p.velocity * dt;
        p.life -= dt;

        float t = 1.0f - (p.life / p.life_max);
        p.color = ColorLerp(emitter.color_src, emitter.color_dst, t);
    }

    emitter.spawn_accumulator += dt;
    float spawn_interval = 1.0f / emitter.spawn_rate;
    while (emitter.spawn_accumulator >= spawn_interval)
    {
        Particle p = CreateParticle(emitter);
        emitter.particles.push_back(p);
        emitter.spawn_accumulator -= spawn_interval;
    }
}

void DrawParticleEmitter(const ParticleEmitter& emitter, const Camera& camera)
{
    for (const Particle& p : emitter.particles)
    {
        DrawBillboard(camera, assets.texture.white, p.position, p.size, p.color);
    }
}

// I'm confused by this. Shouldn't the particle be generated randomly within the cone and then moved in the emitter direction?
// SPHERE and BOX seem to do so. Logically, cone should follow!
//Vector3 ConeDirection(float cone_angle, Vector3 emitter_direction) {
//    float angle_rad = (cone_angle * DEG2RAD);
//    
//    //random angle within cone
//    float u = Random(0.0f, 1.0f);
//    float v = Random(0.0f, 1.0f);
//    float theta = u * 2.0f * PI;
//    float phi = acosf(1.0f - v * (1.0f - cosf(angle_rad)));
//
//    //to cartesian
//    float x = sinf(phi) * cosf(theta);
//    float y = sinf(phi) * sinf(theta);
//    float z = cosf(phi);
//
//    //cone dir in x
//    Vector3 dir = { x, y, z };
//
//    // rotate too match emitter direction
//    Vector3 up = Vector3UnitZ;
//    if (Vector3LengthSqr(Vector3Subtract(emitter_direction, up)) < 0.001f)
//        return dir; //already aligned
//    else
//    {
//        Vector3 axis = Vector3CrossProduct(up, emitter_direction);
//        float angle = acosf(Vector3DotProduct(Vector3Normalize(up), Vector3Normalize(emitter_direction)));
//        dir = Vector3RotateByAxisAngle(dir, axis, angle);
//        return dir;
//    }
//}

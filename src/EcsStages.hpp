#pragma once

#include <flecs.h>

struct Setup {};
struct Update {};
struct Dispose {};
struct Draw2D {};
struct Draw3D {};

struct DeltaTime {
    float value;

    DeltaTime(float value = 0.0f) : value(value) {}
};

struct EcsStages {
    flecs::entity setup,
                  update,
                  dispose,
                  draw2d,
                  draw3d;

    EcsStages(flecs::world& world) {
        world.component<DeltaTime>().add(flecs::Singleton);

        setup = world.pipeline()
            .with(flecs::System)
            .with<Setup>()
            .build();

        update = world.pipeline()
            .with(flecs::System)
            .with<Update>()
            .build();

        dispose = world.pipeline()
            .with(flecs::System)
            .with<Dispose>()
            .build();

        draw2d = world.pipeline()
            .with(flecs::System)
            .with<Draw2D>()
            .build();

        draw3d = world.pipeline()
            .with(flecs::System)
            .with<Draw3D>()
            .build();
    }
};
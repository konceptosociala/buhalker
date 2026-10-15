#pragma once

#include <flecs.h>
#include <raylib.h>

struct GlobalCamera3D {
    Camera3D value = Camera3D();
};

struct GlobalCamera2D {
    Camera2D value = Camera2D();
};

struct GlobalCameraModule {
    GlobalCameraModule(flecs::world& world) {
        world.module<GlobalCameraModule>();

        world.component<GlobalCamera2D>().add(flecs::Singleton);
        world.component<GlobalCamera3D>().add(flecs::Singleton);

        world.set(GlobalCamera2D());
        world.set(GlobalCamera3D());
    }
};
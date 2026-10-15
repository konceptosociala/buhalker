#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#include "EcsStages.hpp"
#include "Player.hpp"
#include "ChunkMeshGenerator.hpp"
#include "GlobalCamera.hpp"

int main() {
    flecs::world world;

    EcsStages stages(world);
    
    InitWindow(800, 600, "Hello gays");
    SetTargetFPS(60);
    DisableCursor();

    world.import<GlobalCameraModule>();
    world.import<PlayerModule>();
    world.import<ChunkMeshGeneratorModule>();
    world.import<ChunkModule>();

    world.run_pipeline(stages.setup);

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        world.set(DeltaTime(dt));
        world.run_pipeline(stages.update, dt);

        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(world.get<GlobalCamera3D>().value);
                world.run_pipeline(stages.draw3d, dt);
            EndMode3D();

            BeginMode2D(world.get<GlobalCamera2D>().value);
                world.run_pipeline(stages.draw2d, dt);
            EndMode2D();
        EndDrawing();
    }

    world.run_pipeline(stages.dispose);

    CloseWindow();

    return 0;
}

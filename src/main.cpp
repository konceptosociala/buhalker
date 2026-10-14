#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#include "EcsStages.hpp"
#include "Physics.hpp"
#include "ChunkMeshGenerator.hpp"
#include "GlobalCamera.hpp"

int main() {
    flecs::world world;

    EcsStages stages(world);

    world.system<>()
        .kind<Setup>()
        .run([](flecs::iter& it) {
            it.world().set(GlobalCamera2D());
            it.world().set(GlobalCamera3D());
            it.world().set(ChunkMeshGenerator());
            it.world().set(TileMap("assets/tilemap.png", 16, 16));

            it.world().entity()
                .set(Camera3D {
                    .position = Vector3(),
                    .target   = Vector3(),
                    .up       = Vector3(0, 1, 0),
                    .fovy     = 75,
                    .projection = CAMERA_PERSPECTIVE,
                })
                .set(Player());
        });

    world.system<const Player, Camera3D>()
        .each([](const Player& player, Camera3D& camera) {
            camera.position = Vector3Add(
                player.position,
                Vector3(0.0f, player.eyeHeight, 0.0f)
            );

            const float cp = std::cos(player.pitch);

            const Vector3 lookDirection = Vector3(
                std::sin(player.yaw) * cp,
                std::sin(player.pitch),
                std::cos(player.yaw) * cp
            );

            camera.target = Vector3Add(camera.position, lookDirection);
        });

    InitWindow(800, 600, "Hello gays");
    SetTargetFPS(60);
    DisableCursor();

    // Chunk chunk;
    // for (int x = 0; x < 16; x++) {
    //     for (int z = 0; z < 16; z++) {
    //         chunk.set(DIRT.getDefaultData(), x, 2, z);
    //         chunk.set(STONE.getDefaultData(), x, 1, z);
    //     }
    // }

    // Mesh mesh = meshGen.generateChunkMesh(
    //     BLOCK_REGISTRY, 
    //     tileMap, 
    //     chunk
    // );
    // UploadMesh(&mesh, false);

    // Material material = LoadMaterialDefault();
    // SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, tileMap.getTexture());

    world.run_pipeline(stages.setup);

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        world.run_pipeline(stages.update, dt);
        
        // const Vector2 mouse = GetMouseDelta();

        // player.yaw -= mouse.x * player.sensitivity;
        // player.pitch -= mouse.y * player.sensitivity;

        // constexpr float maxPitch = 1.55334f;
        // player.pitch = Clamp(player.pitch, -maxPitch, maxPitch);

        // const Vector3 forward = Vector3(
        //     std::sin(player.yaw),
        //     0.0f,
        //     std::cos(player.yaw)
        // );

        // const Vector3 right = Vector3(
        //     std::cos(player.yaw),
        //     0.0f,
        //     -std::sin(player.yaw)
        // );

        // Vector3 movement = Vector3(0.0f, 0.0f, 0.0f);

        // if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, forward);
        // if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, forward);
        // if (IsKeyDown(KEY_A)) movement = Vector3Add(movement, right);
        // if (IsKeyDown(KEY_D)) movement = Vector3Subtract(movement, right);

        // if (Vector3LengthSqr(movement) > 0.0f) {
        //     movement = Vector3Normalize(movement);
        // }

        // player.position = Vector3Add(
        //     player.position,
        //     Vector3Scale(movement, player.speed * dt)
        // );

        // if (IsKeyPressed(KEY_TAB)) {
        //     IsCursorHidden() 
        //         ? EnableCursor() 
        //         : DisableCursor();
        // }

        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(world.get<GlobalCamera3D>().value);
                world.run_pipeline(stages.draw3d, dt);
            //     DrawMesh(mesh, material, MatrixIdentity());
            EndMode3D();

            BeginMode2D(world.get<GlobalCamera2D>().value);
                world.run_pipeline(stages.draw2d, dt);
            EndMode2D();
        EndDrawing();
    }

    world.run_pipeline(stages.dispose);

    // UnloadMesh(mesh);
    // material.maps[MATERIAL_MAP_ALBEDO].texture = {};
    // UnloadMaterial(material);

    CloseWindow();

    return 0;
}

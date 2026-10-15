#pragma once

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "EcsStages.hpp"
#include "GlobalCamera.hpp"
#include "Chunk.hpp"

constexpr float GRAVITY = 9.81f;

struct PlayerState {
    Vector3 position = Vector3(5, 5, 5);
    Vector3 velocity = Vector3();
    float yaw = 0.0f;
    float pitch = 0.0f;
    bool onGround = false;

    ChunkPos chunkPos() {
        return ChunkPos(
            ((int) position.x) >> 4,
            ((int) position.y) >> 4,
            ((int) position.z) >> 4
        );
    }
};

struct PlayerConfig {
    float width         = 0.6f;
    float height        = 1.8f;
    float eyeHeight     = 1.62f;
    float speed         = 5.0f;
    float sensitivity   = 0.0025f;
};

struct PlayerInput {
    bool forward, back, left, right, jump;
    float mouseX, mouseY;

    PlayerInput() = default;
};

struct CurrentPlayer {};

struct PlayerModule {
    PlayerModule(flecs::world& world) {
        world.module<PlayerModule>();

        world.component<PlayerInput>().add(flecs::Singleton);
        world.component<PlayerConfig>().add(flecs::Singleton);

        world.set(PlayerInput());
        world.set(PlayerConfig());

        world.entity()
            .set(Camera3D {
                .position = Vector3(),
                .target   = Vector3(),
                .up       = Vector3(0, 1, 0),
                .fovy     = 75,
                .projection = CAMERA_PERSPECTIVE,
            })
            .set(PlayerState())
            .add<CurrentPlayer>();
        
        world.system<PlayerInput>("Player input system")
            .kind<Update>()
            .each([](PlayerInput& input) {
                input = PlayerInput();

                if (IsKeyDown(KEY_W)) input.forward = true;
                if (IsKeyDown(KEY_S)) input.back = true;
                if (IsKeyDown(KEY_A)) input.left = true;
                if (IsKeyDown(KEY_D)) input.right = true;

                const Vector2 mouse = GetMouseDelta();
                input.mouseX = mouse.x;
                input.mouseY = mouse.y;
            });

        world.system<
            const PlayerConfig, 
            const PlayerInput, 
            const DeltaTime, 
            PlayerState, 
            Camera3D
        >("Player movement system")
            .kind<Update>()
            .each([](
                const PlayerConfig config,
                const PlayerInput input,
                const DeltaTime dt,
                PlayerState& player,
                Camera3D& camera
            ) {
                player.yaw -= input.mouseX * config.sensitivity;
                player.pitch -= input.mouseY * config.sensitivity;

                constexpr float maxPitch = 1.55334f;
                player.pitch = Clamp(player.pitch, -maxPitch, maxPitch);

                const Vector3 forward = Vector3(
                    std::sin(player.yaw),
                    0.0f,
                    std::cos(player.yaw)
                );

                const Vector3 right = Vector3(
                    -std::cos(player.yaw),
                    0.0f,
                    std::sin(player.yaw)
                );

                Vector3 movement = Vector3(0.0f, 0.0f, 0.0f);

                if (input.forward) 
                    movement = Vector3Add(movement, forward);

                if (input.back) 
                    movement = Vector3Subtract(movement, forward);

                if (input.right) 
                    movement = Vector3Add(movement, right);

                if (input.left) 
                    movement = Vector3Subtract(movement, right);

                if (Vector3LengthSqr(movement) > 0.0f) {
                    movement = Vector3Normalize(movement);
                }

                player.position = Vector3Add(
                    player.position,
                    Vector3Scale(movement, config.speed * dt.value)
                );

                camera.position = Vector3Add(
                    player.position,
                    Vector3(0.0f, config.eyeHeight, 0.0f)
                );

                const float cp = std::cos(player.pitch);

                const Vector3 lookDirection = Vector3(
                    std::sin(player.yaw) * cp,
                    std::sin(player.pitch),
                    std::cos(player.yaw) * cp
                );

                camera.target = Vector3Add(camera.position, lookDirection);
            });

        world.system<const Camera3D, const CurrentPlayer, GlobalCamera3D>("Synchronize player camera")
            .kind<Update>()
            .each([](const Camera3D& cam, const CurrentPlayer, GlobalCamera3D& globalCam) {
                globalCam.value = cam;
            });

        world.system<PlayerState, const ChunkStorage, const DeltaTime>("Physics test")
            .kind<Update>()
            .each([](PlayerState& state, const ChunkStorage& chunks, const DeltaTime& dt) {
                if (!state.onGround) {
                    state.velocity.y -= GRAVITY * dt.value;
                }

                state.position.y += state.velocity.y * dt.value;

                ChunkPos playerChunkPos = state.chunkPos();
                // const Chunk& playerChunk = chunks.at(playerChunkPos).get<Chunk>();
                
            });
    }
};
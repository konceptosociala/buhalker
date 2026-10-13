#pragma once

#include <raylib.h>

#include "VoxelWorld.hpp"

int main() {
    InitWindow(800, 600, "Hello gays");
    SetTargetFPS(60);

    VoxelWorld world;

    world.try_emplace(ChunkPos(0, 0, 0));

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(Color(255, 255, 0, 255));
            DrawText("Hello Gays", 190, 200, 20, Color(255, 255, 255, 255));
        EndDrawing();
    }

    return 0;
}

#include <raylib.h>

#include "BlockRegistry.hpp"
#include "ChunkMeshGenerator.hpp"
#include "raymath.h"

int main() {
    InitWindow(800, 600, "Hello gays");
    SetTargetFPS(60);
    DisableCursor();

    {
        TileMap tileMap("assets/tilemap.png", 16, 16);

        Chunk chunk;
        chunk.set(DIRT.getDefaultData(), 0, 0, 0);
        chunk.set(STONE.getDefaultData(), 1, 0, 0);
        chunk.set(DIRT.getDefaultData(), 1, 1, 0);
        chunk.set(STONE.getDefaultData(), 1, 1, 1);
        
        ChunkMeshGenerator meshGen;
        Mesh mesh = meshGen.generateChunkMesh(
            BLOCK_REGISTRY, 
            tileMap, 
            chunk
        );
        UploadMesh(&mesh, false);

        Material material = LoadMaterialDefault();
        SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, tileMap.getTexture());

        Camera3D camera = {
            .position = { 24, 16, 24 },
            .target   = { 8, 8, 8 },
            .up       = { 0, 1, 0 },
            .fovy     = 60,
            .projection = CAMERA_PERSPECTIVE,
        };

        while (!WindowShouldClose()) {
            UpdateCamera(&camera, CAMERA_FREE);

            if (IsKeyPressed(KEY_TAB)) {
                IsCursorHidden() 
                    ? EnableCursor() 
                    : DisableCursor();
            }

            BeginDrawing();
                ClearBackground(SKYBLUE);
                BeginMode3D(camera);
                    DrawMesh(mesh, material, MatrixIdentity());
                EndMode3D();
            EndDrawing();
        }

        UnloadMesh(mesh);
        material.maps[MATERIAL_MAP_ALBEDO].texture = {};
        UnloadMaterial(material);
    }

    CloseWindow();

    return 0;
}

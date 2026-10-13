#include <raylib.h>

#include "BlockRegistry.hpp"
#include "ChunkMeshGenerator.hpp"

int main() {
    InitWindow(800, 600, "Hello gays");
    SetTargetFPS(60);

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

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(Color(255, 255, 0, 255));
            DrawText("Hello Gays", 190, 200, 20, Color(255, 255, 255, 255));
        EndDrawing();
    }

    return 0;
}

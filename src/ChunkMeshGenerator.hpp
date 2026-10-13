#pragma once

#include <raylib.h>

#include "Chunk.hpp"

class ChunkMeshGenerator {
    Mesh generateChunkMesh(const Chunk& chunk) {
        constexpr int maxFaces = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6;

        Mesh mesh = {};
        mesh.triangleCount = maxFaces * 2;
        mesh.vertexCount = maxFaces * 6;
        mesh.vertices = static_cast<float*>(
            MemAlloc(mesh.vertexCount * 3 * sizeof(float))
        );
        mesh.texcoords = static_cast<float*>(
            MemAlloc(mesh.vertexCount * 2 * sizeof(float))
        );
        mesh.normals = static_cast<float*>(
            MemAlloc(mesh.vertexCount * 3 * sizeof(float))
        );



        return mesh;
    }

    void processBlock(
        const Chunk& chunk, 
        Mesh& mesh,
        int x, 
        int y, 
        int z
    ) {
        bool hasTop    = chunk.isInBounds(x, y + 1, z) && chunk.hasSolid(x, y + 1, z);
        bool hasBottom = chunk.isInBounds(x, y - 1, z) && chunk.hasSolid(x, y - 1, z);
        bool hasLeft   = chunk.isInBounds(x - 1, y, z) && chunk.hasSolid(x - 1, y, z);
        bool hasRight  = chunk.isInBounds(x + 1, y, z) && chunk.hasSolid(x + 1, y, z);
        bool hasFront  = chunk.isInBounds(x, y, z + 1) && chunk.hasSolid(x, y, z + 1);
        bool hasBack   = chunk.isInBounds(x, y, z - 1) && chunk.hasSolid(x, y, z - 1);

        if (!hasTop) {
            addFaceTop(mesh, x, y, z);
        }

        // if (!hasBottom)
    }

    void addFaceTop(Mesh& mesh, int xi, int yi, int zi) {
        float x = (float) xi;
        float y = (float) yi;
        float z = (float) zi;

        float vertices[18] = { 
            x,       y + 1, z,
            x,       y + 1, z + 1,
            x + 1,   y + 1, z,
            x + 1,   y + 1, z,
            x,       y + 1, z + 1,
            x + 1,   y + 1, z + 1,
        };

        float uvs[12] = { 
            (u,       v)
            (u,       v + 0.5)
            (u + 0.5, v)
            (u + 0.5, v)
            (u,       v + 0.5)
            (u + 0.5, v + 0.5)
        };

        std::copy(vertices, vertices + 18, mesh.vertices);
    }
};
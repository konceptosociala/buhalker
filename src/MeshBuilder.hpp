#pragma once

#include "ChunkMesh.hpp"

class MeshBuilder {
    static constexpr int VERTICES_PER_FACE = 6;

    ChunkMesh& target;
    int vertexCount = 0;

    public:
        MeshBuilder(ChunkMesh& target, int faceCount) : target(target) {
            target.reserve(faceCount * VERTICES_PER_FACE);
        }

        void addVertex(
            float x, float y, float z,
            float u, float v,
            float nx, float ny, float nz
        ) {
            float* p = target.value.vertices + vertexCount * 3;
            float* t = target.value.texcoords + vertexCount * 2;
            float* n = target.value.normals + vertexCount * 3;

            p[0] = x;  p[1] = y;  p[2] = z;
            t[0] = u;  t[1] = v;
            n[0] = nx; n[1] = ny; n[2] = nz;

            vertexCount++;
        }

        int getVertexCount() const {
            return vertexCount;
        }

        void finish() {
            target.commit(vertexCount);
        }
};

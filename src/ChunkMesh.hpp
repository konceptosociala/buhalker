#pragma once

#include <algorithm>
#include <cstring>
#include <raylib.h>

struct ChunkMesh {
    Mesh value = {};
    int capacity = 0;

    void reserve(int vertexCount) {
        if (vertexCount <= capacity) {
            return;
        }

        const int newCapacity = std::max(vertexCount, capacity + capacity / 2);

        unload();

        value.vertices  = allocBuffer(newCapacity * 3);
        value.texcoords = allocBuffer(newCapacity * 2);
        value.normals   = allocBuffer(newCapacity * 3);
        value.vertexCount = newCapacity;
        value.triangleCount = newCapacity / 3;

        UploadMesh(&value, true);
        capacity = newCapacity;
    }

    void commit(int vertexCount) {
        value.vertexCount = vertexCount;
        value.triangleCount = vertexCount / 3;

        if (vertexCount == 0) {
            return;
        }

        UpdateMeshBuffer(value, 0, value.vertices,  vertexCount * 3 * sizeof(float), 0);
        UpdateMeshBuffer(value, 1, value.texcoords, vertexCount * 2 * sizeof(float), 0);
        UpdateMeshBuffer(value, 2, value.normals,   vertexCount * 3 * sizeof(float), 0);
    }

    bool isEmpty() const {
        return value.vertexCount == 0;
    }

    void unload() {
        if (capacity > 0) {
            UnloadMesh(value);
        }

        value = {};
        capacity = 0;
    }

    private:
        static float* allocBuffer(int floats) {
            float* p = static_cast<float*>(MemAlloc(floats * sizeof(float)));
            std::memset(p, 0, floats * sizeof(float));
            return p;
        }
};

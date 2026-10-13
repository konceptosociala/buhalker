#pragma once

#include <cstring>
#include <vector>
#include <raylib.h>

class MeshBuilder {
    static constexpr int VERTICES_PER_FACE = 6;

    std::vector<float> vertices;
    std::vector<float> texcoords;
    std::vector<float> normals;
    int vertexCount = 0;

    public:
        explicit MeshBuilder(int faceCount) {
            int total = faceCount * VERTICES_PER_FACE;

            vertices.resize(total * 3);
            texcoords.resize(total * 2);
            normals.resize(total * 3);
        }

        void addVertex(
            float x, float y, float z,
            float u, float v,
            float nx, float ny, float nz
        ) {
            float* p = vertices.data() + vertexCount * 3;
            float* t = texcoords.data() + vertexCount * 2;
            float* n = normals.data() + vertexCount * 3;

            p[0] = x;  p[1] = y;  p[2] = z;
            t[0] = u;  t[1] = v;
            n[0] = nx; n[1] = ny; n[2] = nz;

            vertexCount++;
        }

        int getVertexCount() const {
            return vertexCount;
        }

        Mesh toMesh() const {
            Mesh mesh = {};
            mesh.vertexCount = vertexCount;
            mesh.triangleCount = vertexCount / 3;

            mesh.vertices = static_cast<float*>(
                MemAlloc(vertexCount * 3 * sizeof(float))
            );
            mesh.texcoords = static_cast<float*>(
                MemAlloc(vertexCount * 2 * sizeof(float))
            );
            mesh.normals = static_cast<float*>(
                MemAlloc(vertexCount * 3 * sizeof(float))
            );

            std::memcpy(mesh.vertices, vertices.data(), vertexCount * 3 * sizeof(float));
            std::memcpy(mesh.texcoords, texcoords.data(), vertexCount * 2 * sizeof(float));
            std::memcpy(mesh.normals, normals.data(), vertexCount * 3 * sizeof(float));

            return mesh;
        }
};

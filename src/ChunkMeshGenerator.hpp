#pragma once

#include <flecs.h>
#include <raylib.h>

#include "MeshBuilder.hpp"
#include "Utils.hpp"
#include "Chunk.hpp"
#include "TileMap.hpp"
#include "BlockRegistry.hpp"
#include "EcsStages.hpp"

enum class Face {
    Top,
    Bottom,
    Left,
    Right,
    Front,
    Back
};

struct Corner {
    float x, y, z;
};

struct FaceDef {
    Face face;
    int dx, dy, dz;
    Corner corners[4];
};

constexpr FaceDef FACES[6] = {
    {
        Face::Top, 0, 1, 0,
        {
            Corner(0, 1, 0), 
            Corner(0, 1, 1), 
            Corner(1, 1, 0), 
            Corner(1, 1, 1),
        }
    },
    {
        Face::Bottom, 0, -1, 0,
        {
            Corner(0, 0, 1), 
            Corner(0, 0, 0), 
            Corner(1, 0, 1), 
            Corner(1, 0, 0),
        }
    },
    {
        Face::Left, -1, 0, 0,
        {
            Corner(0, 1, 0), 
            Corner(0, 0, 0), 
            Corner(0, 1, 1), 
            Corner(0, 0, 1),
        }
    },
    {
        Face::Right, 1, 0, 0,
        {
            Corner(1, 1, 1), 
            Corner(1, 0, 1), 
            Corner(1, 1, 0), 
            Corner(1, 0, 0),
        }
    },
    {
        Face::Front, 0, 0, 1,
        {
            Corner(0, 1, 1), 
            Corner(0, 0, 1), 
            Corner(1, 1, 1), 
            Corner(1, 0, 1),
        }
    },
    {
        Face::Back, 0, 0, -1,
        {
            Corner(1, 1, 0), 
            Corner(1, 0, 0), 
            Corner(0, 1, 0), 
            Corner(0, 0, 0),
        }
    },
};

constexpr int FACE_TRIANGLES[6] = { 0, 1, 2, 2, 1, 3 };
constexpr float FACE_UVS[4][2] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };

class ChunkMeshGenerator {
    public:
        ChunkMeshGenerator() = default;

        // Rebuilds mesh in place, reusing its 
        // GPU buffers when they are big enough.
        void generateChunkMesh(
            const BlockRegistry& blocks,
            const TileMap& tileMap,
            const Chunk& chunk,
            ChunkMesh& mesh
        ) {
            int faceCount = countFaces(chunk);

            MeshBuilder builder(mesh, faceCount);

            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        processBlock(chunk, blocks, tileMap, builder, x, y, z);
                    }
                }
            }

            builder.finish();
        }

    private:
        const SingleTile& getFaceTile(const SixTile& tile, Face face) {
            switch (face) {
                case Face::Top:    return tile.top;
                case Face::Bottom: return tile.bottom;
                case Face::Left:   return tile.left;
                case Face::Right:  return tile.right;
                case Face::Front:  return tile.front;
                case Face::Back:   return tile.back;
            }

            return tile.top;
        }

        bool isFaceVisible(
            const Chunk& chunk,
            const FaceDef& face,
            int x,
            int y,
            int z
        ) {
            int nx = x + face.dx;
            int ny = y + face.dy;
            int nz = z + face.dz;

            return !(chunk.isInBounds(nx, ny, nz) && chunk.hasSolid(nx, ny, nz));
        }

        int countFaces(const Chunk& chunk) {
            int count = 0;

            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        if (!chunk.hasSolid(x, y, z)) {
                            continue;
                        }

                        for (const FaceDef& face : FACES) {
                            if (isFaceVisible(chunk, face, x, y, z)) {
                                count++;
                            }
                        }
                    }
                }
            }

            return count;
        }

        void processBlock(
            const Chunk& chunk,
            const BlockRegistry& blocks,
            const TileMap& tileMap,
            MeshBuilder& builder,
            int x,
            int y,
            int z
        ) {
            if (!chunk.hasSolid(x, y, z)) {
                return;
            }

            const Block& block = blocks.getBlock(chunk.get(x, y, z));

            float uo = tileMap.getRelativeTileWidth();
            float vo = tileMap.getRelativeTileHeight();

            for (const FaceDef& face : FACES) {
                if (isFaceVisible(chunk, face, x, y, z)) {
                    addFace(builder, tileMap, block, face, x, y, z, uo, vo);
                }
            }
        }

        void addFace(
            MeshBuilder& builder,
            const TileMap& tileMap,
            const Block& block,
            const FaceDef& face,
            int xi,
            int yi,
            int zi,
            float uo,
            float vo
        ) {
            const SingleTile t = match(block.getTile(),
                [](const SingleTile& t) -> SingleTile { return t; },
                [&](const SixTile& t) -> SingleTile   { return getFaceTile(t, face.face); },
                [&](const InvalidTile&) -> SingleTile { return std::get<SingleTile>(tileMap.invalid()); }
            );

            float u = uo * t.u, v = vo * t.v;

            for (int index : FACE_TRIANGLES) {
                const Corner& c = face.corners[index];
                const float* uv = FACE_UVS[index];

                builder.addVertex(
                    xi + c.x, yi + c.y, zi + c.z,
                    u + uo * uv[0], v + vo * uv[1],
                    face.dx, face.dy, face.dz
                );
            }
        }
};

struct ChunkMeshGeneratorModule {
    ChunkMeshGeneratorModule(flecs::world& world) {
        world.module<ChunkMeshGeneratorModule>();

        world.component<ChunkMeshGenerator>().add(flecs::Singleton);
        world.component<TileMap>().add(flecs::Singleton);

        world.add<ChunkMeshGenerator>();
        world.emplace<TileMap>("assets/tilemap.png", 16.0f, 16.0f);

        world.system<ChunkMeshGenerator, const TileMap, const Chunk, ChunkMesh>("Chunk mesh generation")
            .with<DirtyChunk>()
            .kind<Update>()
            .each([](
                flecs::entity e,
                ChunkMeshGenerator generator,
                const TileMap& tileMap,
                const Chunk& chunk,
                ChunkMesh& mesh
            ) {
                generator.generateChunkMesh(BLOCK_REGISTRY, tileMap, chunk, mesh);
                e.remove<DirtyChunk>();
            });

        world.system<ChunkMesh>("Dispose chunk meshes")
            .kind<Dispose>()
            .each([](ChunkMesh& mesh) {
                mesh.unload();
            });
    }
};

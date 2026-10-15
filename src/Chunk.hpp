#pragma once

#include <array>
#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "Block.hpp"
#include "ChunkMesh.hpp"
#include "BlockRegistry.hpp"
#include "EcsStages.hpp"

constexpr int CHUNK_SIZE = 16;
constexpr int VOXEL_COUNT = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class Chunk {
    std::array<BlockData, VOXEL_COUNT> voxels{};

    public: 
        Chunk() = default;

        static constexpr int index(int x, int y, int z) {
            return x + CHUNK_SIZE * y + CHUNK_SIZE * CHUNK_SIZE * z;
        }

        void set(BlockData blockData, int x, int y, int z) {
            voxels[index(x, y, z)] = blockData;
        }

        BlockData get(int x, int y, int z) const {
            return voxels[index(x, y, z)];
        }

        BlockData& getMut(int x, int y, int z) {
            return voxels[index(x, y, z)];
        }

        bool isInBounds(int x, int y, int z) const {
            return x >= 0 && x < CHUNK_SIZE
                && y >= 0 && y < CHUNK_SIZE
                && z >= 0 && z < CHUNK_SIZE;
        }

        bool hasSolid(int x, int y, int z) const {
            return !this->get(x, y, z).isAir();
        }

        static Chunk testChunk() {
            Chunk chunk;
            for (int x = 0; x < 16; x++) {
                for (int z = 0; z < 16; z++) {
                    chunk.set(DIRT.getDefaultData(), x, 2, z);
                    chunk.set(STONE.getDefaultData(), x, 1, z);
                }
            }
            return chunk;
        }
};

struct ChunkPos {
    int x, y, z;

    bool operator==(const ChunkPos&) const = default;
};

struct ChunkHash {
    size_t operator()(const ChunkPos& p) const {
        size_t h = std::hash<int>{}(p.x);
        h ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(p.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

using ChunkStorage = std::unordered_map<ChunkPos, flecs::entity, ChunkHash>;

struct DirtyChunk {};

struct LoadingChunk {};

struct ChunkMaterial {
    Material value;

    ChunkMaterial(const TileMap& tileMap) {
        value = LoadMaterialDefault();
        SetMaterialTexture(&value, MATERIAL_MAP_ALBEDO, tileMap.getTexture());
    }
};

struct ChunkModule {
    ChunkModule(flecs::world& world) {
        world.module<ChunkModule>();
        
        world.component<ChunkStorage>().add(flecs::Singleton);
        world.set(ChunkStorage());

        // --------------- Test chunk ---------------
        flecs::entity chunk = world.entity()
            .set(Chunk::testChunk())
            .set(ChunkMesh())
            .emplace<ChunkMaterial>(world.get<TileMap>())
            .add<DirtyChunk>();

        world.get_mut<ChunkStorage>()[{0, 0, 0}] = chunk;
        // ------------------------------------------

        world.system<const ChunkMesh, const ChunkMaterial>("Chunk rendering system")
            .kind<Draw3D>()
            .each([](const ChunkMesh& mesh, const ChunkMaterial& mat) {
                DrawMesh(mesh.value, mat.value, MatrixIdentity());
            });

        world.system<const ChunkMaterial>("Chunk dispose system")
            .kind<Dispose>()
            .each([](const ChunkMaterial& mat) {
                mat.value.maps[MATERIAL_MAP_ALBEDO].texture = {};
                UnloadMaterial(mat.value);
            });
    }
};
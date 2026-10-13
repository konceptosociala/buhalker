#pragma once

#include <array>

#include "Block.hpp"

constexpr int CHUNK_SIZE = 16;
constexpr int VOXEL_COUNT = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

struct Chunk {
    std::array<BlockData, VOXEL_COUNT> voxels{};

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
};
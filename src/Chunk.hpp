#pragma once

#include <array>
#include <cstdint>
#include <string>

const int CHUNK_SIZE = 16;
const int VOXEL_COUNT = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class Block {

    std::string name;
};

struct BlockData {
    uint32_t data;

    bool isAir() {
        return data == 0;
    }
};

struct Chunk {
    std::array<BlockData, VOXEL_COUNT> voxels;

    static constexpr int index(int x, int y, int z) {
        return x + CHUNK_SIZE * y + CHUNK_SIZE * CHUNK_SIZE * z;
    }

    BlockData& at(int x, int y, int z) {
        return voxels[index(x, y, z)];
    }

    BlockData at(int x, int y, int z) const {
        return voxels[index(x, y, z)];
    }

    bool isInBounds(int x, int y, int z) const {
        return x >= 0 && x < CHUNK_SIZE
            && y >= 0 && y < CHUNK_SIZE
            && z >= 0 && z < CHUNK_SIZE;
    }

    bool hasSolid(int x, int y, int z) const {
        return !this->at(x, y, z).isAir();
    }
};
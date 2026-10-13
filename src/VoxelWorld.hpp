#pragma once

#include <cstddef>
#include <functional>

#include "Chunk.hpp"

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

using VoxelWorld = std::unordered_map<ChunkPos, Chunk, ChunkHash>;

void x() {
    Chunk x = Chunk();
}
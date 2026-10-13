#pragma once

#include <cstdint>
#include <unordered_map>

#include "Block.hpp"

class BlockRegistry {

    std::unordered_map<uint16_t, Block> blocks;

    public:
        BlockRegistry(std::initializer_list<Block> list) {
            for (auto block : list) {
                uint16_t id = block.getId();
                bool inserted = blocks.try_emplace(id, block).second;

                if (!inserted) {
                    throw std::logic_error(std::format("BlockRegistry: duplicate block ID: {}", id));
                }
            }
        }

        const Block& getBlock(uint16_t id) const {
            return blocks.at(id);
        }

        const Block& getBlock(BlockData data) const {
            return getBlock(data.getId());
        }

        Block& getBlockMut(uint16_t id) {
            return blocks.at(id);
        }

        Block& getBlockMut(BlockData data) {
            return getBlockMut(data.getId());
        }
};

constexpr Block AIR = Block("air", 0);

constexpr Block STONE = Block("stone", 1)
    .withTile(SingleTile(10, 20));

constexpr Block DIRT = Block("dirt", 2)
    .withTile(SingleTile(13, 3));

static const BlockRegistry BLOCK_REGISTRY({
    AIR,
    STONE,
    DIRT
});
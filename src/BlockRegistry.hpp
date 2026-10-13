#pragma once

#include <cstdint>
#include <unordered_map>

#include "Block.hpp"

class BlockRegistry {

    std::unordered_map<uint16_t, Block> blocks;

    public:
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
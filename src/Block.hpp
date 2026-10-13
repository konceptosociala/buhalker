#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <raylib.h>

#include "TileMap.hpp"

struct BlockData {
    static constexpr uint32_t ID_MASK = 0x3FF;

    // 1-10 bits = block id 0-1023
    uint32_t data;

    constexpr BlockData() : data(0) {}

    constexpr BlockData(uint32_t data) : data(data) {}

    static constexpr BlockData fromId(uint16_t id) {
        if (id > 1023) {
            throw std::out_of_range(std::format("Block ID exceeds 10 bits: {}", id));
        }

        return BlockData(id);
    }

    uint16_t getId() const {
        return static_cast<uint16_t>(data & ID_MASK);
    }

    bool isAir() const {
        return data == 0;
    }
};

class Block {
    std::string name;
    TextureTile tile;
    BlockData defaultData;

    public:
        constexpr Block(std::string name, uint16_t id) 
            : name(name)
            , tile(InvalidTile {})
            , defaultData(BlockData::fromId(id)) {}

        std::uint16_t getId() const {
            return defaultData.getId();
        }

        std::string getName() const {
            return name;
        }
    
        BlockData getDefaultData() const {
            return defaultData;
        }

        const TextureTile& getTile() const {
            return tile;
        }

        constexpr Block& withTile(TextureTile tile) {
            this->tile = std::move(tile);
            return *this;
        }
};
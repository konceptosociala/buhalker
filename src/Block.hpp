#pragma once

#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>
#include <variant>
#include <raylib.h>

struct BlockData {
    static constexpr uint32_t ID_MASK = 0x3FF;

    // 1-10 bits = block id 0-1023
    uint32_t data;

    uint16_t getId() {
        return static_cast<uint16_t>(data & ID_MASK);
    }

    bool isAir() {
        return data == 0;
    }
};

struct SingleTile {
    uint8_t u;
    uint8_t v;
};

struct SixTile {
    SingleTile top;
    SingleTile bottom;
    SingleTile left;
    SingleTile right;
    SingleTile front;
    SingleTile back;
};

using TextureTile = std::variant<SingleTile, SixTile>;

class TileMap {
    Texture2D texture;
    float tileWidth, tileHeight;
    float relativeTileWidth, relativeTileHeight;

    public:
        TileMap(
            std::string path,
            float tileWidth, 
            float tileHeight
        ) : tileWidth(tileWidth), tileHeight(tileHeight) {
            if (tileWidth < 0) {
                throw std::runtime_error(std::format("Tile width is less than 0: {}", tileWidth));
            }

            if (tileHeight < 0) {
                throw std::runtime_error(std::format("Tile height is less than 0: {}", tileHeight));
            }

            texture = LoadTexture(path.c_str());

            if (!IsTextureValid(texture)) {
                throw std::runtime_error(std::format("Error loading texture: {}", path));
            }

            if (tileWidth > texture.width) {
                throw std::runtime_error(std::format("Tile width is bigger than texture width: {} > {}", tileWidth, texture.width));
            }

            if (tileHeight > texture.height) {
                throw std::runtime_error(std::format("Tile height is bigger than texture height: {} > {}", tileHeight, texture.height));
            }

            relativeTileWidth = tileWidth / texture.width;
            relativeTileHeight = tileHeight / texture.height;
        }

        ~TileMap() {
            UnloadTexture(texture);
        }

        float getTileWidth() const { return tileWidth; }
        float getTileHeight() const { return tileHeight; }
        float getRelativeTileWidth() const { return relativeTileWidth; }
        float getRelativeTileHeight() const { return relativeTileHeight; }      
};

class Block {
    std::string id;
    TextureTile tile;
    BlockData defaultData;

    public:
        std::string getId() {
            return id;
        }
    
        BlockData getDefaultData() const {
            return defaultData;
        }

        const TextureTile& getTile() const {
            return tile;
        }
};
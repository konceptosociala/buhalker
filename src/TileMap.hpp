#pragma once

#include <raylib.h>
#include <cstdint>
#include <string>
#include <variant>
#include <format>
#include <stdexcept>

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

struct InvalidTile {};

using TextureTile = std::variant<SingleTile, SixTile, InvalidTile>;

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
            SetTextureFilter(texture, TEXTURE_FILTER_POINT);

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

        TileMap(const TileMap&) = delete;
        TileMap& operator=(const TileMap&) = delete;

        /// Call only in "Dispose" systems
        void dispose() {
            UnloadTexture(texture);
        }

        TextureTile invalid() const {
            uint8_t resW = texture.width / tileWidth;
            uint8_t resH = texture.height / tileHeight;

            return SingleTile(resW - 1, resH - 1);
        }

        const Texture2D& getTexture() const { return texture; }
        float getTileWidth() const { return tileWidth; }
        float getTileHeight() const { return tileHeight; }
        float getRelativeTileWidth() const { return relativeTileWidth; }
        float getRelativeTileHeight() const { return relativeTileHeight; }      
};
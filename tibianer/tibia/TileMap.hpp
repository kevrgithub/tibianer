#ifndef TIBIA_TILEMAP_HPP
#define TIBIA_TILEMAP_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Tile.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class TileMap
{

public:

    void load(unsigned int width, unsigned int height, std::vector<int> tiles, const std::string& name, int type, int z)
    {
        m_tiles.clear();

        m_tileList.clear();

        m_waterTileNumbers.clear();

        //m_tiles = tiles;
        //m_tiles = std::move(tiles);
        m_tiles.swap(tiles);

        m_name = name;

        m_type = type;

        m_z = z;

        m_waterOffset = 0;

        for (unsigned int i = 0; i < width; ++i)
        {
            for (unsigned int j = 0; j < height; ++j)
            {
                int tileNumber = i + j * width;

                int tileId = m_tiles.at(tileNumber);

                sf::Vector2u tilePosition
                (
                    i * tibia::TILE_SIZE,
                    j * tibia::TILE_SIZE
                );

                tibia::SpriteFlags_t tileFlags = tibia::UMaps::spriteFlags[tileId];

                if (tileFlags.test(tibia::SpriteFlags::water) && m_type == tibia::TileMapTypes::tiles && m_z == tibia::ZAxis::ground)
                {
                    m_waterTileNumbers.push_back(tileNumber);
                }

                if (tileId == tibia::TILE_NULL && m_type == tibia::TileMapTypes::tiles)
                {
                    tileFlags.set(tibia::SpriteFlags::null);
                }

                tibia::Tile::Ptr tile = std::make_shared<tibia::Tile>();
                tile->setNumber(tileNumber);
                tile->setId(tileId);
                tile->setPosition(tilePosition);
                tile->setZ(z);
                tile->setFlags(tileFlags);
                m_tileList.push_back(tile);
            }
        }

        std::sort(m_tileList.begin(), m_tileList.end(), tibia::Tile::sortByTileNumber_t());

        applyTilePatterns();
    }

    void updateTileId(int tileNumber, int tileId)
    {
        m_tiles.at(tileNumber) = tileId;

        m_tileList.at(tileNumber)->setId(tileId);
    }

    void updateTileFlags(int tileNumber, int tileId)
    {
        tibia::SpriteFlags_t tileFlags = tibia::UMaps::spriteFlags[tileId];

        m_tileList.at(tileNumber)->setFlags(tileFlags);
    }

    std::vector<int>* getTiles()
    {
        return &m_tiles;
    }

    tibia::Tile::List* getTileList()
    {
        return &m_tileList;
    }

    std::vector<int>* getWaterTileNumbers()
    {
        return &m_waterTileNumbers;
    }

    void loadWaterTileNumbers()
    {
        m_waterTileNumbers.clear();

        for (auto& tile : m_tileList)
        {
            if (tile->getFlags().test(tibia::SpriteFlags::water))
            {
                m_waterTileNumbers.push_back(tile->getNumber());
            }
        }
    }

    void doAnimatedWater()
    {
        for (auto tileNumber : m_waterTileNumbers)
        {
            int tileId = m_tiles.at(tileNumber);

            if (tileId >= tibia::SpriteData::waterBegin && tileId <= tibia::SpriteData::waterEnd)
            {
                if (tileId == tibia::SpriteData::water[3])
                {
                    tileId = tibia::SpriteData::water[0];
                }
                else if (tileId == tibia::SpriteData::water[7])
                {
                    tileId = tibia::SpriteData::water[4];
                }
                else
                {
                    tileId++;
                }

                updateTileId(tileNumber, tileId);
            }
        }
    }

    void doAnimatedWaterEx()
    {
        if (m_waterOffset > 3)
        {
            m_waterOffset = 0;
        }

        for (auto tileNumber : m_waterTileNumbers)
        {
            int tileId = m_tiles.at(tileNumber);

            if (tileId >= tibia::SpriteData::waterBegin && tileId <= tibia::SpriteData::waterEnd)
            {
                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int waterIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                waterIndex = tibia::mapWidth - waterIndex;

                while (waterIndex > 3)
                {
                    waterIndex = waterIndex - 4;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    waterIndex += 4;
                }

                if (waterIndex < 4)
                {
                    waterIndex += m_waterOffset;

                    while (waterIndex > 3)
                    {
                        waterIndex = waterIndex - 4;
                    }
                }
                else
                {
                    waterIndex += m_waterOffset;

                    while (waterIndex > 7)
                    {
                        waterIndex = waterIndex - 4;
                    }
                }

                updateTileId(tileNumber, tibia::SpriteData::water[waterIndex]);
            }
        }

        m_waterOffset++;
    }

    void applyTilePatterns()
    {
        for (auto& tile : m_tileList)
        {
            int tileId = tile->getId();

            // water
            if (tileId >= tibia::SpriteData::waterBegin && tileId <= tibia::SpriteData::waterEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 4;
                }

                updateTileId(tileNumber, tibia::SpriteData::water[spriteIndex]);
            }

            // lava
            if (tileId >= tibia::SpriteData::lavaBegin && tileId <= tibia::SpriteData::lavaEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 4;
                }

                updateTileId(tileNumber, tibia::SpriteData::lava[spriteIndex]);
            }

            // oil
            if (tileId >= tibia::SpriteData::oilBegin && tileId <= tibia::SpriteData::oilEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 4;
                }

                updateTileId(tileNumber, tibia::SpriteData::oil[spriteIndex]);
            }

            // grey tiles
            if (tileId >= tibia::SpriteData::greyTilesBegin && tileId <= tibia::SpriteData::greyTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                int row = tibia::mapHeight - tileCoords.y;

                row = tibia::mapHeight - row;

                while (row > 3)
                {
                    row = row - 4;
                }

                if (row > 0)
                {
                    spriteIndex += 4 * row;
                }

                updateTileId(tileNumber, tibia::SpriteData::greyTiles[spriteIndex]);
            }

            // brown tiles
            if (tileId >= tibia::SpriteData::brownTilesBegin && tileId <= tibia::SpriteData::brownTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 2)
                {
                    spriteIndex = spriteIndex - 3;
                }

                int row = tibia::mapHeight - tileCoords.y;

                row = tibia::mapHeight - row;

                while (row > 2)
                {
                    row = row - 3;
                }

                if (row > 0)
                {
                    spriteIndex += 3 * row;
                }

                updateTileId(tileNumber, tibia::SpriteData::brownTiles[spriteIndex]);
            }

            // brown ladder tiles
            if (tileId >= tibia::SpriteData::brownLadderTilesBegin && tileId <= tibia::SpriteData::brownLadderTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 2)
                {
                    spriteIndex = spriteIndex - 3;
                }

                int row = tibia::mapHeight - tileCoords.y;

                row = tibia::mapHeight - row;

                while (row > 2)
                {
                    row = row - 3;
                }

                if (row > 0)
                {
                    spriteIndex += 3 * row;
                }

                updateTileId(tileNumber, tibia::SpriteData::brownLadderTiles[spriteIndex]);
            }

            // orange black tiles
            if (tileId >= tibia::SpriteData::orangeBlackTilesBegin && tileId <= tibia::SpriteData::orangeBlackTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                int row = tibia::mapHeight - tileCoords.y;

                row = tibia::mapHeight - row;

                while (row > 3)
                {
                    row = row - 4;
                }

                if (row > 0)
                {
                    spriteIndex += 4 * row;
                }

                updateTileId(tileNumber, tibia::SpriteData::orangeBlackTiles[spriteIndex]);
            }

            // stone tiles
            if (tileId == tibia::SpriteData::stoneTilesBegin || tileId == tibia::SpriteData::stoneTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 1)
                {
                    spriteIndex = spriteIndex - 2;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 2;
                }

                updateTileId(tileNumber, tibia::SpriteData::stoneTiles[spriteIndex]);
            }

            // cobblestone tiles
            if (tileId >= tibia::SpriteData::cobbleStoneTilesBegin && tileId <= tibia::SpriteData::cobbleStoneTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 4;
                }

                updateTileId(tileNumber, tibia::SpriteData::cobbleStoneTiles[spriteIndex]);
            }

            // yellow tiles
            if (tileId >= tibia::SpriteData::yellowTilesBegin && tileId <= tibia::SpriteData::yellowTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 1)
                {
                    spriteIndex = spriteIndex - 2;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 2;
                }

                updateTileId(tileNumber, tibia::SpriteData::yellowTiles[spriteIndex]);
            }

            // cracked sand tiles
            if (tileId >= tibia::SpriteData::crackedSandTilesBegin && tileId <= tibia::SpriteData::crackedSandTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 1)
                {
                    spriteIndex = spriteIndex - 2;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 2;
                }

                updateTileId(tileNumber, tibia::SpriteData::crackedSandTiles[spriteIndex]);
            }

            // brick tiles
            if (tileId >= tibia::SpriteData::brickTilesBegin && tileId <= tibia::SpriteData::brickTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 1)
                {
                    spriteIndex = spriteIndex - 2;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 2;
                }

                updateTileId(tileNumber, tibia::SpriteData::brickTiles[spriteIndex]);
            }

            // mountain tiles
            if (tileId >= tibia::SpriteData::mountainTilesBegin && tileId <= tibia::SpriteData::mountainTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 1)
                {
                    spriteIndex = spriteIndex - 2;
                }

                // odd row
                if (tileCoords.y & 1)
                {
                    spriteIndex += 2;
                }

                updateTileId(tileNumber, tibia::SpriteData::mountainTiles[spriteIndex]);
            }

            // sand tiles
            if (tileId >= tibia::SpriteData::sandTilesBegin && tileId <= tibia::SpriteData::sandTilesEnd)
            {
                int tileNumber = tile->getNumber();

                sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                int spriteIndex = tibia::mapWidth - (tileNumber % tibia::mapWidth);

                spriteIndex = tibia::mapWidth - spriteIndex;

                while (spriteIndex > 3)
                {
                    spriteIndex = spriteIndex - 4;
                }

                int row = tibia::mapHeight - tileCoords.y;

                row = tibia::mapHeight - row;

                while (row > 3)
                {
                    row = row - 4;
                }

                if (row > 0)
                {
                    spriteIndex += 4 * row;
                }

                updateTileId(tileNumber, tibia::SpriteData::sandTiles[spriteIndex]);
            }
        }
    }

    void applyTileObjectPatterns()
    {
        for (auto& tile : m_tileList)
        {
            tibia::Object::List* objectList = tile->getObjectList();

            if (objectList->size() == 0)
            {
                continue;
            }

            for (auto& object : *objectList)
            {
                int objectId = object->getId();

                // horizontal stone wall
                if (objectId == 566 || objectId == 570)
                {
                    int tileNumber = tile->getNumber();

                    if (m_z & 1)
                    {
                        if (tileNumber & 1)
                        {
                            object->setId(566);
                        }
                        else
                        {
                            object->setId(570);
                        }
                    }
                    else
                    {
                        if (tileNumber & 1)
                        {
                            object->setId(570);
                        }
                        else
                        {
                            object->setId(566);
                        }
                    }
                }

                // vertical stone wall
                if (objectId == 584 || objectId == 588)
                {
                    int tileNumber = tile->getNumber();

                    sf::Vector2u tileCoords = getTileCoordsByTileNumber(tileNumber);

                    if (m_z & 1)
                    {
                        if (tileCoords.y & 1)
                        {
                            object->setId(584);
                        }
                        else
                        {
                            object->setId(588);
                        }
                    }
                    else
                    {
                        if (tileCoords.y & 1)
                        {
                            object->setId(588);
                        }
                        else
                        {
                            object->setId(584);
                        }
                    }
                }
            }
        }
    }

    sf::Vector2u getTileCoordsByTileNumber(int tileNumber)
    {
        return sf::Vector2u
        (
            tileNumber % tibia::mapWidth,
            tileNumber / tibia::mapHeight
        );
    }

    std::string getName()
    {
        return m_name;
    }

    void setName(std::string name)
    {
        m_name = name;
    }

    int getType()
    {
        return m_type;
    }

    void setType(int type)
    {
        m_type = type;
    }

    int getZ()
    {
        return m_z;
    }

    void setZ(int z)
    {
        m_z = z;
    }

private:

    std::string m_name;

    int m_type;

    int m_z;

    std::vector<int> m_tiles;

    tibia::Tile::List m_tileList;

    std::vector<int> m_waterTileNumbers;

    int m_waterOffset;

}; // class TileMap

} // namespace tibia

#endif // TIBIA_TILEMAP_HPP

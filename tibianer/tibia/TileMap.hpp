#ifndef TIBIA_TILEMAP_HPP
#define TIBIA_TILEMAP_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Tile.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class TileMap
{

public:

    void load(std::vector<int> tiles, const std::string& name, int type, int z)
    {
        //m_tiles = tiles;
        //m_tiles.swap(tiles);
        m_tiles = std::move(tiles);

        m_name = name;

        m_type = type;

        m_z = z;

        for (unsigned int i = 0; i < tibia::MAP_SIZE; ++i)
        {
            for (unsigned int j = 0; j < tibia::MAP_SIZE; ++j)
            {
                int tileNumber = i + j * tibia::MAP_SIZE;

                int tileId = m_tiles.at(tileNumber);

                sf::Vector2u tilePosition
                (
                    i * tibia::TILE_SIZE,
                    j * tibia::TILE_SIZE
                );

                int tileFlags = tibia::spriteFlags[tileId];

                if (tileFlags & tibia::SpriteFlags::water && m_type == tibia::TileMapTypes::tiles && m_z == tibia::ZAxis::ground)
                {
                    m_waterTileNumbers.push_back(tileNumber);
                }

                if (tileId == tibia::TILE_NULL && m_type == tibia::TileMapTypes::tiles)
                {
                    tileFlags |= tibia::SpriteFlags::null;
                }

                tibia::TilePtr tile = std::make_shared<tibia::Tile>();
                tile->setNumber(tileNumber);
                tile->setId(tileId);
                tile->setPosition(tilePosition);
                tile->setZ(z);
                tile->setFlags(tileFlags);
                m_tileList.push_back(tile);
            }
        }

        std::sort(m_tileList.begin(), m_tileList.end(), tibia::TileSort::sortTilesByTileNumber());
    }

    void updateTileId(int tileNumber, int tileId)
    {
        m_tiles.at(tileNumber) = tileId;

        m_tileList.at(tileNumber)->setId(tileId);
    }

    void updateTileFlags(int tileNumber, int tileId)
    {
        int tileFlags = tibia::spriteFlags[tileId];

        m_tileList.at(tileNumber)->setFlags(tileFlags);
    }

    std::vector<int>* getTiles()
    {
        return &m_tiles;
    }

    tibia::TileList* getTileList()
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

        for (auto tile : m_tileList)
        {
            if (tile->getFlags() & tibia::SpriteFlags::water)
            {
                m_waterTileNumbers.push_back(tile->getNumber());
            }
        }
    }

    void doAnimatedWater()
    {
        //std::cout << "m_waterTileNumbers size: " << m_waterTileNumbers.size() << std::endl;

        for (auto waterTileNumber : m_waterTileNumbers)
        {
            int tileId = m_tiles.at(waterTileNumber);

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

                updateTileId(waterTileNumber, tileId);
            }
        }
    }

    void addMiniMapTiles(std::vector<sf::Vertex>& vertexList)
    {
        for (unsigned int i = 0; i < tibia::MAP_SIZE; ++i)
        {
            for (unsigned int j = 0; j < tibia::MAP_SIZE; ++j)
            {
                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
                {
                    continue;
                }

                if (m_tileList.size() == 0 || m_tileList.size() < tileNumber)
                {
                    continue;
                }

                tibia::Tile* tile = m_tileList.at(tileNumber).get();

                int tileId = tile->getId();

                if (tileId == tibia::TILE_NULL || tileId == 1)
                {
                    continue;
                }

                int tileFlags = tile->getFlags();

                if (tileFlags & (tibia::SpriteFlags::solid | tibia::SpriteFlags::ladder | tibia::SpriteFlags::moveAbove | tibia::SpriteFlags::moveBelow))
                {
                    sf::Vertex quad[4];

                    quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                    quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                    quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                    quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                    sf::Color tileColor;

                    if (tileFlags & tibia::SpriteFlags::solid)
                    {
                        tileColor = tibia::Colors::tileIsSolid;
                    }

                    if (tileFlags & tibia::SpriteFlags::water)
                    {
                        tileColor = tibia::Colors::tileIsWater;
                    }

                    if (tileFlags & tibia::SpriteFlags::lava)
                    {
                        tileColor = tibia::Colors::tileIsLava;
                    }

                    if (tileFlags & (tibia::SpriteFlags::ladder | tibia::SpriteFlags::moveAbove | tibia::SpriteFlags::moveBelow))
                    {
                        tileColor = tibia::Colors::tileIsMoveAboveOrBelow;
                    }

                    quad[0].color = tileColor;
                    quad[1].color = tileColor;
                    quad[2].color = tileColor;
                    quad[3].color = tileColor;

                    vertexList.push_back(quad[0]);
                    vertexList.push_back(quad[1]);
                    vertexList.push_back(quad[2]);
                    vertexList.push_back(quad[3]);
                }
            }
        }
    }

    sf::Vector2u getTileCoordsByTileNumber(int tileNumber)
    {
        int tileId = m_tiles.at(tileNumber) - 1;

        //tileId = tileId - 1;

        return sf::Vector2u
        (
            tileId % (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE),
            tileId / (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE)
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

    tibia::TileList m_tileList;

    std::vector<int> m_waterTileNumbers;

};

} // tibia

#endif // TIBIA_TILEMAP_HPP

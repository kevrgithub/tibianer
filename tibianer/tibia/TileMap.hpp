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
        //m_tiles = tiles;
        //m_tiles.swap(tiles);
        m_tiles = std::move(tiles);

        m_name = name;

        m_type = type;

        m_z = z;

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

                unsigned int tileFlags = tibia::UMaps::spriteFlags[tileId];

                if ((tileFlags & tibia::SpriteFlags::water) && m_type == tibia::TileMapTypes::tiles && m_z == tibia::ZAxis::ground)
                {
                    m_waterTileNumbers.push_back(tileNumber);
                }

                if (tileId == tibia::TILE_NULL && m_type == tibia::TileMapTypes::tiles)
                {
                    tileFlags |= tibia::SpriteFlags::null;
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

        std::sort(m_tileList.begin(), m_tileList.end(), tibia::TileSort::sortTilesByTileNumber());
    }

    void updateTileId(int tileNumber, int tileId)
    {
        m_tiles.at(tileNumber) = tileId;

        m_tileList.at(tileNumber)->setId(tileId);
    }

    void updateTileFlags(int tileNumber, int tileId)
    {
        unsigned int tileFlags = tibia::UMaps::spriteFlags[tileId];

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

    tibia::Tile::List m_tileList;

    std::vector<int> m_waterTileNumbers;

}; // class TileMap

} // namespace tibia

#endif // TIBIA_TILEMAP_HPP

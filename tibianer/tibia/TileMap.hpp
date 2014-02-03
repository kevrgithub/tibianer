#ifndef TIBIA_TILEMAP_HPP
#define TIBIA_TILEMAP_HPP

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"

namespace tibia
{

class TileMap : public sf::Drawable, public sf::Transformable
{

public:

    void load(std::vector<int> tiles, bool useTileOffset = false)
    {
        m_tileSet = tibia::Textures::sprites;

        m_tiles = tiles;

        loadWaterTiles();

        m_width  = tibia::MAP_SIZE;
        m_height = tibia::MAP_SIZE;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(m_tiles.size() * 4);

        for (unsigned int i = 0; i < m_width; ++i)
        {
            for (unsigned int j = 0; j < m_height; ++j)
            {
                int tileNumber = m_tiles.at(i + j * m_width) - 1;

                int tu = tileNumber % (m_tileSet.getSize().x / tibia::TILE_SIZE);
                int tv = tileNumber / (m_tileSet.getSize().x / tibia::TILE_SIZE);

                sf::Vertex* quad = &m_vertices[(i + j * m_width) * 4];

                quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                quad[0].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
                quad[3].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);

                if (useTileOffset == true)
                {
                    quad[0].position.x -= tibia::TILE_DRAW_OFFSET;
                    quad[1].position.x -= tibia::TILE_DRAW_OFFSET;
                    quad[2].position.x -= tibia::TILE_DRAW_OFFSET;
                    quad[3].position.x -= tibia::TILE_DRAW_OFFSET;

                    quad[0].position.y -= tibia::TILE_DRAW_OFFSET;
                    quad[1].position.y -= tibia::TILE_DRAW_OFFSET;
                    quad[2].position.y -= tibia::TILE_DRAW_OFFSET;
                    quad[3].position.y -= tibia::TILE_DRAW_OFFSET;
                }
            }
        }
    }

    void updateTile(int tileNumber, int tileId)
    {
        m_tiles.at(tileNumber) = tileId;

        int currentTile = m_tiles.at(tileNumber);

        currentTile = currentTile - 1;

        int tu = currentTile % (m_tileSet.getSize().x / tibia::TILE_SIZE);
        int tv = currentTile / (m_tileSet.getSize().x / tibia::TILE_SIZE);

        sf::Vertex* quad = &m_vertices[tileNumber * 4];

        quad[0].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
        quad[3].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
    }

    std::vector<int> getTiles()
    {
        return m_tiles;
    }

    std::vector<int> getWaterTiles()
    {
        return m_waterTiles;
    }

    void loadWaterTiles()
    {
        for (unsigned int i = 0; i < m_tiles.size(); i++)
        {
            int tileId = m_tiles.at(i);

            if (tileId == tibia::TILE_NULL)
            {
                continue;
            }

            for (unsigned int j = 0; j < (sizeof(tibia::SpriteData::water) / sizeof(int)); j++)
            {
                if (tileId == tibia::SpriteData::water[j])
                {
                    m_waterTiles.push_back(i);
                }
            }
        }
    }

    void doAnimatedWater()
    {
        for (m_waterTiles_it = m_waterTiles.begin(); m_waterTiles_it != m_waterTiles.end(); m_waterTiles_it++)
        {
            int tileNumber = *m_waterTiles_it;

            int tileId = m_tiles.at(tileNumber);

            for (int j = 0; j < (sizeof(tibia::SpriteData::water) / sizeof(int)); j++)
            {
                if (tileId == tibia::SpriteData::water[j])
                {
                    int waterTileId = tibia::SpriteData::water[j];

                    if (waterTileId == tibia::SpriteData::water[3])
                    {
                        waterTileId = tibia::SpriteData::water[0];
                    }
                    else if (waterTileId == tibia::SpriteData::water[7])
                    {
                        waterTileId = tibia::SpriteData::water[4];
                    }
                    else
                    {
                        waterTileId += 1;
                    }

                    updateTile(tileNumber, waterTileId);

                    break;
                }
            }
        }
    }

    void doAnimatedObjects()
    {
        for (unsigned int tileNumber = 0; tileNumber < m_tiles.size(); tileNumber++)
        {
            int tileId = m_tiles.at(tileNumber);

            if (tileId == tibia::TILE_NULL)
            {
                continue;
            }

            for (unsigned int i = 0; i < tibia::animatedObjectsList.size(); i++)
            {
                std::vector<int> sprites = tibia::animatedObjectsList.at(i);

                for (int n = 0; n < sprites.size(); n++)
                {
                    int sprite = sprites.at(n);

                    if (tileId == sprite)
                    {
                        n++;

                        if (n > sprites.size() - 1)
                        {
                            n = 0;
                        }

                        sprite = sprites.at(n);

                        updateTile(tileNumber, sprite);

                        break;
                    }
                }
            }
        }
    }

    sf::Vector2u getTileCoordsByTileNumber(int tileNumber)
    {
        int tileId = m_tiles.at(tileNumber);

        tileId = tileId - 1;

        sf::Vector2u tileCoords;
        tileCoords.x = tileId % (m_tileSet.getSize().x / tibia::TILE_SIZE);
        tileCoords.y = tileId / (m_tileSet.getSize().x / tibia::TILE_SIZE);

        return tileCoords;
    }

    std::string getName()
    {
        return m_name;
    }

    void setName(std::string name)
    {
        m_name = name;
    }

    std::string getType()
    {
        return m_type;
    }

    void setType(std::string type)
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
    std::string m_type;

    int m_z;

    int m_width;
    int m_height;

    std::vector<int> m_tiles;
    std::vector<int>::iterator m_tiles_it;

    std::vector<int> m_waterTiles;
    std::vector<int>::iterator m_waterTiles_it;

    sf::VertexArray m_vertices;

    sf::Texture m_tileSet;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = &m_tileSet;

        target.draw(m_vertices, states);
    }
};

}

#endif // TIBIA_TILEMAP_HPP

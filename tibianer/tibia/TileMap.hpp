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

    void load(std::vector<int> tiles)
    {
        m_tileSet = tibia::sprites;

        m_tiles = tiles;

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

    sf::Vector2u getTileCoordsByTileNumber(int tileNumber)
    {
        int tileId = m_tiles.at(tileNumber);

        tileId = tileId - 1;

        sf::Vector2u tileCoords;
        tileCoords.x = tileId % (m_tileSet.getSize().x / tibia::TILE_SIZE);
        tileCoords.y = tileId / (m_tileSet.getSize().x / tibia::TILE_SIZE);

        return tileCoords;
    }

    int getTileNumberByTileCoords(int x, int y)
    {
        int tileX = x;
        int tileY = y;

        tileX = tileX - (tileX % tibia::TILE_SIZE);
        tileY = tileY - (tileY % tibia::TILE_SIZE);

        return (tileX + tileY * tibia::MAP_SIZE) / tibia::TILE_SIZE;
    }

    void doAnimatedWater()
    {
        for (unsigned int tileNumber = 0; tileNumber < m_tiles.size(); tileNumber++)
        {
            for (int n = 0; n < sizeof(tibia::animatedTiles::water); n++)
            {
                if (m_tiles.at(tileNumber) == tibia::animatedTiles::water[n])
                {
                    int waterId = tibia::animatedTiles::water[n];

                    if (waterId == tibia::animatedTiles::water[3])
                    {
                        waterId = tibia::animatedTiles::water[0];
                    }
                    else if (waterId == tibia::animatedTiles::water[7])
                    {
                        waterId = tibia::animatedTiles::water[4];
                    }
                    else
                    {
                        waterId = waterId + 1;
                    }

                    updateTile(tileNumber, waterId);

                    break;
                }
            }
        }
    }

    void doAnimatedObjects()
    {
        //std::cout << "TileMap doAnimatedObjects()" << std::endl;

        for (unsigned int tileNumber = 0; tileNumber < m_tiles.size(); tileNumber++)
        {
            for (unsigned int i = 0; i < tibia::animatedObjectsList.size(); i++)
            {
                std::vector<int> sprites = tibia::animatedObjectsList.at(i);

                for (int n = 0; n < sprites.size(); n++)
                {
                    int sprite = sprites.at(n);

                    if (m_tiles.at(tileNumber) == sprite)
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

private:

    unsigned int m_width;
    unsigned int m_height;

    std::vector<int> m_tiles;
    std::vector<int>::iterator m_tiles_it;

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

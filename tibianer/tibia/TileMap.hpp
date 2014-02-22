#ifndef TIBIA_TILEMAP_HPP
#define TIBIA_TILEMAP_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Tile.hpp"

namespace tibia
{

class TileMap : public sf::Drawable, public sf::Transformable
{

public:

    typedef std::shared_ptr<tibia::Tile> tileSharedPtr;
    typedef std::vector<tileSharedPtr>::iterator tilesListIterator;

    struct sortByTileNumber
    {
        bool operator()(tileSharedPtr a, tileSharedPtr b) const
        {
            return (a.get()->getNumber() < b.get()->getNumber());
        }
    };

    void load(std::vector<int> tiles, int type)
    {
        m_texture = tibia::Textures::sprites;

        m_tiles = tiles;

        m_type = type;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(m_tiles.size() * 4);

        for (unsigned int i = 0; i < tibia::MAP_SIZE; ++i)
        {
            for (unsigned int j = 0; j < tibia::MAP_SIZE; ++j)
            {
                int tileNumber = i + j * tibia::MAP_SIZE;

                int tileId = m_tiles.at(tileNumber) - 1;

                int tu = tileId % (m_texture.getSize().x / tibia::TILE_SIZE);
                int tv = tileId / (m_texture.getSize().x / tibia::TILE_SIZE);

                tileId++;

                sf::Vertex* quad = &m_vertices[tileNumber * 4];

                quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                quad[0].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
                quad[3].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);

                sf::Vector2u tilePosition;
                tilePosition.x = quad[0].position.x;
                tilePosition.y = quad[0].position.y;

                // pink tile is transparent
                if (tileId == 1)
                {
                    quad[0].color = tibia::Colors::transparent;
                    quad[1].color = tibia::Colors::transparent;
                    quad[2].color = tibia::Colors::transparent;
                    quad[3].color = tibia::Colors::transparent;
                }

                int tileOffset = 0;

                bool tileIsOffset = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::offsetObjects) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::offsetObjects[k])
                    {
                        tileIsOffset = true;
                        break;
                    }
                }

                if (tileIsOffset == true)
                {
                    quad[0].position.x -= tibia::TILE_DRAW_OFFSET;
                    quad[1].position.x -= tibia::TILE_DRAW_OFFSET;
                    quad[2].position.x -= tibia::TILE_DRAW_OFFSET;
                    quad[3].position.x -= tibia::TILE_DRAW_OFFSET;

                    quad[0].position.y -= tibia::TILE_DRAW_OFFSET;
                    quad[1].position.y -= tibia::TILE_DRAW_OFFSET;
                    quad[2].position.y -= tibia::TILE_DRAW_OFFSET;
                    quad[3].position.y -= tibia::TILE_DRAW_OFFSET;

                    tileOffset = tibia::TILE_DRAW_OFFSET;
                }

                bool tileIsNull = false;

                if (tileId == tibia::TILE_NULL && m_type == tibia::TileMapTypes::tiles)
                {
                    tileIsNull = true;
                }

                bool tileIsSolid = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::solid) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::solid[k])
                    {
                        tileIsSolid = true;
                        break;
                    }
                }

                bool tileIsBlockProjectiles = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::blockProjectiles) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::blockProjectiles[k])
                    {
                        tileIsBlockProjectiles = true;
                        break;
                    }
                }

                bool tileIsWater = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::water) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::water[k])
                    {
                        tileIsWater = true;
                        break;
                    }
                }

                bool tileIsLava = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::lava) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::lava[k])
                    {
                        tileIsLava = true;
                        break;
                    }
                }

                bool tileIsChair = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::chairs) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::chairs[k])
                    {
                        tileIsChair = true;
                        break;
                    }
                }

                bool tileIsLadder = false;

                if (tileId == tibia::SpriteData::ladder)
                {
                    tileIsLadder = true;
                }

                bool tileIsMoveAbove = false;

                if (tileId == tibia::SpriteData::stairs)
                {
                    tileIsMoveAbove = true;
                }

                bool tileIsMoveBelow = false;

                for (int k = 0; k < (sizeof(tibia::SpriteData::holes) / sizeof(int)); k++)
                {
                    if (tileId == tibia::SpriteData::holes[k])
                    {
                        tileIsMoveBelow = true;
                        break;
                    }
                }

                tileSharedPtr tile(new tibia::Tile);
                tile->setNumber(tileNumber);
                tile->setId(tileId);
                tile->setOffset(tileOffset);
                tile->setPosition(tilePosition);
                tile->setIsNull(tileIsNull);
                tile->setIsSolid(tileIsSolid);
                tile->setIsBlockProjectiles(tileIsBlockProjectiles);
                tile->setIsWater(tileIsWater);
                tile->setIsLava(tileIsLava);
                tile->setIsChair(tileIsChair);
                tile->setIsOffset(tileIsOffset);
                tile->setIsLadder(tileIsLadder);
                tile->setIsMoveAbove(tileIsMoveAbove);
                tile->setIsMoveBelow(tileIsMoveBelow);
                m_tilesList.push_back(tile);

                if (tileId != tibia::TILE_NULL)
                {
                    m_verticesList.insert(m_verticesList.end(), quad, quad + 4);
                }
            }
        }

        std::sort(m_tilesList.begin(), m_tilesList.end(), sortByTileNumber());

        loadWaterTiles();
    }

    void updateTile(int tileNumber, int tileId)
    {
        m_tiles.at(tileNumber) = tileId;

        m_tilesList.at(tileNumber).get()->setId(tileId);

        int currentTileId = m_tiles.at(tileNumber) - 1;

        int tu = currentTileId % (m_texture.getSize().x / tibia::TILE_SIZE);
        int tv = currentTileId / (m_texture.getSize().x / tibia::TILE_SIZE);

        //if (m_type == tibia::TileMapTypes::tiles)
        //{
            //sf::Vertex* quad = &m_vertices[tileNumber * 4];

            //quad[0].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
            //quad[1].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
            //quad[2].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
            //quad[3].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
        //}
        //else
        //{
            int vertexIndex = 0;

            std::vector<sf::Vertex>::iterator verticesList_it;
            for (verticesList_it = m_verticesList.begin(); verticesList_it != m_verticesList.end(); verticesList_it++)
            {
                if (vertexIndex == 4)
                {
                    vertexIndex = 0;
                }

                if (vertexIndex == 0)
                {
                    sf::Vertex* vertex0 = &(*verticesList_it);

                    int vertexTileNumber = getTileNumberByTileCoords(vertex0->position);

                    if (vertexTileNumber == tileNumber)
                    {
                        auto verticesList_it1 = std::next(verticesList_it, 1);
                        sf::Vertex* vertex1 = &(*verticesList_it1);

                        auto verticesList_it2 = std::next(verticesList_it, 2);
                        sf::Vertex* vertex2 = &(*verticesList_it2);

                        auto verticesList_it3 = std::next(verticesList_it, 3);
                        sf::Vertex* vertex3 = &(*verticesList_it3);

                        vertex0->texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                        vertex1->texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                        vertex2->texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
                        vertex3->texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
                    }
                }

                vertexIndex++;
            }
        //}
    }

    std::vector<int>* getTiles()
    {
        return &m_tiles;
    }

    std::vector<int>* getWaterTiles()
    {
        return &m_waterTiles;
    }

    std::vector<tileSharedPtr> *getTilesList()
    {
        return &m_tilesList;
    }

    void loadWaterTiles()
    {
        tilesListIterator tilesList_it;
        for (tilesList_it = m_tilesList.begin(); tilesList_it != m_tilesList.end(); tilesList_it++)
        {
            tibia::Tile* tile = tilesList_it->get();

            if (tile->isWater() == true)
            {
                m_waterTiles.push_back(tile->getNumber());
            }
        }
    }

    void doAnimatedWater()
    {
        for (m_waterTiles_it = m_waterTiles.begin(); m_waterTiles_it != m_waterTiles.end(); m_waterTiles_it++)
        {
            int tileNumber = *m_waterTiles_it;

            int tileId = m_tiles.at(tileNumber);
            //int tileId = m_tilesList.at(tileNumber).get()->getId();

            //std::cout << "tileId: " << tileId << std::endl;

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
        for (unsigned int i = 0; i < m_tiles.size(); i++)
        {
            int tileNumber = i;

            int tileId = m_tiles.at(tileNumber);
            //int tileId = m_tilesList.at(tileNumber).get()->getId();

            if (tileId == tibia::TILE_NULL)
            {
                continue;
            }

            for (unsigned int j = 0; j < tibia::animatedObjectsList.size(); j++)
            {
                std::vector<int> sprites = tibia::animatedObjectsList.at(j);

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

    std::vector<sf::Vertex> getMiniMapTiles(std::vector<sf::Vertex> &verticesList)
    {
        for (unsigned int i = 0; i < tibia::MAP_SIZE; ++i)
        {
            for (unsigned int j = 0; j < tibia::MAP_SIZE; ++j)
            {
                int tileNumber = i + j * tibia::MAP_SIZE;

                if (m_tilesList.size() == 0 || m_tilesList.size() < tileNumber)
                {
                    continue;
                }

                tibia::Tile* tile = m_tilesList.at(tileNumber).get();

                if (tile->isSolid() || tile->isLadder() || tile->isMoveAbove() || tile->isMoveBelow())
                {
                    sf::Vertex quad[4];

                    quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                    quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                    quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                    quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                    sf::Color tileColor;

                    if (tile->isSolid() == true)
                    {
                        tileColor = tibia::Colors::tileIsSolid;
                    }

                    if (tile->isLadder() || tile->isMoveAbove() || tile->isMoveBelow())
                    {
                        tileColor = tibia::Colors::tileIsMoveAboveOrBelow;
                    }

                    quad[0].color = tileColor;
                    quad[1].color = tileColor;
                    quad[2].color = tileColor;
                    quad[3].color = tileColor;

                    verticesList.push_back(quad[0]);
                    verticesList.push_back(quad[1]);
                    verticesList.push_back(quad[2]);
                    verticesList.push_back(quad[3]);
                }
            }
        }

        return verticesList;
    }

    sf::Vector2u getTileCoordsByTileNumber(int tileNumber)
    {
        int tileId = m_tiles.at(tileNumber);

        tileId = tileId - 1;

        sf::Vector2u tileCoords;
        tileCoords.x = tileId % (m_texture.getSize().x / tibia::TILE_SIZE);
        tileCoords.y = tileId / (m_texture.getSize().x / tibia::TILE_SIZE);

        return tileCoords;
    }

    template <class T>
    int getTileNumberByTileCoords(T tileCoords)
    {
        int x = tileCoords.x / tibia::TILE_SIZE;
        int y = tileCoords.y / tibia::TILE_SIZE;

        return x + (y * tibia::MAP_SIZE);
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
    std::vector<int>::iterator m_tiles_it;

    std::vector<int> m_waterTiles;
    std::vector<int>::iterator m_waterTiles_it;

    std::vector<tileSharedPtr> m_tilesList;

    sf::VertexArray m_vertices;
    std::vector<sf::Vertex> m_verticesList;

    sf::Texture m_texture;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = &m_texture;

        //if (m_type == tibia::TileMapTypes::tiles)
        //{
            //target.draw(m_vertices, states);
        //}
        //else
        //{
            target.draw(&m_verticesList[0], m_verticesList.size(), sf::Quads, states);
        //}
    }
};

}

#endif // TIBIA_TILEMAP_HPP

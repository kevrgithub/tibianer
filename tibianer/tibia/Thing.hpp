#ifndef TIBIA_THING_HPP
#define TIBIA_THING_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/DrawableAndTransformable.hpp"

namespace tibia
{

class Thing : public tibia::DrawableAndTransformable
{

public:

    struct sortByTileNumber
    {
        bool operator()(Thing* a, Thing* b) const
        {
            return (a->getTileNumber() < b->getTileNumber());
        }
    };

    struct sortByTileCoords
    {
        bool operator()(Thing* a, Thing* b) const
        {
            return (a->getTileX() == b->getTileX() ? a->getTileY() < b->getTileY() : a->getTileX() < b->getTileX());
        }
    };

    void updateTileNumber()
    {
        m_tileNumber = tibia::getTileNumberByTileCoords(m_tileX, m_tileY);
    }

    void updateBox()
    {
        m_box.left = m_tileX;
        m_box.top  = m_tileY;

        m_box.width  = tibia::TILE_SIZE;
        m_box.height = tibia::TILE_SIZE;
    }

    void setCoords(int x, int y)
    {
        setX(x);
        setY(y);

        setTileX(x * tibia::TILE_SIZE);
        setTileY(y * tibia::TILE_SIZE);
    }

    void setTileCoords(int tileX, int tileY)
    {
        setX(tileX / tibia::TILE_SIZE);
        setY(tileY / tibia::TILE_SIZE);

        setTileX(tileX);
        setTileY(tileY);
    }

    void updateTileCoords()
    {
        m_tileX = m_x * tibia::TILE_SIZE;
        m_tileY = m_y * tibia::TILE_SIZE;

        updateTileNumber();

        updateBox();
    }

    int getTileX()
    {
        return m_tileX;
    }

    void setTileX(int x)
    {
        m_tileX = x;
    }

    int getTileY()
    {
        return m_tileY;
    }

    void setTileY(int y)
    {
        m_tileY = y;
    }

    sf::Vector2u getTilePosition()
    {
        //sf::Vector2u position;

        //position.x = m_tileX;
        //position.y = m_tileY;

        //return position;

        return sf::Vector2u(m_tileX, m_tileY);
    }

    int getTileNumber()
    {
        return m_tileNumber;
    }

    void setTileNumber(int tileNumber)
    {
        m_tileNumber = tileNumber;
    }

    int getX()
    {
        return m_x;
    }

    void setX(int x)
    {
        m_x = x;
    }

    int getY()
    {
        return m_y;
    }

    void setY(int y)
    {
        m_y = y;
    }

    void setZ(int z)
    {
        m_z = z;
    }

    int getZ()
    {
        return m_z;
    }

    sf::FloatRect getBox()
    {
        return m_box;
    }

private:

    int m_tileX;
    int m_tileY;

    int m_tileNumber;

    int m_x;
    int m_y;
    int m_z;

    sf::FloatRect m_box;

};

}

#endif // TIBIA_THING_HPP

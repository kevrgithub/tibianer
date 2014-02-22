#ifndef TIBIA_TILE_HPP
#define TIBIA_TILE_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"

namespace tibia
{

class Tile
{

public:

    int getNumber()
    {
        return m_number;
    }

    void setNumber(int number)
    {
        m_number = number;
    }

    int getId()
    {
        return m_id;
    }

    void setId(int id)
    {
        m_id = id;
    }

    int getOffset()
    {
        return m_offset;
    }

    void setOffset(int offset)
    {
        m_offset = offset;
    }

    sf::Vector2u getPosition()
    {
        return m_position;
    }

    void setPosition(sf::Vector2u position)
    {
        m_position = position;
    }

    bool isNull()
    {
        return m_isNull;
    }

    void setIsNull(bool b)
    {
        m_isNull = b;
    }

    bool isSolid()
    {
        return m_isSolid;
    }

    void setIsSolid(bool b)
    {
        m_isSolid = b;
    }

    bool isBlockProjectiles()
    {
        return m_isBlockProjectiles;
    }

    void setIsBlockProjectiles(bool b)
    {
        m_isBlockProjectiles = b;
    }

    bool isWater()
    {
        return m_isWater;
    }

    void setIsWater(bool b)
    {
        m_isWater = b;
    }

    bool isLava()
    {
        return m_isLava;
    }

    void setIsLava(bool b)
    {
        m_isLava = b;
    }

    bool isChair()
    {
        return m_isChair;
    }

    void setIsChair(bool b)
    {
        m_isChair = b;
    }

    bool isOffset()
    {
        return m_isOffset;
    }

    void setIsOffset(bool b)
    {
        m_isOffset = b;
    }

    bool isLadder()
    {
        return m_isLadder;
    }

    void setIsLadder(bool b)
    {
        m_isLadder = b;
    }

    bool isMoveAbove()
    {
        return m_isMoveAbove;
    }

    void setIsMoveAbove(bool b)
    {
        m_isMoveAbove = b;
    }

    bool isMoveBelow()
    {
        return m_isMoveBelow;
    }

    void setIsMoveBelow(bool b)
    {
        m_isMoveBelow = b;
    }

private:

    int m_number;
    int m_id;

    int m_offset;

    sf::Vector2u m_position;

    bool m_isNull;
    bool m_isSolid;
    bool m_isBlockProjectiles;
    bool m_isWater;
    bool m_isLava;
    bool m_isChair;
    bool m_isOffset;
    bool m_isLadder;
    bool m_isMoveAbove;
    bool m_isMoveBelow;
};

}

#endif // TIBIA_TILE_HPP

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

    int getFlags()
    {
        return m_flags;
    }

    void setFlags(int flags)
    {
        m_flags = flags;
    }

private:

    int m_number;
    int m_id;

    int m_offset;

    sf::Vector2u m_position;

    int m_flags;
};

}

#endif // TIBIA_TILE_HPP

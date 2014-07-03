#ifndef TIBIA_TILE_HPP
#define TIBIA_TILE_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Object.hpp"
#include "tibia/Creature.hpp"

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

    sf::Vector2u getPosition()
    {
        return m_position;
    }

    void setPosition(sf::Vector2u position)
    {
        m_position = position;
    }

    int getZ()
    {
        return m_z;
    }

    void setZ(int z)
    {
        m_z = z;
    }

    int getFlags()
    {
        return m_flags;
    }

    void setFlags(int flags)
    {
        m_flags = flags;
    }

    tibia::ObjectList* getObjectList()
    {
        return &m_objectList;
    }

    tibia::CreatureList* getCreatureList()
    {
        return &m_creatureList;
    }

    void addObject(tibia::ObjectPtr object)
    {
        m_objectList.push_back(object);
    }

    void addCreature(tibia::CreaturePtr creature)
    {
        m_creatureList.push_back(creature);
    }

private:

    int m_number;
    int m_id;

    sf::Vector2u m_position;

    int m_z;

    int m_flags;

    tibia::ObjectList m_objectList;
    tibia::CreatureList m_creatureList;
};

typedef std::shared_ptr<tibia::Tile> TilePtr;
typedef std::vector<TilePtr> TileList;

namespace TileSort
{
    struct sortTilesByTileNumber
    {
        bool operator()(tibia::TilePtr a, tibia::TilePtr b) const
        {
            return (a->getNumber() < b->getNumber());
        }
    };
}

} // tibia

#endif // TIBIA_TILE_HPP

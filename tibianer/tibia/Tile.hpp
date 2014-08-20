#ifndef TIBIA_TILE_HPP
#define TIBIA_TILE_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Object.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Animation.hpp"

namespace tibia
{

class Tile
{

public:

    typedef std::shared_ptr<tibia::Tile> Ptr;
    typedef std::vector<tibia::Tile::Ptr> List;

    struct Properties_t
    {
        bool isSolid     = false;
        bool isMoveAbove = false;
        bool isMoveBelow = false;

        bool hasSolidObject             = false;
        bool hasSolidCreature           = false;
        bool hasBlockProjectilesObject  = false;
        bool hasHasHeightObject         = false;
        bool hasMoveAboveObject         = false;
        bool hasMoveBelowObject         = false;
        bool hasModifyHpOnTouchObject   = false;
        bool hasTeleporterObject        = false;
        bool hasMountainRampLeftObject  = false;
        bool hasMountainRampRightObject = false;
    };

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

    int getHeight()
    {
        return m_height;
    }

    void setHeight(int height)
    {
        m_height = height;
    }

    unsigned int getFlags()
    {
        return m_flags;
    }

    void setFlags(unsigned int flags)
    {
        m_flags = flags;
    }

    tibia::Object::List* getObjectList()
    {
        return &m_objectList;
    }

    tibia::Creature::List* getCreatureList()
    {
        return &m_creatureList;
    }

    tibia::Animation::List* getAnimationList()
    {
        return &m_animationList;
    }

    void addObject(tibia::Object::Ptr object)
    {
        m_objectList.push_back(object);
    }

    void addCreature(tibia::Creature::Ptr creature)
    {
        m_creatureList.push_back(creature);
    }

    void addAnimation(tibia::Animation::Ptr animation)
    {
        m_animationList.push_back(animation);
    }

private:

    int m_number;
    int m_id;

    sf::Vector2u m_position;

    int m_height;

    int m_z;

    unsigned int m_flags;

    tibia::Object::List m_objectList;
    tibia::Creature::List m_creatureList;
    tibia::Animation::List m_animationList;

}; // class Tile

namespace TileSort
{
    struct sortTilesByTileNumber
    {
        bool operator()(tibia::Tile::Ptr a, tibia::Tile::Ptr b) const
        {
            return (a->getNumber() < b->getNumber());
        }
    };
}

} // namespace tibia

#endif // TIBIA_TILE_HPP

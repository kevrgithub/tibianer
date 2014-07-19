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

    struct TileProperties_t
    {
        bool isSolid     = false;
        bool isMoveAbove = false;
        bool isMoveBelow = false;
        bool isMountainRampLeftMoveBelow  = false;
        bool isMountainRampRightMoveBelow = false;

        bool hasSolidObject      = false;
        bool hasSolidCreature    = false;
        bool hasHasHeightObject  = false;
        bool hasMoveAboveObject  = false;
        bool hasMoveBelowObject  = false;
        bool hasTeleporterObject = false;
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

    tibia::ObjectList* getObjectList()
    {
        return &m_objectList;
    }

    tibia::CreatureList* getCreatureList()
    {
        return &m_creatureList;
    }

    tibia::AnimationList* getAnimationList()
    {
        return &m_animationList;
    }

    void addObject(tibia::ObjectPtr object)
    {
        m_objectList.push_back(object);
    }

    void addCreature(tibia::CreaturePtr creature)
    {
        m_creatureList.push_back(creature);
    }

    void addAnimation(tibia::AnimationPtr animation)
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

    tibia::ObjectList m_objectList;
    tibia::CreatureList m_creatureList;
    tibia::AnimationList m_animationList;
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

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
        bool hasBearTrapOpenObject      = false;
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

    tibia::SpriteFlags_t getFlags()
    {
        return m_flags;
    }

    void setFlags(tibia::SpriteFlags_t flags)
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

    tibia::Object::List* getEntityList()
    {
        return &m_entityList;
    }

    tibia::Animation::List* getAnimationList()
    {
        return &m_animationList;
    }

    void addObject(tibia::Object::Ptr object)
    {
        if (object->getFlags().test(tibia::SpriteFlags::decal))
        {
            m_objectList.insert(m_objectList.begin(), object);
        }
        else
        {
            m_objectList.push_back(object);
        }
    }

    void removeObject(tibia::Object::Ptr object)
    {
        auto objectIt = std::find(m_objectList.begin(), m_objectList.end(), object);

        if (objectIt != m_objectList.end())
        {
            m_objectList.erase(objectIt);
        }
    }

    void addCreature(tibia::Creature::Ptr creature)
    {
        if (creature->isDead() == true)
        {
            m_creatureList.insert(m_creatureList.begin(), creature);
        }
        else
        {
            m_creatureList.push_back(creature);
        }
    }

    void addEntity(tibia::Object::Ptr entity)
    {
        m_entityList.push_back(entity);
    }

    void addAnimation(tibia::Animation::Ptr animation)
    {
        m_animationList.push_back(animation);
    }

    Properties_t getProperties()
    {
        Properties_t tileProperties;

        if (m_flags.test(tibia::SpriteFlags::solid))
        {
            tileProperties.isSolid = true;
        }

        if (m_flags.test(tibia::SpriteFlags::moveAbove))
        {
            tileProperties.isMoveAbove = true;
        }

        if (m_flags.test(tibia::SpriteFlags::moveBelow))
        {
            tileProperties.isMoveBelow = true;
        }

        tibia::Object::List* objectList = this->getObjectList();

        for (auto& object : *objectList)
        {
            tibia::SpriteFlags_t objectFlags = object->getFlags();

            if (objectFlags.test(tibia::SpriteFlags::solid))
            {
                tileProperties.hasSolidObject = true;
            }

            if (objectFlags.test(tibia::SpriteFlags::blockProjectiles))
            {
                tileProperties.hasBlockProjectilesObject = true;
            }

            if (objectFlags.test(tibia::SpriteFlags::hasHeight))
            {
                tileProperties.hasHasHeightObject = true;
            }

            if (objectFlags.test(tibia::SpriteFlags::moveAbove))
            {
                tileProperties.hasMoveAboveObject = true;
            }

            if (objectFlags.test(tibia::SpriteFlags::moveBelow))
            {
                tileProperties.hasMoveBelowObject = true;
            }

            if (objectFlags.test(tibia::SpriteFlags::modifyHpOnTouch))
            {
                tileProperties.hasModifyHpOnTouchObject = true;
            }

            int objectId = object->getId();

            if (objectId == tibia::SpriteData::mountainRampLeft)
            {
                tileProperties.hasMountainRampLeftObject = true;
            }
            else if (objectId == tibia::SpriteData::mountainRampRight)
            {
                tileProperties.hasMountainRampRightObject = true;
            }
            else if (objectId == tibia::SpriteData::bearTrap[1])
            {
                tileProperties.hasBearTrapOpenObject = true;
            }

            if (object->getType() == tibia::ObjectTypes::teleporter)
            {
                tileProperties.hasTeleporterObject = true;
            }
        }

        tibia::Creature::List* creatureList = this->getCreatureList();

        for (auto& creature : *creatureList)
        {
            if (creature->isDead() == false && creature->isSleeping() == false)
            {
                tileProperties.hasSolidCreature = true;
                break; // break for now since only checking for one thing with creatures
            }
        }

        return tileProperties;
    }

private:

    int m_number;
    int m_id;

    sf::Vector2u m_position;

    int m_height;

    int m_z;

    tibia::SpriteFlags_t m_flags;

    tibia::Object::List m_objectList;
    tibia::Creature::List m_creatureList;
    tibia::Object::List m_entityList;
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

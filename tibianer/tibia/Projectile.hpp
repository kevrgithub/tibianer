#ifndef TIBIA_PROJECTILE_HPP
#define TIBIA_PROJECTILE_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"
#include "tibia/Creature.hpp"

namespace tibia
{

class Projectile : public tibia::Thing
{

public:

    typedef std::shared_ptr<tibia::Projectile> Ptr;
    typedef std::vector<tibia::Projectile::Ptr> List;

    Projectile::Projectile(int type, int direction, sf::Vector2f origin, sf::Vector2f destination, bool isPrecise = false, bool isChild = false)
    {
        m_type = type;

        m_modifyHpType = tibia::ModifyHpTypes::blood;

        m_direction = direction;

        m_isPrecise = isPrecise;
        m_isChild   = isChild;

        m_range  = tibia::ProjectileRanges::default;
        m_damage = tibia::ProjectileDamages::default;
        m_speed  = tibia::ProjectileSpeeds::default;

        m_distanceTravelled     = 0;
        m_tileDistanceTravelled = 0;

        m_spriteTileX = origin.x;
        m_spriteTileY = origin.y;

        m_vectorOrigin      = origin;
        m_vectorDestination = destination;

        m_vectorMovement = tibia::Utility::getNormalByVectors(origin, destination);

        if (m_isPrecise == true)
        {
            m_direction = tibia::Utility::getDirectionByVector(m_vectorMovement);
        }

        setPropertiesByType(m_type);

        setId(m_id);

        m_sprite.setId(m_id);

        setPosition(origin.x, origin.y);
    }

    void setPropertiesByType(int type)
    {
        switch (type)
        {
            case tibia::ProjectileTypes::spellBlue:
                m_id           = tibia::Projectiles::spellBlue;
                m_modifyHpType = tibia::ModifyHpTypes::spellBlue;
                break;

            case tibia::ProjectileTypes::spellBlack:
                m_id           = tibia::Projectiles::spellBlack;
                m_modifyHpType = tibia::ModifyHpTypes::spellBlack;
                break;

            case tibia::ProjectileTypes::spellCacodemon:
                m_id           = tibia::Projectiles::spellCacodemon;
                m_modifyHpType = tibia::ModifyHpTypes::electricity;
                break;

            case tibia::ProjectileTypes::spellFire:
                m_id           = tibia::Projectiles::spellFire[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::fire;
                break;

            case tibia::ProjectileTypes::throwingKnife:
                m_id           = tibia::Projectiles::throwingKnife[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::spear:
                m_id           = tibia::Projectiles::spear[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::bolt:
                m_id          = tibia::Projectiles::bolt[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::powerBolt:
                m_id           = tibia::Projectiles::powerBolt[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::arrow:
                m_id           = tibia::Projectiles::arrow[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::burstArrow:
                m_id           = tibia::Projectiles::burstArrow[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::poisonArrow:
                m_id           = tibia::Projectiles::poisonArrow[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;
        }
    }

    void doMovement(sf::Time timeDelta)
    {
        float moveX = m_vectorMovement.x;
        float moveY = m_vectorMovement.y;

        //std::cout << "timeDelta: " << timeDelta.asSeconds() << std::endl;

        float moveSpeed = m_speed * timeDelta.asSeconds();

        moveSpeed = std::roundf(moveSpeed);

        //std::cout << "moveSpeed: " << moveSpeed << std::endl;

        m_sprite.move(moveX * moveSpeed, moveY * moveSpeed);

        m_distanceTravelled += moveSpeed;

        //std::cout << "m_distanceTravelled: " << m_distanceTravelled << std::endl;

        m_distanceTravelledRoundedToTileSize = m_distanceTravelled - (m_distanceTravelled % tibia::TILE_SIZE);

        if (m_distanceTravelledRoundedToTileSize == m_distanceTravelledPrevious)
        {
            return;
        }

        m_distanceTravelledPrevious = m_distanceTravelledRoundedToTileSize;

        //std::cout << "m_distanceTravelledRoundedToTileSize: " << m_distanceTravelledRoundedToTileSize << std::endl;

        if (m_distanceTravelledRoundedToTileSize > 0 && m_distanceTravelledRoundedToTileSize % tibia::TILE_SIZE == 0)
        {
            m_spriteTileX = m_spriteTileX + (moveX * tibia::TILE_SIZE);
            m_spriteTileY = m_spriteTileY + (moveY * tibia::TILE_SIZE);

            m_tileDistanceTravelled += 1;

            //std::cout << "m_tileDistanceTravelled: " << m_tileDistanceTravelled << std::endl;
        }
    }

    void update(sf::Time timeDelta)
    {
        //updateTileCoords();

        //int drawOffset = getDrawOffset() * tibia::THING_DRAW_OFFSET;

        //setPosition(getTileX() - drawOffset, getTileY() - drawOffset);

        doMovement(timeDelta);

        setTileCoords(getSpriteTilePosition().x, getSpriteTilePosition().y);

        updateTileNumber();
    }

    void setId(int id)
    {
        m_id = id;

        m_flags = tibia::UMaps::spriteFlags[m_id];
    }

    int getId()
    {
        return m_id;
    }

    void setType(int type)
    {
        m_type = type;
    }

    int getType()
    {
        return m_type;
    }

    int getModifyHpType()
    {
        return m_modifyHpType;
    }

    tibia::SpriteFlags_t getFlags()
    {
        return m_flags;
    }

    void setFlags(tibia::SpriteFlags_t flags)
    {
        m_flags = flags;
    }

    void setRange(int range)
    {
        m_range = range;
    }

    int getRange()
    {
        return m_range;
    }

    void setDamage(int damage)
    {
        m_damage = damage;
    }

    int getDamage()
    {
        return m_damage;
    }

    float setSpeed(float speed)
    {
        m_speed = speed;
    }

    float getSpeed()
    {
        return m_speed;
    }

    int getDirection()
    {
        return m_direction;
    }

    void setDistanceTravelled(int distance)
    {
        m_distanceTravelled = distance;
    }

    int getDistanceTravelled()
    {
        return m_distanceTravelled;
    }

    int getDistanceTravelledRoundedToTileSize()
    {
        return m_distanceTravelledRoundedToTileSize;
    }

    int getDistanceTravelledPrevious()
    {
        return m_distanceTravelledPrevious;
    }

    int getTileDistanceTravelled()
    {
        return m_tileDistanceTravelled;
    }

    void setTileDistanceTravelledPrevious(int distance)
    {
        m_tileDistanceTravelledPrevious = distance;
    }

    int getTileDistanceTravelledPrevious()
    {
        return m_tileDistanceTravelledPrevious;
    }

    sf::Vector2f getVectorOrigin()
    {
        return m_vectorOrigin;
    }

    sf::Vector2f getVectorDestination()
    {
        return m_vectorDestination;
    }

    sf::Vector2f getVectorMovement()
    {
        return m_vectorMovement;
    }

    sf::Sprite* getSprite()
    {
        return &m_sprite;
    }

    sf::Vector2f getSpritePosition()
    {
        return m_sprite.getPosition();
    }

    sf::Vector2u getSpriteTilePosition()
    {
        float integralX;
        float fractionalX = std::modf(m_spriteTileX, &integralX);

        float integralY;
        float fractionalY = std::modf(m_spriteTileY, &integralY);

        int x = integralX;
        int y = integralY;

        if (m_vectorMovement.x < 0 && m_vectorMovement.y > 0)
        {
            x += tibia::TILE_SIZE;
        }
        else if (m_vectorMovement.x > 0 && m_vectorMovement.y < 0)
        {
            y += tibia::TILE_SIZE;
        }
        else if (m_vectorMovement.x < 0 && m_vectorMovement.y < 0)
        {
            x += tibia::TILE_SIZE;
            y += tibia::TILE_SIZE;
        }

        // round down to nearest multiple of tile size
        x = x - (x % tibia::TILE_SIZE);
        y = y - (y % tibia::TILE_SIZE);

        return sf::Vector2u(x, y);
    }

    tibia::Creature::Ptr getCreatureOwner()
    {
        return m_creatureOwner;
    }

    void setCreatureOwner(tibia::Creature::Ptr creature)
    {
        m_creatureOwner = creature;
    }

    bool isPrecise()
    {
        return m_isPrecise;
    }

    void setIsPrecise(bool b)
    {
        m_isPrecise = b;
    }

    bool isChild()
    {
        return m_isChild;
    }

    void setIsChild(bool b)
    {
        m_isChild = b;
    }

private:

    int m_id;

    int m_type;
    int m_modifyHpType;

    tibia::SpriteFlags_t m_flags;

    float m_spriteTileX;
    float m_spriteTileY;

    int m_direction;
    int m_range;
    int m_damage;
    float m_speed;

    int m_distanceTravelled;
    int m_distanceTravelledRoundedToTileSize;
    int m_distanceTravelledPrevious;

    int m_tileDistanceTravelled;
    int m_tileDistanceTravelledPrevious;

    bool m_isPrecise;
    bool m_isChild;

    sf::Vector2f m_vectorOrigin;
    sf::Vector2f m_vectorDestination;
    sf::Vector2f m_vectorMovement;

    tibia::Sprite m_sprite;

    tibia::Creature::Ptr m_creatureOwner;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite, states);
    }

};

} // namespace Tibia

#endif // TIBIA_PROJECTILE_HPP

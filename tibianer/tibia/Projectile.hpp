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

            case tibia::ProjectileTypes::spellFire:
                m_id           = tibia::Projectiles::spellFire[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::fire;
                break;

            case tibia::ProjectileTypes::spear:
                m_id           = tibia::Projectiles::spear[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::bolt:
                m_id          = tibia::Projectiles::bolt[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::arrow:
                m_id           = tibia::Projectiles::arrow[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::arrowFire:
                m_id           = tibia::Projectiles::arrowFire[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;

            case tibia::ProjectileTypes::arrowPoison:
                m_id           = tibia::Projectiles::arrowPoison[m_direction];
                m_modifyHpType = tibia::ModifyHpTypes::blood;
                break;
        }
    }

    void doMovement(sf::Time timeDelta)
    {
        sf::Time timeElapsed = m_clock.getElapsedTime();

        if (timeElapsed.asSeconds() >= 0.01)
        {
            float moveX = m_vectorMovement.x;
            float moveY = m_vectorMovement.y;

            m_sprite.move(moveX * m_speed, moveY * m_speed); // * timeDelta.asSeconds()

            m_distanceTravelled += m_speed;

            if (m_distanceTravelled > 0 && m_distanceTravelled % tibia::TILE_SIZE == 0)
            {
                m_spriteTileX = m_spriteTileX + (moveX * tibia::TILE_SIZE);
                m_spriteTileY = m_spriteTileY + (moveY * tibia::TILE_SIZE);

                m_tileDistanceTravelled += 1;
            }

            m_clock.restart();
        }
    }

    void update(sf::Time timeDelta)
    {
        //updateTileCoords();

        //setPosition(getTileX(), getTileY());

        doMovement(timeDelta);

        setTileCoords(getSpriteTilePosition().x, getSpriteTilePosition().y);

        updateTileNumber();
    }

    void setId(int id)
    {
        m_id = id;
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

    int setDistanceTravelled(int distanceTravelled)
    {
        m_distanceTravelled = distanceTravelled;
    }

    int getDistanceTravelled()
    {
        return m_distanceTravelled;
    }

    int getTileDistanceTravelled()
    {
        return m_tileDistanceTravelled;
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

    sf::Vector2f getSpriteTilePosition()
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

        return sf::Vector2f(x, y);
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

    float m_spriteTileX;
    float m_spriteTileY;

    int m_direction;
    int m_range;
    int m_damage;
    float m_speed;

    int m_distanceTravelled;
    int m_tileDistanceTravelled;

    bool m_isPrecise;
    bool m_isChild;

    sf::Vector2f m_vectorOrigin;
    sf::Vector2f m_vectorDestination;
    sf::Vector2f m_vectorMovement;

    tibia::Sprite m_sprite;

    sf::Clock m_clock;

    tibia::Creature::Ptr m_creatureOwner;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite, states);
    }

};

} // namespace Tibia

#endif // TIBIA_PROJECTILE_HPP

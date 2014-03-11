#ifndef TIBIA_PROJECTILE_HPP
#define TIBIA_PROJECTILE_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"
#include "tibia/Creature.hpp"

namespace tibia
{

class Projectile : public tibia::Thing
{

public:

    Projectile::Projectile(int type, int direction, sf::Vector2f origin, sf::Vector2f destination, bool isPrecise = false, bool isChild = false)
    {
        m_type = type;

        m_direction = direction;

        m_isPrecise = isPrecise;

        m_isChild = isChild;

        m_range = tibia::ProjectileRanges::default;

        m_damage = tibia::ProjectileDamages::default;

        m_speed = tibia::ProjectileSpeeds::default;

        m_distanceTravelled = 0;

        m_tileDistanceTravelled = 0;

        m_spriteTileX = origin.x;
        m_spriteTileY = origin.y;

        m_vectorOrigin      = origin;
        m_vectorDestination = destination;

        m_vectorMovement = tibia::getNormalByVectors(origin, destination);

        if (m_isPrecise == true)
        {
            m_direction = tibia::getDirectionByVector(m_vectorMovement);
        }

        setPropertiesByType(m_type);

        m_sprite.setId(m_id);

        setPosition(origin.x, origin.y);
    }

    Projectile::~Projectile()
    {
        //std::cout << "~Projectile()" << std::endl;
    }

    void setPropertiesByType(int type)
    {
        switch (type)
        {
            case tibia::ProjectileTypes::spellBlue:
                m_id                 = tibia::Projectiles::spellBlue;
                m_animationOnHit     = tibia::Animations::spellBlue;
                m_animationOnBlock   = tibia::Animations::spellBlue;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::spellBlack:
                m_id                 = tibia::Projectiles::spellBlack;
                m_animationOnHit     = tibia::Animations::spellBlack;
                m_animationOnBlock   = tibia::Animations::spellBlack;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::spellFire:
                m_id                 = tibia::Projectiles::spellFire[m_direction];
                m_animationOnHit     = tibia::Animations::fire;
                m_animationOnBlock   = tibia::Animations::fire;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::spear:
                m_id                 = tibia::Projectiles::spear[m_direction];
                m_animationOnHit     = tibia::Animations::hitBlood;
                m_animationOnBlock   = tibia::Animations::hitBlock;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::bolt:
                m_id                 = tibia::Projectiles::bolt[m_direction];
                m_animationOnHit     = tibia::Animations::hitBlood;
                m_animationOnBlock   = tibia::Animations::hitBlock;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::arrow:
                m_id                 = tibia::Projectiles::arrow[m_direction];
                m_animationOnHit     = tibia::Animations::hitBlood;
                m_animationOnBlock   = tibia::Animations::hitBlock;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::arrowFire:
                m_id                 = tibia::Projectiles::arrowFire[m_direction];
                m_animationOnHit     = tibia::Animations::fire;
                m_animationOnBlock   = tibia::Animations::hitBlock;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatRed;
                break;

            case tibia::ProjectileTypes::arrowPoison:
                m_id                 = tibia::Projectiles::arrowPoison[m_direction];
                m_animationOnHit     = tibia::Animations::hitPoison;
                m_animationOnBlock   = tibia::Animations::hitBlock;
                m_animatedDecalOnHit = tibia::AnimatedDecals::splatGreen;
                break;
        }
    }

    void doMovement()
    {
        sf::Time timeElapsed = m_clock.getElapsedTime();

        if (timeElapsed.asSeconds() >= 0.01)
        {
            float moveX = m_vectorMovement.x; //tibia::getFloatNormal(m_vectorMovement.x);
            float moveY = m_vectorMovement.y; //tibia::getFloatNormal(m_vectorMovement.y);

            m_sprite.move(moveX * m_speed, moveY * m_speed);

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

    void update()
    {
        //updateTileCoords();

        //setPosition(getTileX(), getTileY());

        doMovement();

        setTileCoords(getSpriteTilePosition().x, getSpriteTilePosition().y);

        updateTileNumber();

        updateBox();
    }

    void setId(int id)
    {
        m_id = id;
    }

    int getId()
    {
        return m_id;
    }

    void setAnimationOnHit(int* animationOnHit)
    {
        m_animationOnHit = animationOnHit;
    }

    int* getAnimationOnHit()
    {
        return m_animationOnHit;
    }

    void setAnimationOnBlock(int* animationOnBlock)
    {
        m_animationOnBlock = animationOnBlock;
    }

    int* getAnimationOnBlock()
    {
        return m_animationOnBlock;
    }

    void setAnimatedDecalOnHit(int* animatedDecalOnHit)
    {
        m_animatedDecalOnHit = animatedDecalOnHit;
    }

    int* getAnimatedDecalOnHit()
    {
        return m_animatedDecalOnHit;
    }

    void setType(int type)
    {
        m_type = type;
    }

    int getType()
    {
        return m_type;
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
        sf::Vector2f position;

        //position.x = m_spriteTileX;
        //position.y = m_spriteTileY;

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

        while (x > 0 && x % tibia::TILE_SIZE != 0) x--;
        while (y > 0 && y % tibia::TILE_SIZE != 0) y--;

        position.x = x;
        position.y = y;

        return position;
    }

    tibia::Creature* getCreatureOwner()
    {
        return m_creatureOwner;
    }

    void setCreatureOwner(tibia::Creature* creature)
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
    int m_spriteType;

    float m_spriteTileX;
    float m_spriteTileY;

    int* m_animationOnHit;
    int* m_animationOnBlock;

    int* m_animatedDecalOnHit;

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

    tibia::Creature* m_creatureOwner;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite, states);
    }

};

}

#endif // TIBIA_PROJECTILE_HPP

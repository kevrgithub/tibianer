#ifndef TIBIA_CREATURE_HPP
#define TIBIA_CREATURE_HPP

#include <cstdlib>
#include <ctime>

#include <string>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Creature : public tibia::Thing
{

public:

    Creature::Creature()
    {
        m_tileOffset = tibia::TILE_DRAW_OFFSET;

        setZ(tibia::ZAxis::ground);

        m_isPlayer = false;

        m_distanceFromPlayer = 0;

        m_direction = tibia::Directions::down;

        m_type = tibia::CreatureTypes::human;

        m_team = tibia::Teams::neutral;

        m_hp    = 100;
        m_hpMax = 100;

        m_mp    = 200;
        m_mpMax = 200;

        m_movementSpeed = tibia::MovementSpeeds::default;

        m_isDead     = false;
        m_hasDecayed = false;

        m_hasOutfit = true;

        m_outfitHead = 0;
        m_outfitBody = 0;
        m_outfitLegs = 0;
        m_outfitFeet = 0;

        setPropertiesByType();

        m_clockMovement.restart();
    }

    void setPropertiesByType()
    {
        if (m_type != tibia::CreatureTypes::human)
        {
            m_hasOutfit = false;
        }

        switch (m_type)
        {
            case tibia::CreatureTypes::gameMaster:
                m_hp = 1000;
                m_hpMax = 1000;

                m_spritesList = tibia::CreatureSprites::gameMaster;
                break;

            case tibia::CreatureTypes::hero:
                m_hp    = 1000;
                m_hpMax = 1000;

                m_spritesList = tibia::CreatureSprites::hero;
                break;
        }
    }

    void updateSprite()
    {
        if (m_spritesList.size() == 0)
        {
            return;
        }

        m_sprite.setId(m_spritesList[m_direction]);
    }

    void setOutfit(int head, int body, int legs, int feet)
    {
        m_outfitHead = head;
        m_outfitBody = body;
        m_outfitLegs = legs;
        m_outfitFeet = feet;

        updateOutfit();
    }

    void setOutfitRandom()
    {
        m_outfitHead = tibia::getRandomNumber(0, 9);
        m_outfitBody = tibia::getRandomNumber(0, 10);
        m_outfitLegs = tibia::getRandomNumber(0, 9);
        m_outfitFeet = tibia::getRandomNumber(0, 9);

        updateOutfit();
    }

    void updateOutfit()
    {
        m_spriteOutfitHead.setId(tibia::Outfits::head[(m_outfitHead * 4) + m_direction]);
        m_spriteOutfitBody.setId(tibia::Outfits::body[(m_outfitBody * 4) + m_direction]);
        m_spriteOutfitLegs.setId(tibia::Outfits::legs[(m_outfitLegs * 4) + m_direction]);
        m_spriteOutfitFeet.setId(tibia::Outfits::feet[(m_outfitFeet * 4) + m_direction]);
    }

    void updateCorpse()
    {
        if (isDead() == false)
        {
            return;
        }

        m_timeCorpse = m_clockCorpse.getElapsedTime();

        if (m_timeCorpse.asSeconds() >= tibia::AnimationTimes::decal)
        {
            int corpseId = m_spriteCorpse.getId();

            if (corpseId == 0) m_spriteCorpse.setId(tibia::SpriteData::corpse[0]);

            if (corpseId >= tibia::SpriteData::corpse[0] && corpseId < tibia::SpriteData::corpse[6])
            {
                m_spriteCorpse.setId(corpseId + 1);
            }
            else if (corpseId == tibia::SpriteData::corpse[6])
            {
                setHasDecayed(true);
            }

            m_clockCorpse.restart();
        }
    }

    void update()
    {
        updateTileCoords();

        int tileOffset = m_tileOffset;

        if (m_isSitting == true)
        {
            tileOffset *= 2;
        }

        setPosition(getTileX() - tileOffset, getTileY() - tileOffset);

        updateSprite();

        updateOutfit();

        updateCorpse();
    }

    void doTurn(int direction)
    {
        int dir = direction;

        if (dir > tibia::Directions::left)
        {
            int random = tibia::getRandomNumber(1, 2);

            switch (dir)
            {
                case tibia::Directions::upLeft:
                    if (random == 1)
                    {
                        dir = tibia::Directions::up;
                    }
                    else
                    {
                        dir = tibia::Directions::left;
                    }
                    break;

                case tibia::Directions::upRight:
                    dir = tibia::getRandomNumber(tibia::Directions::up, tibia::Directions::right);
                    break;

                case tibia::Directions::downRight:
                    dir = tibia::getRandomNumber(tibia::Directions::right, tibia::Directions::down);
                    break;

                case tibia::Directions::downLeft:
                    dir = tibia::getRandomNumber(tibia::Directions::down, tibia::Directions::left);
                    break;
            }
        }

        setDirection(dir);
    }

    void doMove(int direction)
    {
        m_timeMovement = m_clockMovement.getElapsedTime();

        if (m_timeMovement.asSeconds() >= m_movementSpeed)
        {
            m_movementReady = true;
        }

        if (m_movementReady == false)
        {
            return;
        }

        int x = getX();
        int y = getY();

        switch (direction)
        {
            case tibia::Directions::up:
                if (y != 0)
                {
                    setY(y - 1);
                }
                break;

            case tibia::Directions::right:
                if (x != MAP_XY_MAX - 1)
                {
                    setX(x + 1);
                }
                break;

            case tibia::Directions::down:
                if (y != MAP_XY_MAX - 1)
                {
                    setY(y + 1);
                }
                break;

            case tibia::Directions::left:
                if (x != 0)
                {
                    setX(x - 1);
                }
                break;

            case tibia::Directions::upLeft:
                if (y != 0 && x != 0)
                {
                    setY(y - 1);
                    setX(x - 1);
                }
                break;

            case tibia::Directions::upRight:
                if (y != 0 && x != MAP_XY_MAX - 1)
                {
                    setY(y - 1);
                    setX(x + 1);
                }
                break;

            case tibia::Directions::downLeft:
                if (y != MAP_XY_MAX - 1 && x != 0)
                {
                    setY(y + 1);
                    setX(x - 1);
                }
                break;

            case tibia::Directions::downRight:
                if (y != MAP_XY_MAX - 1 && x != MAP_XY_MAX - 1)
                {
                    setY(y + 1);
                    setX(x + 1);
                }
                break;
        }

        m_movementReady = false;

        m_clockMovement.restart();
    }

    void takeDamage(int damage)
    {
        m_hp = m_hp - damage;

        if (m_hp <= 0)
        {
            m_hp = 0;

            setIsDead(true);
        }
    }

    void takeDamageFromCreature(tibia::Creature* creature, int damage)
    {
        m_attacker = creature;

        takeDamage(damage);
    }

    bool isDead()
    {
        return m_isDead;
    }

    void setIsDead(bool b)
    {
        m_isDead = b;

        if (m_isDead == true)
        {
            m_tileOffset = 0;

            m_spriteCorpse.setId(tibia::SpriteData::corpse[0]);

            m_clockCorpse.restart();
        }
        else
        {
            m_tileOffset = tibia::TILE_DRAW_OFFSET;
        }
    }

    bool hasDecayed()
    {
        return m_hasDecayed;
    }

    void setHasDecayed(bool b)
    {
        m_hasDecayed = b;
    }

    int getTileOffset()
    {
        return m_tileOffset;
    }

    void setTileOffset(int offset)
    {
        m_tileOffset = offset;
    }

    bool isPlayer()
    {
        return m_isPlayer;
    }

    void setIsPlayer(bool b)
    {
        m_isPlayer = b;
    }

    float getDistanceFromPlayer()
    {
        return m_distanceFromPlayer;
    }

    void setDistanceFromPlayer(float distance)
    {
        m_distanceFromPlayer = distance;
    }

    bool isSitting()
    {
        return m_isSitting;
    }

    void setIsSitting(bool b)
    {
        m_isSitting = b;
    }

    std::string getName()
    {
        return m_name;
    }

    void setName(std::string name)
    {
        m_name = name;
    }

    int getType()
    {
        return m_type;
    }

    void setType(int type)
    {
        m_type = type;
    }

    int getDirection()
    {
        return m_direction;
    }

    void setDirection(int direction)
    {
        m_direction = direction;
    }

    bool getMovementReady()
    {
        return m_movementReady;
    }

    void setMovementReady(bool b)
    {
        m_movementReady = b;
    }

    float getMovementSpeed()
    {
        return m_movementSpeed;
    }

    void setMovementSpeed(float movementSpeed)
    {
        m_movementSpeed = movementSpeed;
    }

    int getTeam()
    {
        return m_team;
    }

    void setTeam(int team)
    {
        m_team = team;
    }

    int getHp()
    {
        return m_hp;
    }

    void setHp(int hp)
    {
        m_hp = hp;
    }

    int getHpMax()
    {
        return m_hpMax;
    }

    void setHpMax(int hp)
    {
        m_hpMax = hp;
    }

    int getMp()
    {
        return m_mp;
    }

    void setMp(int mp)
    {
        m_mp = mp;
    }

    int getMpMax()
    {
        return m_mpMax;
    }

    void setMpMax(int mp)
    {
        m_mpMax = mp;
    }

    bool hasOutfit()
    {
        return m_hasOutfit;
    }

    void setHasOutfit(bool b)
    {
        m_hasOutfit = b;
    }

    int getOutfitHead()
    {
        return m_outfitHead;
    }

    void setOutfitHead(int head)
    {
        m_outfitHead = head;
    }

    int getOutfitBody()
    {
        return m_outfitBody;
    }

    void setOutfitBody(int body)
    {
        m_outfitBody = body;
    }

    int getOutfitLegs()
    {
        return m_outfitLegs;
    }

    void setOutfitLegs(int legs)
    {
        m_outfitLegs = legs;
    }

    int getOutfitFeet()
    {
        return m_outfitFeet;
    }

    void setOutfitFeet(int feet)
    {
        m_outfitFeet = feet;
    }

    tibia::Sprite getSpriteOutfitHead()
    {
        return m_spriteOutfitHead;
    }

    void setSpriteOutfitHead(tibia::Sprite head)
    {
        m_spriteOutfitHead = head;
    }

    tibia::Sprite getSpriteOutfitBody()
    {
        return m_spriteOutfitBody;
    }

    void setSpriteOutfitBody(tibia::Sprite body)
    {
        m_spriteOutfitBody = body;
    }

    tibia::Sprite getSpriteOutfitLegs()
    {
        return m_spriteOutfitLegs;
    }

    void setSpriteOutfitLegs(tibia::Sprite legs)
    {
        m_spriteOutfitLegs = legs;
    }

    tibia::Sprite getSpriteOutfitFeet()
    {
        return m_spriteOutfitFeet;
    }

    void setSpriteOutfitFeet(tibia::Sprite feet)
    {
        m_spriteOutfitFeet = feet;
    }

private:

    int m_tileOffset;

    bool m_isPlayer;

    float m_distanceFromPlayer;

    bool m_isSitting;

    int m_type;

    std::string m_name;

    int m_direction;

    bool m_movementReady;

    float m_movementSpeed;

    int m_team;

    int m_hp;
    int m_hpMax;

    int m_mp;
    int m_mpMax;

    int m_gold;
    int m_platinum;

    int m_exp;

    int m_level;
    int m_magicLevel;

    int m_cap;

    bool m_isAnimated;

    bool m_isDead;
    bool m_hasDecayed;

    sf::Clock m_clockCorpse;
    sf::Time m_timeCorpse;

    tibia::Sprite m_sprite;
    std::vector<int> m_spritesList;

    bool m_hasOutfit;

    int m_outfitHead;
    int m_outfitBody;
    int m_outfitLegs;
    int m_outfitFeet;

    tibia::Sprite m_spriteOutfitHead;
    tibia::Sprite m_spriteOutfitBody;
    tibia::Sprite m_spriteOutfitLegs;
    tibia::Sprite m_spriteOutfitFeet;

    tibia::Sprite m_spriteCorpse;

    sf::Clock m_clockMovement;
    sf::Time m_timeMovement;

    tibia::Creature* m_attacker;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        if (m_isDead == true)
        {
            target.draw(m_spriteCorpse, states);

            return;
        }

        if (m_hasOutfit == false)
        {
            target.draw(m_sprite, states);
        }
        else
        {
            target.draw(m_spriteOutfitFeet, states);
            target.draw(m_spriteOutfitLegs, states);
            target.draw(m_spriteOutfitBody, states);
            target.draw(m_spriteOutfitHead, states);
        }
    }

};

}

#endif // TIBIA_CREATURE_HPP
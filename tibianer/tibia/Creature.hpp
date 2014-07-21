#ifndef TIBIA_CREATURE_HPP
#define TIBIA_CREATURE_HPP

#include <cstdlib>
#include <ctime>

#include <string>

#include <SFML/Graphics.hpp>

#include "utility.hpp"

#include "tibia/Tibia.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Creature : public tibia::Thing
{

public:

    Creature::Creature(int tileX, int tileY, int z)
    {
        setTileCoords(tileX, tileY);
        setPosition(sf::Vector2f(tileX, tileY));

        setZ(z);

        m_tileOffset = tibia::THING_DRAW_OFFSET;

        m_isPlayer = false;

        m_name = tibia::CREATURE_NAME_DEFAULT;

        m_hasCustomName = false;

        m_direction = tibia::Directions::down;

        m_type = tibia::CreatureTypes::human;

        m_size = tibia::CreatureSizes::small;

        m_team = tibia::Teams::neutral;

        m_movementSpeed = tibia::MovementSpeeds::default;

        m_hp    = 100;
        m_hpMax = 100;

        m_mp    = 200;
        m_mpMax = 200;

        m_isDead     = false;
        m_hasDecayed = false;

        m_isSleeping = false;

        m_hasOutfit = true;

        m_outfitHead = 0;
        m_outfitBody = 0;
        m_outfitLegs = 0;
        m_outfitFeet = 0;

        setPropertiesByType();
    }

    void setPropertiesByType()
    {
        if (m_hasCustomName == false)
        {
            m_name = tibia::umapCreatureNames[m_type];
        }

        if (m_type != tibia::CreatureTypes::human)
        {
            m_hasOutfit = false;
        }

        m_size = umapCreatureSizes[m_type];

        if (m_size == tibia::CreatureSizes::large)
        {
            m_tileOffset = 0;
        }
        else
        {
            m_tileOffset = tibia::THING_DRAW_OFFSET;
        }

        m_spritesList = umapCreatureSprites[m_type];
    }

    void updateSprite()
    {
        if (m_spritesList.size() == 0)
        {
            return;
        }

        m_sprite[0].setId(m_spritesList[m_direction]);

        if (m_size == tibia::CreatureSizes::medium)
        {
            sf::Vector2f spriteOffset(0, 0);

            switch (m_direction)
            {
                case tibia::Directions::up:
                case tibia::Directions::down:
                    spriteOffset.y -= tibia::TILE_SIZE;
                    break;

                case tibia::Directions::right:
                case tibia::Directions::left:
                    spriteOffset.x -= tibia::TILE_SIZE;
                    break;
            }

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x + spriteOffset.x,
                    m_sprite[0].getPosition().y + spriteOffset.y
                )
            );

            m_sprite[1].setId(m_spritesList[m_direction + 4]);
        }
        else if (m_size == tibia::CreatureSizes::large)
        {
            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[1].setId(m_spritesList[m_direction + 4]);
            m_sprite[2].setId(m_spritesList[m_direction + 8]);
            m_sprite[3].setId(m_spritesList[m_direction + 12]);
        }
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
        m_outfitHead = getRandomNumber(0, (tibia::Outfits::head.size() / 4) - 1);
        m_outfitBody = getRandomNumber(0, (tibia::Outfits::body.size() / 4) - 1);
        m_outfitLegs = getRandomNumber(0, (tibia::Outfits::legs.size() / 4) - 1);
        m_outfitFeet = getRandomNumber(0, (tibia::Outfits::feet.size() / 4) - 1);

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

        if (m_timeCorpse.asSeconds() >= tibia::AnimationTimes::corpseDecay)
        {
            int corpseId = m_spriteCorpse.getId();

            if (corpseId == 0)
            {
                m_spriteCorpse.setId(tibia::SpriteData::corpse[0]);
            }

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

        int drawOffset = getDrawOffset() * tibia::THING_DRAW_OFFSET;

        if (m_isDead == false)
        {
            drawOffset += m_tileOffset;
        }

        setPosition(getTileX() - drawOffset, getTileY() - drawOffset);

        //setPosition(getTileX(), getTileY());

        updateSprite();

        updateOutfit();

        updateCorpse();
    }

    void doTurn(int direction)
    {
        int dir = direction;

        if (dir > tibia::Directions::left)
        {
            int random = getRandomNumber(1, 2);

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
                    dir = getRandomNumber(tibia::Directions::up, tibia::Directions::right);
                    break;

                case tibia::Directions::downRight:
                    dir = getRandomNumber(tibia::Directions::right, tibia::Directions::down);
                    break;

                case tibia::Directions::downLeft:
                    dir = getRandomNumber(tibia::Directions::down, tibia::Directions::left);
                    break;
            }
        }

        setDirection(dir);
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
            m_spriteCorpse.setId(tibia::SpriteData::corpse[0]);

            m_clockCorpse.restart();
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

    bool isSleeping()
    {
        return m_isSleeping;
    }

    void setIsSleeping(bool b)
    {
        m_isSleeping = b;
    }

    std::string getName()
    {
        return m_name;
    }

    void setName(std::string name)
    {
        m_name = name;
    }

    bool hasCustomName()
    {
        return m_hasCustomName;
    }

    void setHasCustomName(bool b)
    {
        m_hasCustomName = b;
    }

    int getType()
    {
        return m_type;
    }

    void setType(int type)
    {
        m_type = type;

        setPropertiesByType();
    }

    int getSize()
    {
        return m_size;
    }

    void setSize(int size)
    {
        m_size = size;
    }

    int getDirection()
    {
        return m_direction;
    }

    void setDirection(int direction)
    {
        m_direction = direction;
    }

    bool getIsMovementReady()
    {
        return m_isMovementReady;
    }

    void setIsMovementReady(bool b)
    {
        m_isMovementReady = b;
    }

    bool getIsLogicReady()
    {
        return m_isLogicReady;
    }

    void setIsLogicReady(bool b)
    {
        m_isLogicReady = b;
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

    tibia::Sprite* getSpriteOutfitHead()
    {
        return &m_spriteOutfitHead;
    }

    void setSpriteOutfitHead(tibia::Sprite head)
    {
        m_spriteOutfitHead = head;
    }

    tibia::Sprite* getSpriteOutfitBody()
    {
        return &m_spriteOutfitBody;
    }

    void setSpriteOutfitBody(tibia::Sprite body)
    {
        m_spriteOutfitBody = body;
    }

    tibia::Sprite* getSpriteOutfitLegs()
    {
        return &m_spriteOutfitLegs;
    }

    void setSpriteOutfitLegs(tibia::Sprite legs)
    {
        m_spriteOutfitLegs = legs;
    }

    tibia::Sprite* getSpriteOutfitFeet()
    {
        return &m_spriteOutfitFeet;
    }

    void setSpriteOutfitFeet(tibia::Sprite feet)
    {
        m_spriteOutfitFeet = feet;
    }

    sf::Clock* getClockMovement()
    {
        return &m_clockMovement;
    }

    sf::Clock* getClockLogic()
    {
        return &m_clockLogic;
    }

private:

    int m_tileOffset;

    bool m_isPlayer;

    int m_type;

    int m_size;

    std::string m_name;

    bool m_hasCustomName;

    int m_direction;

    bool m_isMovementReady;
    bool m_isLogicReady;

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

    bool m_isSleeping;

    sf::Clock m_clockCorpse;
    sf::Time m_timeCorpse;

    std::vector<int> m_spritesList;
    std::vector<int> m_spritesCorpseList;

    tibia::Sprite m_sprite[4];
    tibia::Sprite m_spriteCorpse;//[4];

    bool m_hasOutfit;

    int m_outfitHead;
    int m_outfitBody;
    int m_outfitLegs;
    int m_outfitFeet;

    tibia::Sprite m_spriteOutfitHead;
    tibia::Sprite m_spriteOutfitBody;
    tibia::Sprite m_spriteOutfitLegs;
    tibia::Sprite m_spriteOutfitFeet;

    sf::Clock m_clockMovement;
    sf::Clock m_clockLogic;

    tibia::Creature* m_attacker;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        if (m_isDead == true)
        {
            target.draw(m_spriteCorpse, states);

            return;
        }

        if (m_isSleeping == true)
        {
            return;
        }

        if (m_hasOutfit == false)
        {
            target.draw(m_sprite[0], states);

            if (m_size == tibia::CreatureSizes::medium)
            {
                target.draw(m_sprite[1], states);
            }
            else if (m_size == tibia::CreatureSizes::large)
            {
                target.draw(m_sprite[1], states);
                target.draw(m_sprite[2], states);
                target.draw(m_sprite[3], states);
            }
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

typedef std::shared_ptr<tibia::Creature> CreaturePtr;
typedef std::vector<CreaturePtr> CreatureList;

}

#endif // TIBIA_CREATURE_HPP
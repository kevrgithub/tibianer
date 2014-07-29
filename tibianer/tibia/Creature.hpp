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

    typedef std::shared_ptr<tibia::Creature> Ptr;
    typedef std::vector<tibia::Creature::Ptr> List;

    Creature::Creature(int tileX, int tileY, int z)
    {
        setTileCoords(tileX, tileY);
        setPosition(sf::Vector2f(tileX, tileY));

        setZ(z);

        m_tileOffset = tibia::THING_DRAW_OFFSET;

        m_isPlayer      = false;
        m_hasCustomName = false;
        m_isDead        = false;
        m_isSleeping    = false;
        m_hasOutfit     = true;

        m_name = tibia::CREATURE_NAME_DEFAULT;

        m_direction = tibia::Directions::down;

        m_team = tibia::Teams::good;

        m_type = tibia::CreatureTypes::human;

        m_size = tibia::CreatureSizes::small;

        m_corpseSize = tibia::CreatureCorpseSizes::small;

        m_bloodType = tibia::CreatureBloodTypes::red;

        m_movementSpeed = tibia::MovementSpeeds::default;

        m_numAnimations = 1;

        m_currentAnimation = 0;

        m_spritesList = tibia::CreatureSprites::human;

        m_spritesCorpseList = tibia::CreatureCorpseSprites::human;

        m_outfit[0] = tibia::Outfits::default[0];
        m_outfit[1] = tibia::Outfits::default[1];
        m_outfit[2] = tibia::Outfits::default[2];
        m_outfit[3] = tibia::Outfits::default[3];

        m_hp    = 100;
        m_hpMax = 100;

        m_mp    = 200;
        m_mpMax = 200;

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

        m_numAnimations = umapCreatureNumAnimations[m_type];

        m_corpseSize = umapCreatureCorpseSizes[m_type];

        m_spritesList = umapCreatureSprites[m_type];

        m_spritesCorpseList = umapCreatureCorpseSprites[m_type];

        m_spriteCorpse[0].setId(m_spritesCorpseList[0]);

        if (m_corpseSize == tibia::CreatureCorpseSizes::medium)
        {
            // move corpse so it doesn't overlap creatures
            m_spriteCorpse[0].move
            (
                sf::Vector2f
                (
                    tibia::TILE_SIZE,
                    0
                )
            );

            m_spriteCorpse[1].setId(m_spritesCorpseList[1]);

            m_spriteCorpse[1].setPosition
            (
                sf::Vector2f
                (
                    m_spriteCorpse[0].getPosition().x - tibia::TILE_SIZE,
                    m_spriteCorpse[0].getPosition().y
                )
            );
        }
        else if (m_corpseSize == tibia::CreatureCorpseSizes::large)
        {
            // move corpse so it doesn't overlap creatures
            m_spriteCorpse[0].move
            (
                sf::Vector2f
                (
                    tibia::TILE_SIZE,
                    tibia::TILE_SIZE
                )
            );

            m_spriteCorpse[1].setId(m_spritesCorpseList[1]);
            m_spriteCorpse[2].setId(m_spritesCorpseList[2]);
            m_spriteCorpse[3].setId(m_spritesCorpseList[3]);

            m_spriteCorpse[1].setPosition
            (
                sf::Vector2f
                (
                    m_spriteCorpse[0].getPosition().x - tibia::TILE_SIZE,
                    m_spriteCorpse[0].getPosition().y
                )
            );

            m_spriteCorpse[2].setPosition
            (
                sf::Vector2f
                (
                    m_spriteCorpse[0].getPosition().x,
                    m_spriteCorpse[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_spriteCorpse[3].setPosition
            (
                sf::Vector2f
                (
                    m_spriteCorpse[0].getPosition().x - tibia::TILE_SIZE,
                    m_spriteCorpse[0].getPosition().y - tibia::TILE_SIZE
                )
            );
        }

        m_bloodType = umapCreatureBloodTypes[m_type];
    }

    void updateSprite()
    {
        if (m_spritesList.size() == 0)
        {
            return;
        }

        if (m_size == tibia::CreatureSizes::small)
        {
            m_sprite[0].setId(m_spritesList[m_direction + (4 * m_currentAnimation)]);
        }
        else if (m_size == tibia::CreatureSizes::medium)
        {
            m_sprite[0].setId(m_spritesList[m_direction +     (8 * m_currentAnimation)]);
            m_sprite[1].setId(m_spritesList[m_direction + 4 + (8 * m_currentAnimation)]);

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
        }
        else if (m_size == tibia::CreatureSizes::large)
        {
            m_sprite[0].setId(m_spritesList[m_direction      + (16 * m_currentAnimation)]);
            m_sprite[1].setId(m_spritesList[m_direction + 4  + (16 * m_currentAnimation)]);
            m_sprite[2].setId(m_spritesList[m_direction + 8  + (16 * m_currentAnimation)]);
            m_sprite[3].setId(m_spritesList[m_direction + 12 + (16 * m_currentAnimation)]);

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
        }
    }

    void setOutfit(int head, int body, int legs, int feet)
    {
        m_outfit[0] = head;
        m_outfit[1] = body;
        m_outfit[2] = legs;
        m_outfit[3] = feet;

        updateOutfit();
    }

    void setOutfitRandom()
    {
        m_outfit[0] = getRandomNumber(0, (tibia::Outfits::head.size() / 4) - 1);
        m_outfit[1] = getRandomNumber(0, (tibia::Outfits::body.size() / 4) - 1);
        m_outfit[2] = getRandomNumber(0, (tibia::Outfits::legs.size() / 4) - 1);
        m_outfit[3] = getRandomNumber(0, (tibia::Outfits::feet.size() / 4) - 1);

        updateOutfit();
    }

    void updateOutfit()
    {
        m_spriteOutfit[0].setId(tibia::Outfits::head[(m_outfit[0] * 4) + m_direction]);
        m_spriteOutfit[1].setId(tibia::Outfits::body[(m_outfit[1] * 4) + m_direction]);
        m_spriteOutfit[2].setId(tibia::Outfits::legs[(m_outfit[2] * 4) + m_direction]);
        m_spriteOutfit[3].setId(tibia::Outfits::feet[(m_outfit[3] * 4) + m_direction]);
    }

    void updateAnimation()
    {
        m_timeAnimation = m_clockAnimation.getElapsedTime();

        if (m_timeAnimation.asSeconds() >= tibia::AnimationTimes::creatureAnimation)
        {
            m_currentAnimation++;

            if (m_currentAnimation > m_numAnimations - 1)
            {
                m_currentAnimation = 0;
            }

            m_clockAnimation.restart();
        }
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
            int corpseId = m_spriteCorpse[0].getId();

            for (unsigned int i = 0; i < m_spritesCorpseList.size(); i++)
            {
                int spriteId = m_spritesCorpseList.at(i);

                if (corpseId == spriteId)
                {
                    if (m_corpseSize == tibia::CreatureCorpseSizes::medium)
                    {
                        i = i + 2;
                    }
                    else if (m_corpseSize == tibia::CreatureCorpseSizes::large)
                    {
                        i = i + 4;
                    }
                    else
                    {
                        i++;
                    }

                    if (i > m_spritesCorpseList.size() - 1)
                    {
                        setIsReadyForErase(true);
                        return;
                    }

                    m_spriteCorpse[0].setId(m_spritesCorpseList.at(i));

                    if (m_corpseSize == tibia::CreatureCorpseSizes::medium)
                    {
                        m_spriteCorpse[1].setId(m_spritesCorpseList.at(i + 1));
                    }
                    else if (m_corpseSize == tibia::CreatureCorpseSizes::large)
                    {
                        m_spriteCorpse[1].setId(m_spritesCorpseList.at(i + 1));
                        m_spriteCorpse[2].setId(m_spritesCorpseList.at(i + 2));
                        m_spriteCorpse[3].setId(m_spritesCorpseList.at(i + 3));
                    }

                    break;
                }
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

        updateSprite();
        updateOutfit();
        updateAnimation();
        updateCorpse();
    }

    void doTurn(int direction)
    {
        if (direction == tibia::Directions::null)
        {
            return;
        }

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

    void modifyHp(int hpChange)
    {
        m_hp = m_hp + hpChange;

        if (m_hp > m_hpMax)
        {
            m_hp = m_hpMax;
        }

        if (m_hp <= 0)
        {
            m_hp = 0;

            setIsDead(true);
        }
    }

    void setLastAttacker(tibia::Creature::Ptr creature)
    {
        m_lastAttacker = creature;
    }

    tibia::Creature::Ptr getLastAttacker()
    {
        return m_lastAttacker;
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
            m_clockCorpse.restart();
        }
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

    int getBloodType()
    {
        return m_bloodType;
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
        return m_outfit[0];
    }

    void setOutfitHead(int head)
    {
        m_outfit[0] = head;
    }

    int getOutfitBody()
    {
        return m_outfit[1];
    }

    void setOutfitBody(int body)
    {
        m_outfit[1] = body;
    }

    int getOutfitLegs()
    {
        return m_outfit[2];
    }

    void setOutfitLegs(int legs)
    {
        m_outfit[2] = legs;
    }

    int getOutfitFeet()
    {
        return m_outfit[3];
    }

    void setOutfitFeet(int feet)
    {
        m_outfit[3] = feet;
    }

    tibia::Sprite* getSpriteOutfitHead()
    {
        return &m_spriteOutfit[0];
    }

    tibia::Sprite* getSpriteOutfitBody()
    {
        return &m_spriteOutfit[1];
    }

    tibia::Sprite* getSpriteOutfitLegs()
    {
        return &m_spriteOutfit[2];
    }

    tibia::Sprite* getSpriteOutfitFeet()
    {
        return &m_spriteOutfit[3];
    }

    sf::Clock* getClockMovement()
    {
        return &m_clockMovement;
    }

    sf::Clock* getClockLogic()
    {
        return &m_clockLogic;
    }

    sf::Clock* getClockCorpse()
    {
        return &m_clockCorpse;
    }

    sf::Clock* getClockAnimation()
    {
        return &m_clockAnimation;
    }

private:

    int m_tileOffset;

    bool m_isPlayer;

    int m_type;

    int m_size;

    int m_corpseSize;

    int m_bloodType;

    int m_numAnimations;
    int m_currentAnimation;

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

    bool m_isDead;

    bool m_isSleeping;

    sf::Clock m_clockAnimation;
    sf::Time m_timeAnimation;

    sf::Clock m_clockCorpse;
    sf::Time m_timeCorpse;

    std::vector<int> m_spritesList;
    std::vector<int> m_spritesCorpseList;

    tibia::Sprite m_sprite[tibia::NUM_CREATURE_SPRITES];
    tibia::Sprite m_spriteCorpse[tibia::NUM_CREATURE_SPRITES];

    bool m_hasOutfit;

    // head, body, legs, feet
    int m_outfit[tibia::NUM_CREATURE_SPRITES];

    tibia::Sprite m_spriteOutfit[tibia::NUM_CREATURE_SPRITES];

    sf::Clock m_clockMovement;
    sf::Clock m_clockLogic;

    tibia::Creature::Ptr m_lastAttacker;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        if (m_isDead == true)
        {
            target.draw(m_spriteCorpse[0], states);

            if (m_corpseSize == tibia::CreatureCorpseSizes::medium)
            {
                target.draw(m_spriteCorpse[1], states);
            }
            else if (m_corpseSize == tibia::CreatureCorpseSizes::large)
            {
                target.draw(m_spriteCorpse[1], states);
                target.draw(m_spriteCorpse[2], states);
                target.draw(m_spriteCorpse[3], states);
            }

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
            // draw in reverse: feet, legs, body, head
            target.draw(m_spriteOutfit[3], states);
            target.draw(m_spriteOutfit[2], states);
            target.draw(m_spriteOutfit[1], states);
            target.draw(m_spriteOutfit[0], states);
        }
    }

}; // class Creature

namespace CreatureSort
{
    struct sortByIsDead
    {
        bool operator()(tibia::Creature::Ptr a, tibia::Creature::Ptr b) const
        {
            return (a->isDead() > b->isDead());
        }
    };
}

} // namespace tibia

#endif // TIBIA_CREATURE_HPP
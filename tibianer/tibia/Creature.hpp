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

    struct Skills_t
    {
        int fistFighting     = 10;
        int clubFighting     = 10;
        int swordFighting    = 10;
        int axeFighting      = 10;
        int distanceFighting = 10;
        int shielding        = 10;
        int fishing          = 10;
    };

    Skills_t skills;

    struct StatusEffect_t
    {
        int type;
        int damage;
        unsigned int ticks;

        sf::Time timePerTick;
        sf::Clock clock;

        std::string name;
    };

    typedef std::vector<tibia::Creature::StatusEffect_t> StatusEffectList;

    struct InventoryItem_t
    {
        int id;
        int count;
        tibia::SpriteFlags_t flags;
    };

    typedef std::vector<tibia::Creature::InventoryItem_t> InventoryItemList;

    struct InventoryItemSortById_t
    {
        bool operator()(tibia::Creature::InventoryItem_t a, tibia::Creature::InventoryItem_t b) const
        {
            return a.id < b.id;
        }
    };

    struct InventoryItemSortByCount_t
    {
        bool operator()(tibia::Creature::InventoryItem_t a, tibia::Creature::InventoryItem_t b) const
        {
            return a.count > b.count;
        }
    };

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

        m_vocation = tibia::Vocations::none;

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

        m_level      = 1;
        m_magicLevel = 1;

        m_exp = 0;

        m_cap = tibia::INVENTORY_ITEMS_MAX;

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
            m_name = tibia::UMaps::creatureNames[m_type];
        }

        if (m_type != tibia::CreatureTypes::human)
        {
            m_hasOutfit = false;
        }

        m_size = tibia::UMaps::creatureSizes[m_type];

        if (m_size == tibia::CreatureSizes::large)
        {
            m_tileOffset = 0;
        }
        else
        {
            m_tileOffset = tibia::THING_DRAW_OFFSET;
        }

        m_numAnimations = tibia::UMaps::creatureNumAnimations[m_type];

        m_corpseSize = tibia::UMaps::creatureCorpseSizes[m_type];

        m_spritesList = tibia::UMaps::creatureSprites[m_type];

        m_spritesCorpseList = tibia::UMaps::creatureCorpseSprites[m_type];

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

        m_bloodType = tibia::UMaps::creatureBloodTypes[m_type];
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
        m_outfit[0] = utility::getRandomNumber(0, (tibia::Outfits::head.size() / 4) - 1);
        m_outfit[1] = utility::getRandomNumber(0, (tibia::Outfits::body.size() / 4) - 1);
        m_outfit[2] = utility::getRandomNumber(0, (tibia::Outfits::legs.size() / 4) - 1);
        m_outfit[3] = utility::getRandomNumber(0, (tibia::Outfits::feet.size() / 4) - 1);

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

        if (m_timeAnimation.asSeconds() >= tibia::CreatureData::animationFrameTime)
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
        int corpseId = m_spriteCorpse[0].getId();

        for (std::size_t i = 0; i < m_spritesCorpseList.size(); i++)
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
            int random = utility::getRandomNumber(1, 2);

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
                    dir = utility::getRandomNumber(tibia::Directions::up, tibia::Directions::right);
                    break;

                case tibia::Directions::downRight:
                    dir = utility::getRandomNumber(tibia::Directions::right, tibia::Directions::down);
                    break;

                case tibia::Directions::downLeft:
                    dir = utility::getRandomNumber(tibia::Directions::down, tibia::Directions::left);
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

    tibia::Creature::StatusEffectList::iterator findStatusEffect(int type)
    {
        return std::find_if
        (
            m_statusEffectList.begin(),
            m_statusEffectList.end(),
            [&type](tibia::Creature::StatusEffect_t const& statusEffect)
            { 
                return statusEffect.type == type;
            }
        );
    }

    bool hasStatusEffect(int type)
    {
        auto findStatusEffectIt = findStatusEffect(type);

        return findStatusEffectIt != m_statusEffectList.end();
    }

    void addStatusEffect(int type, int damage, unsigned int ticks, sf::Time timePerTick)
    {
        auto findStatusEffectIt = findStatusEffect(type);

        if (findStatusEffectIt != m_statusEffectList.end())
        {
            findStatusEffectIt->ticks = ticks;
            findStatusEffectIt->clock.restart();
            return;
        }

        tibia::Creature::StatusEffect_t statusEffect;
        statusEffect.type        = type;
        statusEffect.damage      = damage;
        statusEffect.ticks       = ticks;
        statusEffect.timePerTick = timePerTick;

        statusEffect.name = tibia::UMaps::creatureStatusEffectsNames[type];

        statusEffect.clock.restart();

        m_statusEffectList.push_back(statusEffect);
    }

    tibia::Creature::InventoryItemList::iterator findInventoryItem(int id)
    {
        return std::find_if
        (
            m_inventoryItemList.begin(),
            m_inventoryItemList.end(),
            [&id](tibia::Creature::InventoryItem_t const& inventoryItem)
            { 
                return inventoryItem.id == id;
            }
        );
    }

    bool hasInventoryItem(int id)
    {
        auto findInventoryItemIt = findInventoryItem(id);

        return findInventoryItemIt != m_inventoryItemList.end();
    }

    int addInventoryItem(int id, int count,  tibia::SpriteFlags_t flags)
    {
        bool foundGroupableObject = false;

        int groupableObjectsIndex = 0;

        std::size_t groupableObjectsSize = 1;

        tibia::Creature::InventoryItemList::iterator groupableInventoryItemIt;

        if (flags.test(tibia::SpriteFlags::groupable))
        {
            for (auto inventoryItemIt = m_inventoryItemList.begin(); inventoryItemIt != m_inventoryItemList.end(); inventoryItemIt++)
            {
                if (foundGroupableObject == true)
                {
                    break;
                }

                if (inventoryItemIt->flags.test(tibia::SpriteFlags::groupable))
                {
                    groupableObjectsIndex = 0;

                    for (auto& groupableObjects : tibia::groupedObjectsList)
                    {
                        if (foundGroupableObject == true)
                        {
                            break;
                        }

                        for (auto groupableObject : groupableObjects)
                        {
                            if (foundGroupableObject == true)
                            {
                                break;
                            }

                            if (groupableObject == inventoryItemIt->id)
                            {
                                for (auto groupableObject : groupableObjects)
                                {
                                    if (groupableObject == id)
                                    {
                                        groupableInventoryItemIt = inventoryItemIt;

                                        groupableObjectsSize = groupableObjects.size();

                                        foundGroupableObject = true;
                                        break;
                                    }
                                }
                            }
                        }

                        if (foundGroupableObject == false)
                        {
                            groupableObjectsIndex++;
                        }
                    }
                }
            }
        }

        if (flags.test(tibia::SpriteFlags::stackable))
        {
            if (foundGroupableObject == true)
            {
                if ((groupableInventoryItemIt->count + count) > tibia::INVENTORY_ITEM_COUNT_MAX)
                {
                    if ((m_inventoryItemList.size() + 1) == tibia::INVENTORY_ITEMS_MAX)
                    {
                        return tibia::CreatureAddInventoryItemResult::inventoryItemsMax;
                    }

                    int remainderCount = (groupableInventoryItemIt->count + count) - tibia::INVENTORY_ITEM_COUNT_MAX;

                    groupableInventoryItemIt->count = tibia::INVENTORY_ITEM_COUNT_MAX;

                    groupableInventoryItemIt->id =
                        tibia::groupedObjectsList
                            .at(groupableObjectsIndex)
                            .at(tibia::Utility::getGroupableObjectIndexByCount(groupableInventoryItemIt->count, groupableObjectsSize));

                    tibia::Creature::InventoryItem_t inventoryItem;
                    inventoryItem.id =
                        tibia::groupedObjectsList
                            .at(groupableObjectsIndex)
                            .at(tibia::Utility::getGroupableObjectIndexByCount(remainderCount, groupableObjectsSize));
                    inventoryItem.count = remainderCount;
                    inventoryItem.flags = groupableInventoryItemIt->flags;

                    m_inventoryItemList.insert(m_inventoryItemList.begin(), inventoryItem);

                    return tibia::CreatureAddInventoryItemResult::success;
                }

                groupableInventoryItemIt->count += count;

                groupableInventoryItemIt->id =
                    tibia::groupedObjectsList
                        .at(groupableObjectsIndex)
                        .at(tibia::Utility::getGroupableObjectIndexByCount(groupableInventoryItemIt->count, groupableObjectsSize));

                return tibia::CreatureAddInventoryItemResult::success;
            }

            auto findInventoryItemIt = findInventoryItem(id);

            if (findInventoryItemIt != m_inventoryItemList.end())
            {
                if ((findInventoryItemIt->count + count) > tibia::INVENTORY_ITEM_COUNT_MAX)
                {
                    return tibia::CreatureAddInventoryItemResult::inventoryItemCountMax;
                }

                findInventoryItemIt->count += count;
                return tibia::CreatureAddInventoryItemResult::success;
            }
        }

        if (m_inventoryItemList.size() == tibia::INVENTORY_ITEMS_MAX)
        {
            return tibia::CreatureAddInventoryItemResult::inventoryItemsMax;
        }

        tibia::Creature::InventoryItem_t inventoryItem;
        inventoryItem.id    = id;
        inventoryItem.count = count;
        inventoryItem.flags = flags;

        m_inventoryItemList.insert(m_inventoryItemList.begin(), inventoryItem);
        //m_inventoryItemList.push_back(inventoryItem);

        return tibia::CreatureAddInventoryItemResult::success;
    }

    void removeInventoryItem(int index)
    {
        m_inventoryItemList.erase(m_inventoryItemList.begin() + index);
    }

    void removeInventoryItem(tibia::Creature::InventoryItemList::iterator it)
    {
        m_inventoryItemList.erase(it);
    }

    void sortInventoryItemsReverse()
    {
        std::reverse(m_inventoryItemList.begin(), m_inventoryItemList.end());
    }

    void sortInventoryItemsById()
    {
        std::sort(m_inventoryItemList.begin(), m_inventoryItemList.end(), InventoryItemSortById_t());
    }

    void sortInventoryItemsByCount()
    {
        sortInventoryItemsById();
        std::stable_sort(m_inventoryItemList.begin(), m_inventoryItemList.end(), InventoryItemSortByCount_t());
    }

    int getHealthState()
    {
        if (m_hp <= 0 || m_hpMax <= 0)
        {
            return tibia::HealthStates::dead;
        }

        float hpPercent = ((float)m_hp / (float)m_hpMax) * 100.0;

        //std::cout << "name:      " << m_name << std::endl;
        //std::cout << "hp:        " << m_hp << std::endl;
        //std::cout << "hpMax:     " << m_hpMax << std::endl;
        //std::cout << "hpPercent: " << hpPercent << std::endl;
        //std::cout << std::endl;

        if (hpPercent >= 91)
        {
            return tibia::HealthStates::healthy;
        }
        else if (hpPercent >= 76 && hpPercent <= 90)
        {
            return tibia::HealthStates::barelyWounded;
        }
        else if (hpPercent >= 51 && hpPercent <= 75)
        {
            return tibia::HealthStates::lightlyWounded;
        }
        else if (hpPercent >= 26 && hpPercent <= 50)
        {
            return tibia::HealthStates::heavilyWounded;
        }
        else if (hpPercent >= 11 && hpPercent <= 25)
        {
            return tibia::HealthStates::critical;
        }
        else if (hpPercent >= 1 && hpPercent <= 10)
        {
            return tibia::HealthStates::nearlyDead;
        }
        else if (hpPercent <= 0)
        {
            return tibia::HealthStates::dead;
        }

        return tibia::HealthStates::unknown;
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

            m_clockDead.restart();
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

    int getCorpseSize()
    {
        return m_corpseSize;
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

    int getVocation()
    {
        return m_vocation;
    }

    void setVocation(int vocation)
    {
        m_vocation = vocation;
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

    int getLevel()
    {
        return m_level;
    }

    void setLevel(int level)
    {
        m_level = level;
    }

    int getMagicLevel()
    {
        return m_magicLevel;
    }

    void setMagicLevel(int magicLevel)
    {
        m_magicLevel = magicLevel;
    }

    unsigned int getExp()
    {
        return m_exp;
    }

    void setExp(unsigned int exp)
    {
        m_exp = exp;
    }

    int getCap()
    {
        return m_cap;
    }

    void setCap(int cap)
    {
        m_cap = cap;
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

    sf::Clock* getClockDead()
    {
        return &m_clockDead;
    }

    sf::Clock* getClockAnimation()
    {
        return &m_clockAnimation;
    }

    tibia::Creature::StatusEffectList* getStatusEffectList()
    {
        return &m_statusEffectList;
    }

    tibia::Creature::InventoryItemList* getInventoryItemList()
    {
        return &m_inventoryItemList;
    }

    std::vector<int>* getSpritesList()
    {
        return &m_spritesList;
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

    int m_vocation;

    int m_hp;
    int m_hpMax;

    int m_mp;
    int m_mpMax;

    int m_gold;
    int m_platinum;

    unsigned int m_exp; // experience points

    int m_level;
    int m_magicLevel;

    int m_cap;

    bool m_isDead;

    bool m_isSleeping;

    sf::Clock m_clockAnimation;
    sf::Time m_timeAnimation;

    sf::Clock m_clockCorpse;

    sf::Clock m_clockDead;

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

    tibia::Creature::StatusEffectList m_statusEffectList;

    tibia::Creature::InventoryItemList m_inventoryItemList;

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

} // namespace tibia

#endif // TIBIA_CREATURE_HPP
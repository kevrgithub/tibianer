#ifndef TIBIA_CREATURE_HPP
#define TIBIA_CREATURE_HPP

#include <cstdlib>
#include <ctime>

#include <string>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/DrawableAndTransformable.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Creature : public tibia::DrawableAndTransformable
{

public:

    Creature::Creature()
    {
        m_tileOffset = tibia::TILE_DRAW_OFFSET;

        m_z = 0;

        m_direction = 2;

        m_movementSpeed = 0.5;

        m_outfitHead = 0;
        m_outfitBody = 0;
        m_outfitLegs = 0;
        m_outfitFeet = 0;

        m_clockMovement.restart();
    }

    void setCoords(int x, int y)
    {
        setX(x);
        setY(y);

        setTileX(x * tibia::TILE_SIZE);
        setTileY(y * tibia::TILE_SIZE);
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

    void updateTileCoords()
    {
        m_tileX = m_x * tibia::TILE_SIZE;
        m_tileY = m_y * tibia::TILE_SIZE;
    }

    void update()
    {
        updateTileCoords();

        int tileOffset = m_tileOffset;

        if (m_isSitting == true)
        {
            tileOffset *= 2;
        }

        setPosition(m_tileX - tileOffset, m_tileY - tileOffset);

        updateOutfit();
    }

    void doTurn(int direction)
    {
        setDirection(direction);
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

        switch (direction)
        {
            case tibia::Directions::up:
                if (m_y != 0)
                {
                    m_y--;
                }
                break;

            case tibia::Directions::right:
                if (m_x != MAP_XY_MAX)
                {
                    m_x++;
                }
                break;

            case tibia::Directions::down:
                if (m_y != MAP_XY_MAX)
                {
                    m_y++;
                }
                break;

            case tibia::Directions::left:
                if (m_x != 0)
                {
                    m_x--;
                }
                break;
        }

        m_movementReady = false;

        m_clockMovement.restart();
    }

    int getTileX()
    {
        return m_tileX;
    }

    void setTileX(int x)
    {
        m_tileX = x;
    }

    int getTileY()
    {
        return m_tileY;
    }

    void setTileY(int y)
    {
        m_tileY = y;
    }

    int getTileOffset()
    {
        return m_tileOffset;
    }

    void setTileOffset(int offset)
    {
        m_tileOffset = offset;
    }

    bool getIsSitting()
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

    int getX()
    {
        return m_x;
    }

    void setX(int x)
    {
        m_x = x;
    }

    int getY()
    {
        return m_y;
    }

    void setY(int y)
    {
        m_y = y;
    }

    int getZ()
    {
        return m_z;
    }

    void setZ(int z)
    {
        m_z = z;
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

    int getHp()
    {
        return m_hp;
    }

    void setHp(int hp)
    {
        m_hp = hp;
    }

    int getMp()
    {
        return m_mp;
    }

    void setMp(int mp)
    {
        m_mp = mp;
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

    sf::Clock getClockMovement()
    {
        return m_clockMovement;
    }

private:

    int m_tileX;
    int m_tileY;

    int m_tileOffset;

    bool m_isSitting;

    std::string m_name;

    int m_x;
    int m_y;
    int m_z;

    int m_direction;

    bool m_movementReady;

    float m_movementSpeed;

    int m_hp;
    int m_mp;

    int m_gold;
    int m_platinum;

    int m_exp;

    int m_level;
    int m_magicLevel;

    int m_cap;

    bool m_isAnimated;

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
    sf::Time m_timeMovement;

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        if (m_hasOutfit == true)
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
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
        m_drawOffset = 8;

        m_z = 0;

        m_direction = 2;

        m_outfitHead = 0;
        m_outfitBody = 0;
        m_outfitLegs = 0;
        m_outfitFeet = 0;
    }

    void setCoords(int x, int y)
    {
        setX(x);
        setY(y);
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
        m_spriteOutfitHead.setId(tibia::outfits::head[(m_outfitHead * 4) + m_direction]);
        m_spriteOutfitBody.setId(tibia::outfits::body[(m_outfitBody * 4) + m_direction]);
        m_spriteOutfitLegs.setId(tibia::outfits::legs[(m_outfitLegs * 4) + m_direction]);
        m_spriteOutfitFeet.setId(tibia::outfits::feet[(m_outfitFeet * 4) + m_direction]);
    }

    void update()
    {
        m_drawX = m_x * tibia::TILE_SIZE;
        m_drawY = m_y * tibia::TILE_SIZE;

        setPosition(m_drawX - m_drawOffset, m_drawY - m_drawOffset);

        updateOutfit();
    }

    int getDrawX()
    {
        return m_drawX;
    }

    void setDrawX(int x)
    {
        m_drawX = x;
    }

    int getDrawY()
    {
        return m_drawY;
    }

    void setDrawY(int y)
    {
        m_drawY = y;
    }

    int getDrawOffset()
    {
        return m_drawOffset;
    }

    void setDrawOffset(int offset)
    {
        m_drawOffset = offset;
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

private:

    int m_drawX;
    int m_drawY;

    int m_drawOffset;

    int m_x;
    int m_y;
    int m_z;

    int m_direction;

    int m_hp;
    int m_mp;

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
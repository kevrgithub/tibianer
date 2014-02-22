#ifndef TIBIA_SPRITE_HPP
#define TIBIA_SPRITE_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"

namespace tibia
{

class Sprite : public sf::Sprite
{

public:

    Sprite::Sprite()
    {
        setTexture(tibia::Textures::sprites);

        setTextureRect(sf::IntRect(0, 0, tibia::TILE_SIZE, tibia::TILE_SIZE));
    }

    Sprite::Sprite(unsigned int id)
    {
        setTexture(tibia::Textures::sprites);

        setId(id);
    }

    void setId(unsigned int id)
    {
        m_id = id;

        setTextureRect(tibia::getSpriteRectById(m_id));
    }

    unsigned int getId()
    {
        return m_id;
    }

private:

    unsigned int m_id;

};

}

#endif // TIBIA_SPRITE_HPP
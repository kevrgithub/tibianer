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
    }

    Sprite::Sprite(int id)
    {
        setTexture(tibia::Textures::sprites);
        setTextureRect(tibia::getSpriteRectById(id));
    }

    void setId(int id)
    {
        setTextureRect(tibia::getSpriteRectById(id));
    }

};

}

#endif // TIBIA_SPRITE_HPP
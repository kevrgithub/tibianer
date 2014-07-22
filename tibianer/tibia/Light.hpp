#ifndef TIBIA_LIGHT_HPP
#define TIBIA_LIGHT_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"

namespace tibia
{

class Light : public sf::Sprite
{

public:

    Light::Light()
    {
        setTexture(tibia::Textures::lights);

        setSize(0);

        setColor(tibia::Colors::light);

        setOrigin(tibia::Lights::size.x / 2, tibia::Lights::size.y / 2);
    }

    void setSize(unsigned int size)
    {
        if (size > tibia::Lights::numLights - 1)
        {
            size = tibia::Lights::numLights;
        }

        m_size = size;

        setTextureRect(sf::IntRect(0, size * tibia::Lights::size.y, tibia::Lights::size.x, tibia::Lights::size.y));
    }

    unsigned int getSize()
    {
        return m_size;
    }

    void setColorbyId(unsigned int id)
    {
        auto lightColorIt = umapLightColors.find(id);

        if (lightColorIt != umapLightColors.end())
        {
            setColor(lightColorIt->second);
        }
        else
        {
            setColor(tibia::Colors::light);
        }
    }

private:

    unsigned int m_size;

};

} // tibia

#endif // TIBIA_LIGHT_HPP

#ifndef TIBIA_BAR_HPP
#define TIBIA_BAR_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"

namespace tibia
{

class Bar : public sf::Sprite
{

public:

    Bar::Bar()
    {
        setTexture(tibia::Textures::bars);
    }

    void setValues(int type, int currentValue, int maxValue)
    {
        m_type = type;

        setTextureRect
        (
            sf::IntRect
            (
                0,
                type * tibia::BarsData::height,
                currentValue * tibia::BarsData::width / maxValue,
                tibia::BarsData::height
            )
        );
    }

    int getType()
    {
        return m_type;
    }

private:

    int m_type;

}; // class Bar

} // namespace tibia

#endif // TIBIA_BAR_HPP

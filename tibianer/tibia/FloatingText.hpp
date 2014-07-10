#ifndef TIBIA_FLOATINGTEXT_HPP
#define TIBIA_FLOATINGTEXT_HPP

#include <vector>
#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/BitmapFont.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/DrawableAndTransformable.hpp"

namespace tibia
{

class FloatingText : public tibia::DrawableAndTransformable
{

public:

    void setText(tibia::BitmapFont& bitmapFont, sf::Vector2u tileCoords, int z, std::string text, sf::Color textColor)
    {
        m_texture = bitmapFont.getTexture();

        m_z = z;

        m_textPosition = static_cast<sf::Vector2f>(tileCoords);

        m_textPosition.x += tibia::TILE_SIZE / 2;
        //m_textPosition.y += tibia::TILE_SIZE / 2;

        m_bitmapFontText.setText(&bitmapFont, text, textColor, true);
        m_bitmapFontText.setPosition(m_textPosition);
    }

    int getZ()
    {
        return m_z;
    }

    void update()
    {
        m_textPosition.y -= tibia::FloatingTextData::speed;

        m_bitmapFontText.setPosition(m_textPosition);
    }

    sf::Clock* getClock()
    {
        return &m_clock;
    }

private:

    sf::Texture* m_texture;

    int m_z;

    tibia::BitmapFontText m_bitmapFontText;

    sf::Vector2f m_textPosition;

    sf::Clock m_clock;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = m_texture;

        target.draw(m_bitmapFontText, states);
    }

};

} // tibia

#endif // TIBIA_FLOATINGTEXT_HPP

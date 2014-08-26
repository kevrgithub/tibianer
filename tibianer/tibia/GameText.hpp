#ifndef TIBIA_GAMETEXT_HPP
#define TIBIA_GAMETEXT_HPP

#include <vector>
#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/BitmapFont.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/DrawableAndTransformable.hpp"

namespace tibia
{

class GameText : public tibia::DrawableAndTransformable
{

public:

    void setText(tibia::BitmapFont& bitmapFont, sf::Vector2u tileCoords, int z, std::string text, sf::Color textColor)
    {
        m_texture = bitmapFont.getTexture();

        m_tilePosition = tileCoords;

        m_z = z;

        m_text = text;

        m_textColor = textColor;

        m_numTextLines = 0;

        m_textLines.clear();

        boost::split(m_textLines, text, boost::is_any_of("\n"));

        m_numTextLines = m_textLines.size();

        sf::Vector2f textPosition = static_cast<sf::Vector2f>(tileCoords);

        int textHeight = bitmapFont.getGlyphSize()->y;

        m_bitmapFontTextList.clear();

        for (auto& textLine : m_textLines)
        {
            tibia::BitmapFontText bitmapFontText;

            bitmapFontText.setText(&bitmapFont, textLine, textColor, true);

            bitmapFontText.setPosition(textPosition);

            textPosition.y = textPosition.y + textHeight + 1;

            m_bitmapFontTextList.push_back(bitmapFontText);
        }
    }

    int getZ()
    {
        return m_z;
    }

    sf::Vector2u getTilePosition()
    {
        return m_tilePosition;
    }

    std::string getText()
    {
        return m_text;
    }

    sf::Color* getTextColor()
    {
        return &m_textColor;
    }

    int getNumTextLines()
    {
        return m_numTextLines;
    }

    std::vector<std::string>* getTextLines()
    {
        return &m_textLines;
    }

    sf::Clock* getClock()
    {
        return &m_clock;
    }

private:

    sf::Texture* m_texture;

    sf::Vector2u m_tilePosition;

    std::string m_text;

    sf::Color m_textColor;

    int m_z;

    int m_type;

    int m_numTextLines;

    std::vector<std::string> m_textLines;

    std::vector<tibia::BitmapFontText> m_bitmapFontTextList;

    sf::Clock m_clock;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = m_texture;

        for (auto& bitmapFontText : m_bitmapFontTextList)
        {
            target.draw(bitmapFontText, states);
        }
    }

}; // class GameText

} // namespace tibia

#endif // TIBIA_GAMETEXT_HPP

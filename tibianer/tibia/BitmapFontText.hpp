#ifndef TIBIA_BITMAPFONTTEXT_HPP
#define TIBIA_BITMAPFONTTEXT_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "tibia/BitmapFont.hpp"

namespace tibia
{

class BitmapFontText : public sf::Drawable, public sf::Transformable
{

public:

    BitmapFontText::BitmapFontText()
    {
        m_vertexArray.setPrimitiveType(sf::Quads);
    }

    void setText(tibia::BitmapFont* bf, std::string text, sf::Color textColor, bool isCentered = false)
    {
        m_bitmapFont = bf;

        m_vertexArray.clear();

        m_vertexArray.resize(text.size() * 4);

        int x = 0;
        int y = 0;

        int textWidth = 0;

        for (std::size_t i = 0; i < text.size(); i++)
        {
            int asciiValue = static_cast<int>(text[i]);

            // skip unused characters
            if (asciiValue < 32 || asciiValue > 127)
            {
                continue;
            }

            // first 32 ascii characters skipped, need to offset the value
            asciiValue = asciiValue - 32;

            int tu = asciiValue % (bf->getTexture()->getSize().x / bf->getGlyphSize()->x);
            int tv = asciiValue / (bf->getTexture()->getSize().x / bf->getGlyphSize()->x);

            sf::Vertex* quad = &m_vertexArray[i * 4];

            quad[0].position = sf::Vector2f(x                        , y);
            quad[1].position = sf::Vector2f(x + bf->getGlyphSize()->x, y);
            quad[2].position = sf::Vector2f(x + bf->getGlyphSize()->x, y + bf->getGlyphSize()->y);
            quad[3].position = sf::Vector2f(x                        , y + bf->getGlyphSize()->y);

            quad[0].texCoords = sf::Vector2f(tu       * bf->getGlyphSize()->x, tv       * bf->getGlyphSize()->y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * bf->getGlyphSize()->x, tv       * bf->getGlyphSize()->y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * bf->getGlyphSize()->x, (tv + 1) * bf->getGlyphSize()->y);
            quad[3].texCoords = sf::Vector2f(tu       * bf->getGlyphSize()->x, (tv + 1) * bf->getGlyphSize()->y);

            quad[0].color = textColor;
            quad[1].color = textColor;
            quad[2].color = textColor;
            quad[3].color = textColor;

            int glyphWidth = bf->getGlyphWidths()->at(asciiValue); //bf->getGlyphSize()->x;

            x +=         glyphWidth + bf->getGlpyhSpace();

            textWidth += glyphWidth + bf->getGlpyhSpace();
        }

        if (isCentered == true)
        {
            for (std::size_t i = 0; i < m_vertexArray.getVertexCount(); i++)
            {
                m_vertexArray[i].position.x = m_vertexArray[i].position.x - (textWidth / 2);
            }
        }
    }

    sf::VertexArray* getVertexArray()
    {
        return &m_vertexArray;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = m_bitmapFont->getTexture();

        target.draw(m_vertexArray, states);
    }

    sf::VertexArray m_vertexArray;
    tibia::BitmapFont* m_bitmapFont;

}; // class BitmapFontText

} // namespace tibia

#endif // TIBIA_BITMAPFONTTEXT_HPP

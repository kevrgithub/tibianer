#ifndef TIBIA_BITMAPFONT_HPP
#define TIBIA_BITMAPFONT_HPP

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace tibia
{

class BitmapFont
{

public:

    BitmapFont::BitmapFont()
    {
        m_vertexArray.setPrimitiveType(sf::Quads);
        m_vertexArray.resize(m_numGlyphs * 4);
    }

    bool load(const std::string& texture, sf::Vector2u glyphSize, std::vector<int>* glyphWidths, int glpyhSpace = 0)
    {
        m_glyphSize = glyphSize;

        m_glyphWidths = *glyphWidths;

        m_glpyhSpace = glpyhSpace;

        if (m_texture.loadFromFile(texture) == false)
        {
            return false;
        }

        for (int i = 0; i < m_numGlyphs; i++)
        {
            int tu = i % (m_texture.getSize().x / glyphSize.x);
            int tv = i / (m_texture.getSize().x / glyphSize.x);

            sf::Vertex* quad = &m_vertexArray[i * 4];

            quad[0].position = sf::Vector2f(i       * glyphSize.x, 1       * glyphSize.y);
            quad[1].position = sf::Vector2f((i + 1) * glyphSize.x, 1       * glyphSize.y);
            quad[2].position = sf::Vector2f((i + 1) * glyphSize.x, (1 + 1) * glyphSize.y);
            quad[3].position = sf::Vector2f(i       * glyphSize.x, (1 + 1) * glyphSize.y);

            quad[0].texCoords = sf::Vector2f(tu       * glyphSize.x, tv       * glyphSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * glyphSize.x, tv       * glyphSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * glyphSize.x, (tv + 1) * glyphSize.y);
            quad[3].texCoords = sf::Vector2f(tu       * glyphSize.x, (tv + 1) * glyphSize.y);
        }

        return true;
    }

    sf::VertexArray* getVertexArray()
    {
        return &m_vertexArray;
    }

    sf::Texture* getTexture()
    {
        return &m_texture;
    }

    sf::Vector2u* getGlyphSize()
    {
        return &m_glyphSize;
    }

    std::vector<int>* getGlyphWidths()
    {
        return &m_glyphWidths;
    }

    int getGlpyhSpace()
    {
        return m_glpyhSpace;
    }

private:

    sf::VertexArray m_vertexArray;
    sf::Texture m_texture;
    sf::Vector2u m_glyphSize;
    std::vector<int> m_glyphWidths;

    int m_numGlyphs = 96;

    int m_glpyhSpace;

}; // class BitmapFont

} // namespace tibia

#endif // TIBIA_BITMAPFONT_HPP

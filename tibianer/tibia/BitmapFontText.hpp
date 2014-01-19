#include <string>

#include <boost/algorithm/string.hpp>

#include <SFML/Graphics.hpp>

namespace tibia
{

class BitmapFontText : public sf::Drawable, public sf::Transformable
{

public:

    bool load(sf::Color textColor, std::string text, const std::string& texture, sf::Vector2u characterSize, bool useFullAsciiSet)
    {
        if (!m_texture.loadFromFile(texture))
        {
            return false;
        }

        boost::replace_all(text, "\n", "\x7F");

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(text.size() * 4);

        int x = 0;
        int y = 0;

        for (unsigned int i = 0; i < text.size(); i++)
        {
            int asciiValue = static_cast<int>(text[i]);

            if (useFullAsciiSet == false)
            {
                if (asciiValue != 127)
                {
                    asciiValue = asciiValue - 32;
                }
            }

            int tu = asciiValue % (m_texture.getSize().x / characterSize.x);
            int tv = asciiValue / (m_texture.getSize().x / characterSize.x);

            sf::Vertex* quad = &m_vertices[i * 4];

            // new line
            if (asciiValue == 127)
            {
                x = 0;
                y += characterSize.y;

                continue;
            }

            quad[0].position = sf::Vector2f(x                  ,     characterSize.y + y);
            quad[1].position = sf::Vector2f(x + characterSize.x,     characterSize.y + y);
            quad[2].position = sf::Vector2f(x + characterSize.x, 2 * characterSize.y + y);
            quad[3].position = sf::Vector2f(x                  , 2 * characterSize.y + y);

            quad[0].texCoords = sf::Vector2f(tu       * characterSize.x, tv       * characterSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * characterSize.x, tv       * characterSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * characterSize.x, (tv + 1) * characterSize.y);
            quad[3].texCoords = sf::Vector2f(tu       * characterSize.x, (tv + 1) * characterSize.y);

            quad[0].color = textColor;
            quad[1].color = textColor;
            quad[2].color = textColor;
            quad[3].color = textColor;

            //quad[0].position = sf::Vector2f(i       * characterSize.x, 1       * characterSize.y);
            //quad[1].position = sf::Vector2f((i + 1) * characterSize.x, 1       * characterSize.y);
            //quad[2].position = sf::Vector2f((i + 1) * characterSize.x, (1 + 1) * characterSize.y);
            //quad[3].position = sf::Vector2f(i       * characterSize.x, (1 + 1) * characterSize.y);

            //quad[0].texCoords = sf::Vector2f(tu       * characterSize.x, tv       * characterSize.y);
            //quad[1].texCoords = sf::Vector2f((tu + 1) * characterSize.x, tv       * characterSize.y);
            //quad[2].texCoords = sf::Vector2f((tu + 1) * characterSize.x, (tv + 1) * characterSize.y);
            //quad[3].texCoords = sf::Vector2f(tu       * characterSize.x, (tv + 1) * characterSize.y);

            x += characterSize.x;
        }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        states.texture = &m_texture;

        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_texture;
};

}
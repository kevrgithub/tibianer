#ifndef TIBIA_TEXT_HPP
#define TIBIA_TEXT_HPP

#include <string>

#include <SFML/Graphics.hpp>

namespace tibia
{

class Text : public sf::Drawable, public sf::Transformable
{

public:

    void load(sf::Text)
    {
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //states.transform *= getTransform();

        //target.draw(m_text);

        //target.draw(m, states);
    }

};

}

#endif // TIBIA_TEXT_HPP

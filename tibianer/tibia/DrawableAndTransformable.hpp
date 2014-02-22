#ifndef TIBIA_DRAWABLEANDTRANSFORMABLE_HPP
#define TIBIA_DRAWABLEANDTRANSFORMABLE_HPP

#include <SFML/Graphics.hpp>

namespace tibia
{

class DrawableAndTransformable : public sf::Drawable, public sf::Transformable
{

public:

    DrawableAndTransformable::DrawableAndTransformable()
    {
        //
    }

};

}

#endif // TIBIA_DRAWABLEANDTRANSFORMABLE_HPP
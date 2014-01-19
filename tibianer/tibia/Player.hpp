#ifndef TIBIA_PLAYER_HPP
#define TIBIA_PLAYER_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Player : public tibia::Creature
{

public:

    Player::Player()
    {
        setHasOutfit(true);
    }

};

}

#endif // TIBIA_PLAYER_HPP
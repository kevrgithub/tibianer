#ifndef TIBIA_SOUND_HPP
#define TIBIA_SOUND_HPP

#include <SFML/Audio.hpp>

#include "tibia/Tibia.hpp"

namespace tibia
{

class Sound : public sf::Sound
{

public:

    typedef std::shared_ptr<tibia::Sound> Ptr;
    typedef std::vector<tibia::Sound::Ptr> List;

}; // class Sound

} // namespace tibia

#endif // TIBIA_SPRITE_HPP

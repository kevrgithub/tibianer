#ifndef TIBIA_OBJECT_HPP
#define TIBIA_OBJECT_HPP

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"
#include "tibia/Script.hpp"

namespace tibia
{

class Object : public tibia::Thing
{

public:

    typedef std::shared_ptr<tibia::Object> Ptr;
    typedef std::vector<tibia::Object::Ptr> List;

    struct SortById_t
    {
        bool operator()(tibia::Object::Ptr a, tibia::Object::Ptr b) const
        {
            return a->getId() < b->getId();
        }
    };

    struct SortByCount_t
    {
        bool operator()(tibia::Object::Ptr a, tibia::Object::Ptr b) const
        {
            return a->getCount() > b->getCount();
        }
    };

    struct Properties_t
    {
        unsigned int uniqueId = 0;

        std::string signName;
        std::string signText;

        std::string bookName;
        std::string bookText;

        int doorKeyType = tibia::KeyTypes::none;
        unsigned int doorKeyId = 0;

        unsigned int teleporterX = 0;
        unsigned int teleporterY = 0;
        unsigned int teleporterZ = tibia::ZAxis::default;

        unsigned int bedX = 0;
        unsigned int bedY = 0;
        unsigned int bedDirection = tibia::Directions::null;

        std::string changeMapName;

        tibia::Script doScript;
        tibia::Script onInteractScript;

        tibia::Script leverScriptOn;
        tibia::Script leverScriptOff;

        tibia::Script stepTileScriptOnStartTouch;
        tibia::Script stepTileScriptOnStopTouch;
    };

    Properties_t properties;

    Object::Object()
    {
        setIsObject(true);
    }

    Object::Object(sf::Vector2u tileCoords, int z, int id)
    {
        setIsObject(true);

        setTileCoords(tileCoords.x, tileCoords.y);

        setZ(z);

        setId(id);

        if (m_flags.test(tibia::SpriteFlags::groupable))
        {
            setCountById();
        }
    }

    void setExtraSprites()
    {
        for (unsigned int i = 1; i < tibia::NUM_OBJECT_SPRITES - 1; ++i)
        {
            m_shouldDrawExtraSprite[i] = false;
        }

        // brick wall arch
        if (m_id == 16)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);
            m_sprite[3].setId(m_id + 1);
            m_sprite[4].setId(m_id + 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE * 2,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[4].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE * 2,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;
            m_shouldDrawExtraSprite[4] = true;

            return;
        }

        // wood rail horizontal
        if (m_id == 183)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // wood rail vertical
        if (m_id == 187)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // long painting horizontal
        if (m_id == 3691)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - (tibia::TILE_SIZE * 2),
                    m_sprite[0].getPosition().y
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // long painting vertical
        if (m_id == 3688)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - (tibia::TILE_SIZE * 2)
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // metal gate horizontal
        if (m_id == 3694)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // metal gate vertical
        if (m_id == 3697)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE 
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // metal fence horizontal
        if (m_id == 3700)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // metal fence vertical
        if (m_id == 3703)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE 
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // well
        else if (m_id == 315)
        {
            m_sprite[4].setId(m_id - 4);

            m_sprite[4].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE * 2,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[4] = true;

            //return // do not return because well is a quadObject with a 5th sprite
        }

        // doors
        else if (m_id == 555 || m_id == 558 || m_id == 3295 || m_id == 3299)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(54);
            m_sprite[3].setId(53);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );


            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;

            return;
        }
        else if (m_id == 549)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(57);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );


            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // mill bread grinder
        else if (m_id == 906 || m_id == 915)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 3);
            m_sprite[3].setId(m_id - 4);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );


            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;

            return;
        }
        else if (m_id == 907 || m_id == 916)
        {
            m_sprite[1].setId(m_id - 3);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );


            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // mill wooden wheel
        else if (m_id == 895 || m_id == 901)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);
            m_sprite[3].setId(m_id - 3);
            m_sprite[4].setId(m_id - 4);
            m_sprite[5].setId(m_id - 5);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - (tibia::TILE_SIZE * 2),
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[4].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[5].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - (tibia::TILE_SIZE * 2),
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;
            m_shouldDrawExtraSprite[4] = true;
            m_shouldDrawExtraSprite[5] = true;
        }

        // wooden arch vertical
        else if (m_id == 1659)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);
            m_sprite[3].setId(m_id - 3);
            m_sprite[4].setId(m_id - 4);
            m_sprite[5].setId(m_id - 5);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[4].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - (tibia::TILE_SIZE * 2)
                )
            );

            m_sprite[5].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - (tibia::TILE_SIZE * 2)
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;
            m_shouldDrawExtraSprite[4] = true;
            m_shouldDrawExtraSprite[5] = true;
        }

        // wooden arch horizontal
        else if (m_id == 1665)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 3);
            m_sprite[3].setId(m_id - 4);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;
        }
        else if (m_id == 1663)
        {
            m_sprite[1].setId(m_id - 3);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
        }

        // mountain ramps
        else if (m_id == 2097 || m_id == 2098 || m_id == 2102 || m_id == 2103)
        {
            m_sprite[1].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // wooden fence corner
        else if (m_id == 2714)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // wooden fence vertical
        else if (m_id == 2717)
        {
            m_sprite[1].setId(m_id - 5);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // wooden fence horizontal
        else if (m_id == 2718)
        {
            m_sprite[1].setId(m_id - 5);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // torch hanging horizontal
        if (m_id == 2968)
        {
            m_sprite[1].setId(m_id + 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x + tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }
        else if (m_id == 2970)
        {
            m_sprite[1].setId(m_id - 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x + tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }
        else if (m_id == 2971)
        {
            m_sprite[1].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x + tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // torch hanging vertical
        if (m_id == 2972)
        {
            m_sprite[1].setId(m_id + 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y + tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }
        else if (m_id == 2974)
        {
            m_sprite[1].setId(m_id - 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y + tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }
        else if (m_id == 2975)
        {
            m_sprite[1].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y + tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // tall sign vertical
        else if (m_id == 3310)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // tall sign horizontal
        else if (m_id == 3313)
        {
            m_sprite[1].setId(m_id - 1);
            m_sprite[2].setId(m_id - 2);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }

        // right to left, bottom then top
        for (auto spriteId : tibia::SpriteData::quadObjects)
        {
            if (m_id == spriteId)
            {
                m_sprite[1].setId(m_id - 1);
                m_sprite[2].setId(m_id - 2);
                m_sprite[3].setId(m_id - 3);

                m_sprite[1].setPosition
                (
                    sf::Vector2f
                    (
                        m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                        m_sprite[0].getPosition().y
                    )
                );

                m_sprite[2].setPosition
                (
                    sf::Vector2f
                    (
                        m_sprite[0].getPosition().x,
                        m_sprite[0].getPosition().y - tibia::TILE_SIZE
                    )
                );

                m_sprite[3].setPosition
                (
                    sf::Vector2f
                    (
                        m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                        m_sprite[0].getPosition().y - tibia::TILE_SIZE
                    )
                );

                m_shouldDrawExtraSprite[1] = true;
                m_shouldDrawExtraSprite[2] = true;
                m_shouldDrawExtraSprite[3] = true;

                return;
            }
        }

        // quad object brick wall that is irregular, sprites 1 and 2 are backwards
        if (m_id == 25)
        {
            m_sprite[1].setId(m_id - 2);
            m_sprite[2].setId(m_id - 1);
            m_sprite[3].setId(m_id - 3);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_sprite[3].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;
            m_shouldDrawExtraSprite[3] = true;

            return;
        }

        // right to left
        for (auto spriteId : tibia::SpriteData::horizontalObjects)
        {
            if (m_id == spriteId)
            {
                m_sprite[1].setId(m_id - 1);

                m_sprite[1].setPosition
                (
                    sf::Vector2f
                    (
                        m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                        m_sprite[0].getPosition().y
                    )
                );

                m_shouldDrawExtraSprite[1] = true;

                return;
            }
        }

        // bottom to top
        for (auto spriteId : tibia::SpriteData::verticalObjects)
        {
            if (m_id == spriteId)
            {
                m_sprite[1].setId(m_id - 1);

                m_sprite[1].setPosition
                (
                    sf::Vector2f
                    (
                        m_sprite[0].getPosition().x,
                        m_sprite[0].getPosition().y - tibia::TILE_SIZE
                    )
                );

                m_shouldDrawExtraSprite[1] = true;

                return;
            }
        }

        // vertical object mountain wall that is irregular, sprite order is backwards
        if (m_id == 1068)
        {
            m_sprite[1].setId(m_id + 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;

            return;
        }

        // mountain wall corner piece
        if (m_id == 1070)
        {
            m_sprite[1].setId(m_id - 4);
            m_sprite[2].setId(m_id - 1);

            m_sprite[1].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x - tibia::TILE_SIZE,
                    m_sprite[0].getPosition().y
                )
            );

            m_sprite[2].setPosition
            (
                sf::Vector2f
                (
                    m_sprite[0].getPosition().x,
                    m_sprite[0].getPosition().y - tibia::TILE_SIZE
                )
            );

            m_shouldDrawExtraSprite[1] = true;
            m_shouldDrawExtraSprite[2] = true;

            return;
        }
    }

    void doAnimation()
    {
        for (auto& animatedObjects : tibia::animatedObjectsList)
        {
            auto animatedObjectIt = std::find(animatedObjects.begin(), animatedObjects.end(), m_id);

            if (animatedObjectIt != animatedObjects.end())
            {
                animatedObjectIt++;

                if (animatedObjectIt == animatedObjects.end())
                {
                    animatedObjectIt = animatedObjects.begin();
                }

                setId(*animatedObjectIt);

                return;
            }
        }
    }

    void doDecay()
    {
        for (auto& decayObjects : tibia::decayObjectsList)
        {
            auto decayObjectIt = std::find(decayObjects.begin(), decayObjects.end(), m_id);

            if (decayObjectIt != decayObjects.end())
            {
                decayObjectIt++;

                if (decayObjectIt == decayObjects.end())
                {
                    setIsReadyForErase(true);
                    return;
                }

                setId(*decayObjectIt);

                m_clockDecay.restart();

                return;
            }
        }
    }

    void doRegenerate()
    {
        if (m_id == tibia::SpriteData::bushBlueBerryEmpty)
        {
            setId(tibia::SpriteData::bushBlueBerry);
        }
        if (m_id == tibia::SpriteData::snowBallMoundEmpty)
        {
            setId(tibia::SpriteData::snowBallMound);
        }
        else if (m_id == tibia::SpriteData::grassJungleCut)
        {
            setId(tibia::SpriteData::grassJungle);
        }
        else if (m_id == tibia::SpriteData::wheatCut)
        {
            setId(tibia::SpriteData::wheatGreen);
        }
        else if (m_id == tibia::SpriteData::wheatGreen)
        {
            setId(tibia::SpriteData::wheatYellow);
        }
        else if (m_id == tibia::SpriteData::digHole[1])
        {
            setId(tibia::SpriteData::digHole[0]);
        }
        else if (m_id == tibia::SpriteData::digHoleIce[1])
        {
            setId(tibia::SpriteData::digHoleIce[0]);
        }

        m_clockRegenerate.restart();
    }

    void setId(int id)
    {
        m_id = id;

        m_sprite[0].setId(m_id);

        m_flags = tibia::UMaps::spriteFlags[m_id];

        if (m_flags.test(tibia::SpriteFlags::tileEdge))
        {
            setDrawIndex(tibia::DRAW_INDEX_TILE_EDGE);
        }

        if (m_flags.test(tibia::SpriteFlags::decal))
        {
            setDrawIndex(tibia::DRAW_INDEX_DECAL);
        }

        if (m_flags.test(tibia::SpriteFlags::drawLast))
        {
            setDrawIndex(tibia::DRAW_INDEX_LAST);
        }

        if
        (
            m_flags.test(tibia::SpriteFlags::tileEdge) == false &&
            m_flags.test(tibia::SpriteFlags::decal)    == false &&
            m_flags.test(tibia::SpriteFlags::drawLast) == false
        )
        {
            setDrawIndex(tibia::DRAW_INDEX_DEFAULT);
        }

        if (m_flags.test(tibia::SpriteFlags::transparent))
        {
            m_sprite[0].setColor(sf::Color::Transparent);
        }
        else
        {
            m_sprite[0].setColor(sf::Color::White);
        }

        if (m_flags.test(tibia::SpriteFlags::offset))
        {
            m_sprite[0].setPosition
            (
                -tibia::THING_DRAW_OFFSET,
                -tibia::THING_DRAW_OFFSET
            );
        }

        setExtraSprites();
    }

    int getId()
    {
        return m_id;
    }

    void setCountById()
    {
        for (auto& groupableObjects : tibia::groupedObjectsList)
        {
            auto groupableObjectIt = std::find(groupableObjects.begin(), groupableObjects.end(), m_id);

            if (groupableObjectIt != groupableObjects.end())
            {
                m_count = tibia::Utility::getCountByGroupableObjectIndex(groupableObjectIt - groupableObjects.begin(), groupableObjects.size());
                return;
            }
        }
    }

    void setCount(int count)
    {
        m_count = count;
    }

    int getCount()
    {
        return m_count;
    }

    int getType()
    {
        return m_type;
    }

    void setType(int type)
    {
        m_type = type;
    }

    sf::Sprite* getSprite()
    {
        return m_sprite;
    }

    tibia::SpriteFlags_t getFlags()
    {
        return m_flags;
    }

    void setFlags(tibia::SpriteFlags_t flags)
    {
        m_flags = flags;
    }

    void setIsDecay(bool b)
    {
        m_isDecay = b;

        m_clockDecay.restart();
    }

    bool isDecay()
    {
        return m_isDecay;
    }

    sf::Clock* getClockDecay()
    {
        return &m_clockDecay;
    }

    sf::Clock* getClockRegenerate()
    {
        return &m_clockRegenerate;
    }

    void update()
    {
        updateTileCoords();

        int drawOffset = getDrawOffset() * tibia::THING_DRAW_OFFSET;

        setPosition(getTileX() - drawOffset, getTileY() - drawOffset);
    }

private:

    int m_id;

    int m_type;

    int m_count = 1;

    tibia::Sprite m_sprite[tibia::NUM_OBJECT_SPRITES];

    bool m_shouldDrawExtraSprite[tibia::NUM_OBJECT_SPRITES];

    tibia::SpriteFlags_t m_flags;

    bool m_isDecay;

    sf::Clock m_clockDecay;

    sf::Clock m_clockRegenerate;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite[0], states);

        for (unsigned int i = 1; i < tibia::NUM_OBJECT_SPRITES - 1; ++i)
        {
            if (m_shouldDrawExtraSprite[i] == true)
            {
                target.draw(m_sprite[i], states);
            }
        }
    }

}; // class Object

} // namespace tibia

#endif // TIBIA_OBJECT_HPP

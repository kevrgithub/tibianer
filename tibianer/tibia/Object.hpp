#ifndef TIBIA_OBJECT_HPP
#define TIBIA_OBJECT_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Object : public tibia::Thing
{

public:

    typedef std::shared_ptr<tibia::Object> Ptr;
    typedef std::vector<tibia::Object::Ptr> List;

    struct Properties_t
    {
        std::string signName = "a sign";
        std::string signText;

        std::string bookName = "a book";
        std::string bookText;

        unsigned int teleporterX;
        unsigned int teleporterY;
        unsigned int teleporterZ;

        std::string changeMapName;
    };

    Properties_t properties;

    Object::Object(sf::Vector2u tileCoords, int z, int id)
    {
        setTileCoords(tileCoords.x, tileCoords.y);

        setZ(z);

        setId(id);
    }

    void setIsOffset()
    {
        m_isOffset = false;

        if (m_flags & tibia::SpriteFlags::offset)
        {
            m_isOffset = true;
        }

        if (m_isOffset == true)
        {
            m_sprite[0].setPosition
            (
                -tibia::THING_DRAW_OFFSET,
                -tibia::THING_DRAW_OFFSET
            );
        }
    }

    void setIsAnimated()
    {
        m_isAnimated = false;

        if (m_flags & tibia::SpriteFlags::animated)
        {
            m_isAnimated = true;
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
        bool animationIsDone = false;

        for (unsigned int i = 0; i < tibia::animatedObjectsList.size(); i++)
        {
            if (animationIsDone == true)
            {
                break;
            }

            std::vector<int> sprites = tibia::animatedObjectsList.at(i);

            for (unsigned int j = 0; j < sprites.size(); j++)
            {
                int spriteId = sprites.at(j);

                if (m_id == spriteId)
                {
                    j++;

                    if (j > sprites.size() - 1)
                    {
                        j = 0;
                    }

                    spriteId = sprites.at(j);

                    setId(spriteId);

                    animationIsDone = true;

                    break;
                }
            }
        }
    }

    void doDecay()
    {
        bool decayIsDone = false;

        for (unsigned int i = 0; i < tibia::decayObjectsList.size(); i++)
        {
            if (decayIsDone == true)
            {
                break;
            }

            std::vector<int> sprites = tibia::decayObjectsList.at(i);

            for (unsigned int j = 0; j < sprites.size(); j++)
            {
                int spriteId = sprites.at(j);

                if (m_id == spriteId)
                {
                    j++;

                    if (j > sprites.size() - 1)
                    {
                        setIsReadyForErase(true);
                        return;
                    }

                    spriteId = sprites.at(j);

                    setId(spriteId);

                    m_clockDecay.restart();

                    decayIsDone = true;

                    break;
                }
            }
        }
    }

    void setId(int id)
    {
        m_id = id;

        m_sprite[0].setId(m_id);

        m_flags = tibia::umapSpriteFlags[m_id];

        if (m_flags & tibia::SpriteFlags::drawLast)
        {
            setDrawIndex(tibia::DRAW_INDEX_LAST);
        }

        if (m_flags & tibia::SpriteFlags::transparent)
        {
            m_sprite[0].setColor(tibia::Colors::transparent);
        }

        setIsAnimated();

        setIsOffset();

        setExtraSprites();
    }

    int getId()
    {
        return m_id;
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

    unsigned int getFlags()
    {
        return m_flags;
    }

    void setFlags(unsigned int flags)
    {
        m_flags = flags;
    }

    bool isOffset()
    {
        return m_isOffset;
    }

    bool isAnimated()
    {
        return m_isAnimated;
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

    void update()
    {
        updateTileCoords();

        int drawOffset = getDrawOffset() * tibia::THING_DRAW_OFFSET;

        setPosition(getTileX() - drawOffset, getTileY() - drawOffset);
    }

private:

    int m_id;

    int m_type;

    tibia::Sprite m_sprite[tibia::NUM_OBJECT_SPRITES];

    bool m_shouldDrawExtraSprite[tibia::NUM_OBJECT_SPRITES];

    unsigned int m_flags;

    bool m_isOffset;

    bool m_isAnimated;

    bool m_isDecay;

    sf::Clock m_clockDecay;

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

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

    Object::Object(int tileX, int tileY, int z, int id)
    {
        setTileCoords(tileX, tileY);

        setZ(z);

        setId(id);

        setIsOffset();

        setIsAnimated();
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
                sf::Vector2f
                (
                    getPosition().x - tibia::THING_DRAW_OFFSET,
                    getPosition().y - tibia::THING_DRAW_OFFSET
                )
            );
        }
    }

    void setIsAnimated()
    {
        m_isAnimated = false;

        for (auto spriteId : tibia::SpriteData::animatedObjects)
        {
            if (spriteId == m_id)
            {
                m_isAnimated = true;
                break;
            }
        }
    }

    void setExtraSprites()
    {
        for (unsigned int i = 1; i < 8 - 1; ++i)
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

        // wall torch vertical
        else if (m_id == 2973)
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

        // bottom to top, right then left
        for (auto spriteId : tibia::SpriteData::quadVerticalObjects)
        {
            if (m_id == spriteId)
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

        m_flags = tibia::spriteFlags[m_id];

        if (m_flags & tibia::SpriteFlags::drawLast)
        {
            setDrawIndex(1);
        }

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

        setPosition(getTileX(), getTileY());
    }

    void setSignText(const std::string& text)
    {
        m_signText = text;
    }

    std::string getSignText()
    {
        return m_signText;
    }

    unsigned int getTeleporterX()
    {
        return m_teleporterX;
    }

    void setTeleporterX(unsigned int x)
    {
        m_teleporterX = x;
    }

    unsigned int getTeleporterY()
    {
        return m_teleporterY;
    }

    void setTeleporterY(unsigned int y)
    {
        m_teleporterY = y;
    }

    unsigned int getTeleporterZ()
    {
        return m_teleporterZ;
    }

    void setTeleporterZ(unsigned int z)
    {
        m_teleporterZ = z;
    }

private:

    int m_id;

    int m_type;

    tibia::Sprite m_sprite[8];

    bool m_shouldDrawExtraSprite[8];

    unsigned int m_flags;

    bool m_isOffset;

    bool m_isAnimated;

    bool m_isDecay;

    sf::Clock m_clockDecay;

    std::string m_signText;

    unsigned int m_teleporterX;
    unsigned int m_teleporterY;
    unsigned int m_teleporterZ;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite[0], states);

        for (unsigned int i = 1; i < 8 - 1; ++i)
        {
            if (m_shouldDrawExtraSprite[i] == true)
            {
                target.draw(m_sprite[i], states);
            }
        }
    }

};

typedef std::shared_ptr<tibia::Object> ObjectPtr;
typedef std::vector<ObjectPtr> ObjectList;

} // tibia

#endif // TIBIA_OBJECT_HPP

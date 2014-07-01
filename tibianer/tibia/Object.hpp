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
        m_shouldDrawExtraSprite1 = false;
        m_shouldDrawExtraSprite2 = false;
        m_shouldDrawExtraSprite3 = false;
        m_shouldDrawExtraSprite4 = false;

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

            m_shouldDrawExtraSprite1 = true;
            m_shouldDrawExtraSprite2 = true;
            m_shouldDrawExtraSprite3 = true;
            m_shouldDrawExtraSprite4 = true;

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

            m_shouldDrawExtraSprite4 = true;

            //return // do not return because well is a quadObject with a 5th sprite
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

            m_shouldDrawExtraSprite1 = true;

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

            m_shouldDrawExtraSprite1 = true;
            m_shouldDrawExtraSprite2 = true;

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

            m_shouldDrawExtraSprite1 = true;

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

            m_shouldDrawExtraSprite1 = true;

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

            m_shouldDrawExtraSprite1 = true;
            m_shouldDrawExtraSprite2 = true;

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

            m_shouldDrawExtraSprite1 = true;
            m_shouldDrawExtraSprite2 = true;

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

                m_shouldDrawExtraSprite1 = true;
                m_shouldDrawExtraSprite2 = true;
                m_shouldDrawExtraSprite3 = true;

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

                m_shouldDrawExtraSprite1 = true;
                m_shouldDrawExtraSprite2 = true;
                m_shouldDrawExtraSprite3 = true;

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

                m_shouldDrawExtraSprite1 = true;

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

                m_shouldDrawExtraSprite1 = true;

                return;
            }
        }
    }

    void doAnimation()
    {
        bool animationIsDone = false;

        for (unsigned int j = 0; j < tibia::animatedObjectsList.size(); j++)
        {
            if (animationIsDone == true)
            {
                break;
            }

            std::vector<int> sprites = tibia::animatedObjectsList.at(j);

            for (unsigned int n = 0; n < sprites.size(); n++)
            {
                int spriteId = sprites.at(n);

                if (m_id == spriteId)
                {
                    n++;

                    if (n > sprites.size() - 1)
                    {
                        n = 0;
                    }

                    spriteId = sprites.at(n);

                    setId(spriteId);

                    animationIsDone = true;

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

        setExtraSprites();
    }

    int getId()
    {
        return m_id;
    }

    sf::Sprite* getSprite()
    {
        return m_sprite;
    }

    int getFlags()
    {
        return m_flags;
    }

    void setFlags(int flags)
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

    void update()
    {
        updateTileCoords();

        setPosition(getTileX(), getTileY());
    }

private:

    int m_id;

    tibia::Sprite m_sprite[5];

    bool m_shouldDrawExtraSprite1;
    bool m_shouldDrawExtraSprite2;
    bool m_shouldDrawExtraSprite3;
    bool m_shouldDrawExtraSprite4;

    int m_flags;

    bool m_isOffset;

    bool m_isAnimated;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite[0], states);

        if (m_shouldDrawExtraSprite1 == true)
        {
            target.draw(m_sprite[1], states);
        }

        if (m_shouldDrawExtraSprite2 == true)
        {
            target.draw(m_sprite[2], states);
        }

        if (m_shouldDrawExtraSprite3 == true)
        {
            target.draw(m_sprite[3], states);
        }

        if (m_shouldDrawExtraSprite4 == true)
        {
            target.draw(m_sprite[4], states);
        }
    }

};

typedef std::shared_ptr<tibia::Object> ObjectPtr;
typedef std::vector<ObjectPtr> ObjectList;

} // tibia

#endif // TIBIA_OBJECT_HPP

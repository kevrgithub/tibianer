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

            setOffset();

            setAnimated();
        }

        void setOffset()
        {
            m_isOffset = false;

            for (auto spriteId : tibia::SpriteData::offsetObjects)
            {
                if (spriteId == m_id)
                {
                    m_isOffset = true;
                    break;
                }
            }

            if (m_isOffset == true)
            {
                m_sprite[0].setPosition
                (
                    sf::Vector2f
                    (
                        getPosition().x - tibia::TILE_DRAW_OFFSET,
                        getPosition().y - tibia::TILE_DRAW_OFFSET
                    )
                );
            }
        }

        void setAnimated()
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

            // well
            if (m_id == 315)
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

            // brick wall arch
            else if (m_id == 16)
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

            // right to left, bottom to top
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

            // bottom to top, right to left
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

        void setId(int id)
        {
            m_id = id;

            m_sprite[0].setId(m_id);

            setExtraSprites();
        }

        int getId()
        {
            return m_id;
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

}

#endif // TIBIA_OBJECT_HPP

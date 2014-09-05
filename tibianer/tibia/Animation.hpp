#ifndef TIBIA_ANIMATION_HPP
#define TIBIA_ANIMATION_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Animation : public tibia::Thing
{

public:

    typedef std::shared_ptr<tibia::Animation> Ptr;
    typedef std::vector<tibia::Animation::Ptr> List;

    Animation::Animation(int tileX, int tileY, int z, int id, int numFrames)
    {
        setTileCoords(tileX, tileY);
        setPosition(sf::Vector2f(tileX, tileY));

        setZ(z);

        setId(id);

        setNumFrames(numFrames);

        setFrameTime(tibia::AnimationTimes::default);

        m_clock.restart();
    }

    void setId(int id)
    {
        m_id = id;

        m_sprite.setId(m_id);

        m_flags = tibia::UMaps::spriteFlags[m_id];

        m_numRepeat = 0;

        if
        (
            id == tibia::Animations::fire[0]        ||
            id == tibia::Animations::electricity[0]
        )
        {
            setNumRepeat(1);
        }
    }

    int getId()
    {
        return m_id;
    }

     tibia::SpriteFlags_t getFlags()
    {
        return m_flags;
    }

    void setFlags( tibia::SpriteFlags_t flags)
    {
        m_flags = flags;
    }

    void setNumFrames(int numFrames)
    {
        m_numFrames = numFrames;

        m_currentFrame = 0;
    }

    int getNumFrames()
    {
        return m_numFrames;
    }

    void advanceFrame()
    {
        sf::Time timeElapsed = m_clock.getElapsedTime();

        if (timeElapsed.asSeconds() >= m_frameTime)
        {
            m_currentFrame++;

            if (m_currentFrame > m_numFrames - 1)
            {
                if (m_numRepeat > 0)
                {
                    m_currentFrame = 0;

                    m_numRepeat--;
                }
                else
                {
                    setIsReadyForErase(true);
                    return;
                }
            }

            m_sprite.setId(m_id + m_currentFrame);

            m_clock.restart();
        }
    }

    int getCurrentFrame()
    {
        return m_currentFrame;
    }

    void setNumRepeat(int numRepeat)
    {
        m_numRepeat = numRepeat;
    }

    int getNumRepeat()
    {
        return m_numRepeat;
    }

    void setFrameTime(float time)
    {
        m_frameTime = time;
    }

    float getFrameTime()
    {
        return m_frameTime;
    }

    void update()
    {
        updateTileCoords();

        int drawOffset = getDrawOffset() * tibia::THING_DRAW_OFFSET;

        setPosition(getTileX() - drawOffset, getTileY() - drawOffset);

        advanceFrame();
    }

private:

    int m_id;

    tibia::SpriteFlags_t m_flags;

    tibia::Sprite m_sprite;

    int m_numFrames;

    int m_currentFrame;

    float m_frameTime;

    int m_numRepeat;

    sf::Clock m_clock;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite, states);
    }

}; // class Animation

} // namespace tibia

#endif // TIBIA_ANIMATION_HPP

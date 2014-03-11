#ifndef TIBIA_ANIMATION_HPP
#define TIBIA_ANIMATION_HPP

#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Sprite.hpp"

namespace tibia
{

class Animation : public tibia::Thing
{

public:

    Animation::Animation(int tileX, int tileY, int z, int id, int numFrames)
    {
        setTileCoords(tileX, tileY);
        setPosition(sf::Vector2f(tileX, tileY));

        setZ(z);

        setId(id);

        setNumFrames(numFrames);

        setFrameTime(tibia::AnimationTimes::default);

        m_numRepeat = 0;

        m_clock.restart();
    }

    void setId(int id)
    {
        m_id = id;

        m_sprite.setId(m_id);
    }

    int getId()
    {
        return m_id;
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

                //return;
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

        setPosition(getTileX(), getTileY());

        advanceFrame();
    }

private:

    int m_id;

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

};

}


#endif // TIBIA_ANIMATION_HPP

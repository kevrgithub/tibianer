#ifndef TIBIA_UTILITY_HPP
#define TIBIA_UTILITY_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include "tibia/Tibia.hpp"

namespace tibia
{

    namespace Utility
    {

        sf::IntRect getSpriteRectById(int id)
        {
            id = id - 1;

            int u = (id % (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE)) * tibia::TILE_SIZE;
            int v = (id / (tibia::Textures::sprites.getSize().y / tibia::TILE_SIZE)) * tibia::TILE_SIZE;

            return sf::IntRect(u, v, tibia::TILE_SIZE, tibia::TILE_SIZE);
        }

        template <class T>
        int getTileNumberByTileCoords(T tileCoords)
        {
            int x = tileCoords.x / tibia::TILE_SIZE;
            int y = tileCoords.y / tibia::TILE_SIZE;

            return x + (y * tibia::MAP_SIZE);
        }

        bool isTileNumberOutOfBounds(int tileNumber)
        {
            if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
            {
                return true;
            }

            return false;
        }

        bool isTilePositionOutOfBounds(const sf::Vector2u& tilePosition)
        {
            if
            (
                tilePosition.x < 0 ||
                tilePosition.y < 0 ||
                tilePosition.x > tibia::MAP_TILE_XY_MAX ||
                tilePosition.y > tibia::MAP_TILE_XY_MAX
            )
            {
                return true;
            }

            return false;
        }

        int calculateTileDistance(int x1, int y1, int x2, int y2)
        {
            return std::max(std::abs(x1 - x2), std::abs(y1 - y2)) / tibia::TILE_SIZE;
        }

        float calculateVolumeByTileDistance(float volume, int distance)
        {
            volume = volume - (distance * tibia::VOLUME_MULTIPLIER);

            if (volume < 0)
            {
                volume = 0;
            }

            //std::cout << "volume: " << volume << std::endl;

            return volume;
        }

        unsigned int getSpriteFlags(int id)
        {
            unsigned int flags = 0;

            for (auto spriteId : tibia::SpriteData::solid)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::solid;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::blockProjectiles)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::blockProjectiles;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::water)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::water;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::lava)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::lava;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::hasHeight)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::hasHeight;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::offsetObjects)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::offset;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::moveAbove)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::moveAbove;
                }
            }

            for (auto spriteId : tibia::SpriteData::moveBelow)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::moveBelow;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::lightSource)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::lightSource;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::interactive)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::interactive;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::drawLastObjects)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::drawLast;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::transparent)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::transparent;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::animatedObjects)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::animated;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::ignoreHeightObjects)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::ignoreHeight;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::fixDrawOrderObjects)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::fixDrawOrder;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::moveable)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::moveable;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::modifyHpOnTouchObjects)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::modifyHpOnTouch;
                    break;
                }
            }

            return flags;
        }

        int getDirectionByKey(sf::Keyboard::Key key)
        {
            int direction = -1;

            switch (key)
            {
                case sf::Keyboard::Up:
                    direction = tibia::Directions::up;
                    break;

                case sf::Keyboard::Right:
                    direction = tibia::Directions::right;
                    break;

                case sf::Keyboard::Down:
                    direction = tibia::Directions::down;
                    break;

                case sf::Keyboard::Left:
                    direction = tibia::Directions::left;
                    break;
            }

            return direction;
        }

    sf::Vector2i getVectorByDirection(int direction)
    {
        sf::Vector2i vec(0, 0);

        switch (direction)
        {
            case tibia::Directions::up:
                vec.y--;
                break;

            case tibia::Directions::right:
                vec.x++;
                break;

            case tibia::Directions::down:
                vec.y++;
                break;

            case tibia::Directions::left:
                vec.x--;
                break;

            case tibia::Directions::upLeft:
                vec.x--;
                vec.y--;
                break;

            case tibia::Directions::upRight:
                vec.x++;
                vec.y--;
                break;

            case tibia::Directions::downLeft:
                vec.x--;
                vec.y++;
                break;

            case tibia::Directions::downRight:
                vec.x++;
                vec.y++;
                break;
        }

        return vec;
    }

    int getDirectionByVector(sf::Vector2f vec)
    {
        int direction = tibia::Directions::null;

        if (vec.x == 0 && vec.y < 0)
        {
            direction = tibia::Directions::up;
        }
        else if (vec.x > 0 && vec.y == 0)
        {
            direction = tibia::Directions::right;
        }
        else if (vec.x == 0 && vec.y > 0)
        {
            direction = tibia::Directions::down;
        }
        else if (vec.x < 0 && vec.y == 0)
        {
            direction = tibia::Directions::left;
        }
        else if (vec.x < 0 && vec.y < 0)
        {
            direction = tibia::Directions::upLeft;
        }
        else if (vec.x > 0 && vec.y < 0)
        {
            direction = tibia::Directions::upRight;
        }
        else if (vec.x < 0 && vec.y > 0)
        {
            direction = tibia::Directions::downLeft;
        }
        else if (vec.x > 0 && vec.y > 0)
        {
            direction = tibia::Directions::downRight;
        }

        return direction;
    }

    sf::Vector2f getNormalByVectors(sf::Vector2f origin, sf::Vector2f destination)
    {
        sf::Vector2f normal = destination - origin;

        normal = normal / thor::length(normal);

        return normal;
    }

    } // namespace utility

} // namespace tibia

#endif // TIBIA_UTILITY_HPP

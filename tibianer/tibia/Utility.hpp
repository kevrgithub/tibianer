#ifndef TIBIA_UTILITY_HPP
#define TIBIA_UTILITY_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

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

        int calculateTileDistance(int x1, int y1, int x2, int y2)
        {
            return std::max(std::abs(x1 - x2), std::abs(y1 - y2)) / tibia::TILE_SIZE;
        }

        int getSpriteFlags(int id)
        {
            int flags = 0;

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

            for (auto spriteId : tibia::SpriteData::chairs)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::chair;
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

            if (id == tibia::SpriteData::ladder)
            {
                flags |= tibia::SpriteFlags::ladder;
            }

            if (id == tibia::SpriteData::stairs)
            {
                flags |= tibia::SpriteFlags::moveAbove;
            }

            for (auto spriteId : tibia::SpriteData::holes)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::moveBelow;
                    break;
                }
            }

            for (auto spriteId : tibia::SpriteData::lights)
            {
                if (id == spriteId)
                {
                    flags |= tibia::SpriteFlags::light;
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

    sf::Vector2u getVectorByDirection(int direction)
    {
        sf::Vector2u vec(0, 0);

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

    } // utility

} // tibia

#endif // TIBIA_UTILITY_HPP

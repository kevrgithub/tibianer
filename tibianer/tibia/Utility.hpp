#ifndef TIBIA_UTILITY_HPP
#define TIBIA_UTILITY_HPP

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cmath>

#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include "tibia/Tibia.hpp"

namespace tibia
{

    namespace Utility
    {

        template <typename T>
        void vectorMoveToFront(std::vector<T>& vec, typename std::vector<T>::iterator element)
        {
            T temp(*element);
            std::copy_backward(vec.begin(), std::prev(element), element);
            *vec.begin() = temp;
        }

        sf::Vector2f getViewPosition(sf::View& view)
        {
            sf::Vector2f center = view.getCenter();

            sf::Vector2f half   = view.getSize() / 2.0f;

            return center - half;
        }

        sf::IntRect getSpriteRectById(int id)
        {
            id = id - 1;

            int u = (id % (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE)) * tibia::TILE_SIZE;
            int v = (id / (tibia::Textures::sprites.getSize().y / tibia::TILE_SIZE)) * tibia::TILE_SIZE;

            return sf::IntRect(u, v, tibia::TILE_SIZE, tibia::TILE_SIZE);
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

        tibia::SpriteFlags_t getSpriteFlags(int id)
        {
            tibia::SpriteFlags_t flags;

            std::map<tibia::SpriteFlags::Flags, std::vector<int>> spriteFlagsMap =
            {
                {tibia::SpriteFlags::solid,            tibia::SpriteData::solid},
                {tibia::SpriteFlags::blockProjectiles, tibia::SpriteData::blockProjectiles},
                {tibia::SpriteFlags::offset,           tibia::SpriteData::offsetObjects},
                {tibia::SpriteFlags::lightSource,      tibia::SpriteData::lightSource},
                {tibia::SpriteFlags::water,            tibia::SpriteData::water},
                {tibia::SpriteFlags::lava,             tibia::SpriteData::lava},
                {tibia::SpriteFlags::hasHeight,        tibia::SpriteData::hasHeight},
                {tibia::SpriteFlags::moveAbove,        tibia::SpriteData::moveAbove},
                {tibia::SpriteFlags::moveBelow,        tibia::SpriteData::moveBelow},
                {tibia::SpriteFlags::drawLast,         tibia::SpriteData::drawLastObjects},
                {tibia::SpriteFlags::transparent,      tibia::SpriteData::transparent},
                {tibia::SpriteFlags::animated,         tibia::SpriteData::animatedObjects},
                {tibia::SpriteFlags::ignoreHeight,     tibia::SpriteData::ignoreHeightObjects},
                {tibia::SpriteFlags::fixDrawOrder,     tibia::SpriteData::fixDrawOrderObjects},
                {tibia::SpriteFlags::modifyHpOnTouch,  tibia::SpriteData::modifyHpOnTouchObjects},
                {tibia::SpriteFlags::moveable,         tibia::SpriteData::moveable},
                {tibia::SpriteFlags::groupable,        tibia::SpriteData::groupable},
                {tibia::SpriteFlags::stackable,        tibia::SpriteData::stackable},
                {tibia::SpriteFlags::pickupable,       tibia::SpriteData::pickupable},
                {tibia::SpriteFlags::decal,            tibia::SpriteData::decals},
            };

            for (auto spriteFlagsMapPair : spriteFlagsMap)
            {
                auto findIdIt = std::find(spriteFlagsMapPair.second.begin(), spriteFlagsMapPair.second.end(), id);
                
                if (findIdIt != spriteFlagsMapPair.second.end())
                {
                    flags.set(spriteFlagsMapPair.first);
                }
            }

            return flags;
        }

        int getDirectionByKey(sf::Keyboard::Key key)
        {
            int direction = tibia::Directions::null;

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

    int getCountByGroupableObjectIndex(int index, int numObjects)
    {
        switch (index)
        {
            case 0:
                return 1;

            case 1:
                return 2;

            case 2:
                return 3;

            case 3:
                return 4;
        }

        if (numObjects == 8)
        {
            switch (index)
            {
                case 4:
                    return 5;

                case 5:
                    return 10;

                case 6:
                    return 50;

                case 7:
                    return 100;
            }
        }
        else if (numObjects == 7)
        {
            switch (index)
            {
                case 4:
                    return 5;

                case 5:
                    return 50;

                case 6:
                    return 100;
            }
        }
        else if (numObjects == 6)
        {
            switch (index)
            {
                case 4:
                    return 5;

                case 5:
                    return 100;
            }
        }
        else if (numObjects == 5)
        {
            switch (index)
            {
                case 4:
                    return 100;
            }
        }

        return 1;
    }

    int getGroupableObjectIndexByCount(int count, int numObjects)
    {
        if (count == 1)
        {
            return 0;
        }
        else if (count == 2)
        {
            return 1;
        }
        else if (count == 3)
        {
            return 2;
        }

        if (numObjects == 8)
        {
            if (count == 4)
            {
                return 3;
            }
            else if (count >= 5 && count < 10)
            {
                return 4;
            }
            else if (count >= 10 && count < 50)
            {
                return 5;
            }
            else if (count >= 50 && count < 100)
            {
                return 6;
            }
            else if (count == 100)
            {
                return 7;
            }
        }
        else if (numObjects == 7)
        {
            if (count == 4)
            {
                return 3;
            }
            else if (count >= 5 && count < 10)
            {
                return 4;
            }
            else if (count >= 10 && count < 50)
            {
                return 5;
            }
            else if (count >= 50)
            {
                return 6;
            }
        }
        else if (numObjects == 6)
        {
            if (count == 4)
            {
                return 3;
            }
            else if (count >= 5 && count < 10)
            {
                return 4;
            }
            else if (count >= 10)
            {
                return 5;
            }
        }
        else if (numObjects == 5)
        {
            if (count == 4)
            {
                return 3;
            }
            else if (count >= 5)
            {
                return 4;
            }
        }
        else if (numObjects < 5)
        {
            int index = numObjects - 1;

            if (count > index)
            {
                return index;
            }
        }

        return 0;
    }

    } // namespace utility

} // namespace tibia

#endif // TIBIA_UTILITY_HPP

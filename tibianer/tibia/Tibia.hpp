#ifndef TIBIA_TIBIA_HPP
#define TIBIA_TIBIA_HPP

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

namespace tibia
{

    sf::Texture sprites;
    sf::Vector2u spritesSize;

    const int MAP_SIZE = 128;

    const int TILE_SIZE = 32;

    namespace animatedTiles
    {
        int water[] = {106, 107, 108, 109, 110, 111, 112, 113};
    }

    namespace animatedObjects
    {
        std::vector<int> bluePortal;

        std::vector<int> electricity;

        std::vector<int> magicWall;
        std::vector<int> magicWall2;
        std::vector<int> magicWall3;
        std::vector<int> magicWall4;
    }

    std::vector<std::vector<int> > animatedObjectsList;

    namespace outfits
    {
        // 10
        int head[] = 
        {
            671, 672, 670, 673,
            794, 800, 812, 806,
            795, 801, 813, 807,
            796, 802, 814, 808,
            797, 803, 815, 809,
            798, 804, 816, 810,
            799, 805, 817, 811,
            2561, 2562, 2564, 2563,
            2577, 2578, 2580, 2579,
            2593, 2594, 2596, 2595,
        };

        // 11
        int body[] = 
        {
            675, 676, 674, 677,
            818, 824, 836, 830,
            819, 825, 837, 831,
            820, 826, 838, 832,
            821, 827, 839, 833,
            822, 828, 840, 834,
            823, 829, 841, 835,
            2549, 2550, 2552, 2551,
            2565, 2566, 2568, 2567,
            2581, 2582, 2584, 2583,
            2597, 2598, 2600, 2599,
        };

        // 10
        int legs[] = 
        {
            842, 848, 860, 854,
            843, 849, 861, 855,
            844, 850, 862, 856,
            845, 851, 863, 857,
            846, 852, 864, 858,
            847, 853, 865, 859,
            2553, 2554, 2556, 2555,
            2569, 2570, 2572, 2571,
            2585, 2586, 2588, 2587,
            2601, 2602, 2604, 2603,
        };

        // 10
        int feet[] = 
        {
            866, 872, 884, 878,
            867, 873, 885, 879,
            868, 874, 886, 880,
            869, 875, 887, 881,
            870, 876, 888, 882,
            871, 877, 889, 883,
            2557, 2558, 2560, 2559,
            2573, 2574, 2576, 2575,
            2589, 2590, 2592, 2591,
            2605, 2606, 2608, 2607,
        };
    }

    int getRandomNumber(int low, int high)
    {
        return std::rand() % (high - low + 1) + low;
    }

    bool loadSprites()
    {
        if (!tibia::sprites.loadFromFile("images/sprites.png"))
        {
            return false;
        }

        tibia::spritesSize = tibia::sprites.getSize();

        return true;
    }

    sf::IntRect getSpriteRectById(int id)
    {
        id = id - 1;

        int u = (id % (tibia::spritesSize.x / tibia::TILE_SIZE)) * tibia::TILE_SIZE;
        int v = (id / (tibia::spritesSize.y / tibia::TILE_SIZE)) * tibia::TILE_SIZE;

        sf::IntRect ir(u, v, tibia::TILE_SIZE, tibia::TILE_SIZE);

        return ir;
    }

    void loadAnimatedObjects()
    {
        tibia::animatedObjects::bluePortal.push_back(197);
        tibia::animatedObjects::bluePortal.push_back(198);
        //tibia::animatedObjects::bluePortal.push_back(199);
        tibia::animatedObjectsList.push_back(tibia::animatedObjects::bluePortal);

        tibia::animatedObjects::electricity.push_back(1497);
        tibia::animatedObjects::electricity.push_back(1498);
        tibia::animatedObjectsList.push_back(tibia::animatedObjects::electricity);

        tibia::animatedObjects::magicWall.push_back(3268);
        tibia::animatedObjects::magicWall.push_back(3272);
        tibia::animatedObjects::magicWall.push_back(3276);
        tibia::animatedObjectsList.push_back(tibia::animatedObjects::magicWall);

        tibia::animatedObjects::magicWall2.push_back(3269);
        tibia::animatedObjects::magicWall2.push_back(3273);
        tibia::animatedObjects::magicWall2.push_back(3277);
        tibia::animatedObjectsList.push_back(tibia::animatedObjects::magicWall2);

        tibia::animatedObjects::magicWall3.push_back(3270);
        tibia::animatedObjects::magicWall3.push_back(3274);
        tibia::animatedObjects::magicWall3.push_back(3278);
        tibia::animatedObjectsList.push_back(tibia::animatedObjects::magicWall3);

        tibia::animatedObjects::magicWall4.push_back(3271);
        tibia::animatedObjects::magicWall4.push_back(3275);
        tibia::animatedObjects::magicWall4.push_back(3279);
        tibia::animatedObjectsList.push_back(tibia::animatedObjects::magicWall4);
    }

}

#endif // TIBIA_TIBIA_HPP

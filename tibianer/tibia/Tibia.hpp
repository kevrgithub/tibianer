#ifndef TIBIA_TIBIA_HPP
#define TIBIA_TIBIA_HPP

#include <string>
#include <vector>
#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

namespace tibia
{
    const int MAP_SIZE = 128;

    const int TILE_NUMBER_MAX = (MAP_SIZE * MAP_SIZE) - 1;

    const int TILE_SIZE = 32;

    const int NUM_TILES_X = 13;
    const int NUM_TILES_Y = 9;

    const int MAP_XY_MAX = (MAP_SIZE * TILE_SIZE) - TILE_SIZE;

    const int TILES_WIDTH  = NUM_TILES_X * TILE_SIZE;
    const int TILES_HEIGHT = NUM_TILES_Y * TILE_SIZE;

    const int TILE_NULL = 0;

    const int TILE_DRAW_OFFSET = 8;

    namespace Textures
    {
        sf::Texture sprites;

        sf::Texture light;
        sf::Texture light2;
        sf::Texture light3;
    }

    bool loadTextures()
    {
        if (tibia::Textures::sprites.loadFromFile("images/sprites.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light.loadFromFile("images/light.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light2.loadFromFile("images/light2.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light3.loadFromFile("images/light3.png") == false)
        {
            return false;
        }

        return true;
    }

    namespace Fonts
    {
        std::string default = "fonts/OpenSans.ttf";
        int defaultSize = 48;

        std::string small = "fonts/NewFont.ttf";
        int smallSize = 16;

        int gameSize = 24;
    }

    namespace Colors
    {
        sf::Color black(0, 0, 0);
        sf::Color white(255, 255, 255);
        sf::Color red(255, 0, 0);
        sf::Color green(0, 255, 0);
        sf::Color blue(0, 0, 255);
        sf::Color yellow(255, 255, 0);
        sf::Color teal(0, 255, 255);
        sf::Color pink(255, 0, 255);

        sf::Color mainWindowColor(32, 32, 32);

        sf::Color windowBorderColor(128, 128, 128);

        sf::Color gameTextShadowColor(32, 32, 32, 192);
    }

    namespace GuiData
    {
        int gameWindowX = 32;
        int gameWindowY = 32;
    }

    enum ZAxis
    {
        underGround = -1,
        ground      = 0,
        aboveGround = 1
    };

    enum Directions
    {
        up,
        right,
        down,
        left
    };

    namespace MovementSpeeds
    {
        float player = 0.01; // 0.1
    }

    namespace SpriteData
    {
        int guiTextIcons[] = {529, 530, 531, 532, 533, 534};

        int corpse[] = {491, 492, 493, 494, 495, 496, 497};

        int lever[] = {49, 50};

        int stepTileStone[] = {398,  399};
        int stepTileWood[]  = {1275, 1276};

        int trough[]   = {52, 51, 685, 686, 687, 688, 689, 690};
        int bucket[]   = {310, 678, 679, 680, 681, 682, 683, 684};
        int cauldron[] = {1181, 1182, 1183, 1184, 1187, 1188, 1189, 1190};
        int bottle[]   = {1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563};

        int parcel[] = {1268, 1269};
        int letter[] = {1270, 1271};

        int label   = 1272;
        int depot   = 1273;
        int mailBox = 1274;

        int runeBlank = 1277;

        int fishingRod = 1470;

        int dustBin = 1472;

        int spellBlue = 1829;

        int presentBox = 2160;

        int spellBlack = 3252;

        int water[] = 
        {
            106, 107, 108, 109,
            110, 111, 112, 113,
        };

        int lava[] =
        {
            1785, 1786, 1787, 1788,
            1789, 1790, 1791, 1792,
        };

        int solid[] = 
        {
            12, 13, 21, 25, 27, 28, 29,
            37,
            39,
            44,
            46,
            51, 52,
            55, 56, 58, 60, 66, 67, 68, 72,
            74, 75,
            88,
            106, 107, 108, 109, 110, 111, 112, 113,
            114,
            116,
            129, 130, 131, 132, 133, 134, 135,
            136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
            159, 163,
            201,
            202, 203, 204, 205,
            285, 286, 288, 290,
            315,
            339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362,
            367, 368, 369, 370,
            376, 378,
            379, 380, 381, 382, 383, 384,
            388,
            390, 391, 392, 393,
            466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477,
            486,
            523,
            548, 549, 554, 559,
            562,
            565, 566, 569, 570, 574, 575, 576, 580, 582, 584, 586, 588,
            685, 686, 687, 688, 689, 690,
            703, 707,
            906, 907, 909, 910, 915, 916, 918, 919,
            934, 935, 936, 937, 938, 939, 940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960,
            961, 962,
            997, 999, 1001,
            1003,
            1004, 1005, 1009, 1015, 1021, 1025, 1029, 1033, 1038, 1042, 1043, 1044, 1046, 1048,
            1060,
            1066, 1067, 1068, 1069, 1070,
            1071, 1072, 1073, 1074,
            1092, 1093, 1094, 1095, 1096, 1097, 1098, 1099, 1100, 1101, 1102, 1103,
            1122,
            1140, 1144,
            1145, 1146,
            1147, 1148,
            1152,
            1154, 1156,
            1160, 1164, 1168, 1172, 1176, 1180,
            1181, 1182, 1183, 1184,
            1185, 1186,
            1187, 1188, 1189, 1190,
            1191,
            1192,
            1193, 1194,
            1195, 1196, 1197, 1198,
            1199,
            1203,
            1224,
            1264, 1265, 1266, 1267,
            1273, 1274,
            1349, 1351,
            1465, 1466, 1467, 1468,
            1472,
            1495, 1496,
            1564,
            1565, 1566,
            1567,
            1591, 1592, 1593, 1594,
            1595, 1596, 1597, 1598,
            1602,
            1603, 1604,
            1610,
            1614,
            1618,
            1622,
            1624,
            1626,
            1630,
            1634,
            1635, 1636, 1637, 1638,
            1639, 1640, 1641, 1642,
            1651,
            1652,
            1655, 1659,
            1663, 1665,
            1669,
            1670, 1671,
            1672,
            1676,
            1680,
            1684,
            1688,
            1692,
            1760,
            1764,
            1768,
            1772,
            1785, 1786, 1787, 1788, 1789, 1790, 1791, 1792,
            1817, 1818, 1819, 1820, 1821, 1822, 1823, 1824, 1825, 1826, 1827, 1828,
            1939, 1940, 1941, 1942, 1943, 1944, 1945, 1946,
            2123,
            2127, 2131,
            2152,
            2164,
            2168,
            2172,
            2408,
            2486, 2490, 2494,
            2538,
            2664, 2665, 2666, 2667,
            2668, 2669, 2670, 2671, 2672, 2673, 2674,
            2675, 2676, 2677, 2678,
            2679, 2680, 2681, 2682, 2683, 2684, 2685,
            2686, 2687, 2688, 2689, 2690, 2691,
            2692, 2693, 2694,
            2695, 2696, 2697, 2698,
            2703, 2704, 2705, 2706, 2707, 2708, 2709, 2710, 2711, 2712, 2713, 2714, 2715, 2716, 2717, 2718,
            2849, 2850, 2851, 2852, 2853, 2854, 2855, 2856, 2857, 2858, 2859, 2860,
            3030, 3032, 3034,
            3035,
            3039,
            3043,
            3178, 3179, 3180, 3181, 3182, 3183,
            3184,
            3185,
            3186,
            3235,
            3236,
            3237,
            3254, 3255, 3256, 3257,
            3260, 3261, 3262, 3263, 3264, 3265, 3266, 3267,
            3271, 3275, 3279,
            3295, 3303,
            3310, 3313
        };

        int blockProjectiles[] =
        {
            88
        };

        int chairs[] = 
        {
            298,
            299,
            521, 522,
            1054, 1055,
            1225,
            1268, 1269,
            1924, 1925, 1926, 1927,
            3239, 3240
        };
    }

    //namespace AnimatedObjects
    //{
        //std::vector<int> bluePortal = {197, 198, 199};
    //}

    std::vector<std::vector<int>> animatedObjectsList;

    namespace Outfits
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

    float calculateDistance(float x1, float y1, float x2, float y2)
    {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    int getRandomNumber(int low, int high)
    {
        return std::rand() % (high - low + 1) + low;
    }

    sf::IntRect getSpriteRectById(int id)
    {
        id = id - 1;

        int u = (id % (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE)) * tibia::TILE_SIZE;
        int v = (id / (tibia::Textures::sprites.getSize().y / tibia::TILE_SIZE)) * tibia::TILE_SIZE;

        sf::IntRect ir(u, v, tibia::TILE_SIZE, tibia::TILE_SIZE);

        return ir;
    }

    int getTileNumberByTileCoords(int x, int y)
    {
        int tileX = x;
        int tileY = y;

        tileX = tileX - (tileX % tibia::TILE_SIZE);
        tileY = tileY - (tileY % tibia::TILE_SIZE);

        return (tileX + tileY * tibia::MAP_SIZE) / tibia::TILE_SIZE;
    }

}

#endif // TIBIA_TIBIA_HPP

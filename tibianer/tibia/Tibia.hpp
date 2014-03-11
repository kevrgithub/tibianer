#ifndef TIBIA_TIBIA_HPP
#define TIBIA_TIBIA_HPP

#include <string>
#include <vector>
#include <memory>
#include <cmath>

#include "boost_foreach.hpp"

#include <SFML/Graphics.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

namespace tibia
{
    const int MAP_SIZE = 128;

    const int TILE_NUMBER_MAX = (MAP_SIZE * MAP_SIZE) - 1;

    const int TILE_SIZE = 32;

    const int NUM_TILES_X = 13;
    const int NUM_TILES_Y = 9;

    const int NUM_TILES_FROM_CENTER_X = 6;
    const int NUM_TILES_FROM_CENTER_Y = 4;

    const int NUM_TILES_TOTAL = NUM_TILES_X * NUM_TILES_Y;

    const int MAP_XY_MAX = MAP_SIZE;

    const int MAP_TILE_XY_MAX = MAP_SIZE * TILE_SIZE;

    const int TILES_WIDTH  = NUM_TILES_X * TILE_SIZE;
    const int TILES_HEIGHT = NUM_TILES_Y * TILE_SIZE;

    const int TILE_NULL = 0;

    const int TILE_DRAW_OFFSET = 8;

    const int VOLUME_MULTIPLIER = 8;

    const float TEXT_TIME = 5.0;

    const float DRAW_DISTANCE_MAX = 10.0;

    const int CREATURES_MAX_LOAD = 256;

    const int LIGHT_WIDTH  = 480;
    const int LIGHT_HEIGHT = 352;

    namespace Textures
    {
        sf::Texture sprites;

        sf::Texture font;
        sf::Texture font2;

        sf::Texture light;
        sf::Texture light2;
        sf::Texture light3;
        sf::Texture light4;
        sf::Texture light5;
    }

    namespace Fonts
    {
        std::string default = "fonts/OpenSans.ttf";
        std::string small   = "fonts/NewFont.ttf";
        std::string martel  = "fonts/martel.ttf";
    }

    namespace FontSizes
    {
        int default = 48;
        int small   = 16;
        int game    = 24;
        int title   = 128;
    }

    namespace Colors
    {
        sf::Color transparent(0, 0, 0, 0);

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

        sf::Color tileIsSolid(64, 64, 64);
        sf::Color tileIsWater(0, 0, 192);
        sf::Color tileIsLava(192, 0, 0);
        sf::Color tileIsMoveAboveOrBelow(255, 255, 0);

        sf::Color light(255, 255, 255, 192);
    }

    enum Teams
    {
        neutral,
        good,
        evil
    };

    namespace GuiData
    {
        int gameWindowX = 32;
        int gameWindowY = 32;

        int gameWindowWidth  = TILES_WIDTH;
        int gameWindowHeight = TILES_HEIGHT;

        sf::IntRect gameWindowRect(gameWindowX, gameWindowY, gameWindowWidth, gameWindowHeight);

        int miniMapWindowX = gameWindowX + gameWindowWidth + 32;
        int miniMapWindowY = 32;

        int miniMapWindowWidth  = 128;
        int miniMapWindowHeight = 88;

        sf::IntRect miniMapWindowRect(miniMapWindowX, miniMapWindowY, miniMapWindowWidth, miniMapWindowHeight);
    }

    enum ZAxis
    {
        floor       = -2,
        underGround = -1,
        ground      = 0,
        aboveGround = 1,
        ceiling     = 2
    };

    enum Directions
    {
        up,
        right,
        down,
        left,
        upLeft,
        upRight,
        downRight,
        downLeft,

        begin = up,
        end   = downLeft
    };

    namespace MovementSpeeds
    {
        float default = 0.5;
        float player  = 0.01; // 0.2
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

        int blankRune = 1277;

        int fishingRod = 1470;

        int dustBin = 1472;

        int presentBox = 2160;

        int ladder = 455;

        int stairs = 460;

        int treeWall = 3344;

        std::vector<int> magicWalls = {3271, 3275, 3279};

        std::vector<int> water =
        {
            106, 107, 108, 109,
            110, 111, 112, 113
        };

        int waterBegin = water.front();
        int waterEnd   = water.back();

        std::vector<int> lava =
        {
            1785, 1786, 1787, 1788,
            1789, 1790, 1791, 1792
        };

        int lavaBegin = lava.front();
        int lavaEnd   = lava.back();

        std::vector<int> solid =
        {
            12, 13, 21, 24, 25, 27, 28, 29,
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
            334,
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
            3310, 3313,
            3322, 3326,
            3344,
            3348,
            3374
        };

        std::vector<int> blockProjectiles =
        {
            12, 13, 21, 24, 25, 27, 28, 29,
            55, 56, 58, 60, 66, 67, 68, 72,
            88,
            315,
            388,
            466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477,
            486,
            548, 549, 554, 555,
            562,
            565, 566, 569, 570, 574, 575, 576, 580, 582, 584, 586, 588,
            703, 707,
            906, 907, 909, 910, 915, 916, 918, 919,
            934, 935, 936, 937, 938, 939, 940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960,
            997, 999, 1001,
            1066, 1067, 1068, 1069, 1070,
            1092, 1093, 1094, 1095, 1096, 1097, 1098, 1099, 1100, 1101, 1102, 1103,
            1140, 1144, 1152, 1160, 1164, 1168, 1172, 1176, 1180,
            1195, 1196, 1197, 1198,
            1224,
            1591, 1592, 1593, 1594,
            1595, 1596, 1597, 1598,
            1602,
            1610, 1614, 1618, 1622, 1630, 1634, 1669,
            1670, 1671,
            1676, 1680, 1684, 1688, 1692, 1760, 1764, 1768, 1772,
            1939, 1940, 1941, 1942, 1943, 1944, 1945, 1946,
            2123, 2127, 2131,
            2152,
            2164, 2168, 2172,
            2408,
            2486, 2490, 2494,
            2703, 2704, 2705, 2706, 2707, 2708, 2709, 2710, 2711, 2712, 2713, 2714, 2715, 2716, 2717, 2718,
            3030, 3032, 3034,
            3039, 3043,
            3235,
            3254, 3255, 3256, 3257,
            3271, 3275, 3279,
            3294, 3295, 3301, 3303,
            3310, 3313,
            3322, 3326,
            3344,
            3348
        };

        std::vector<int> chairs =
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

        std::vector<int> offsetObjects =
        {
            37,
            46,
            88,
            1264, 1265, 1266, 1267,
            1273, 1274
        };

        std::vector<int> holes =
        {
            47, 456, 479, 480, 481, 487,
            610, 611,
            691, 692, 693, 694, 695, 696, 697, 698, 699,
            1205,
            3241, 3243
        };

        std::vector<int> lights =
        {
            197, 198, 199,
            395, 396,
            443, 444, 446, 447, 449, 450,
            455, // ladder
            486,
            489,
            525, 526,
            703, 707,
            1049,
            1123, 1124,
            1154, 1156,
            1489, 1490, 1491, 1492, 1493, 1494,
            1497, 1498,
            1582, 1583, 1584, 1585, 1586,
            1785, 1786, 1787, 1788, 1789, 1790, 1791, 1792,
            1818, 1819, 1821, 1822, 1826, 1828,
            1879, 1880,
            2127, 2131,
            2664, 2665, 2666, 2667,
            2668, 2669, 2670, 2671, 2672, 2673,
            2675, 2676, 2677, 2678,
            2679, 2680, 2681, 2682, 2683, 2684,
            2686, 2687, 2688, 2689, 2690, 2691,
            2693, 2694,
            2695, 2696, 2697, 2698,
            2868, 2869, 2871, 2872,
            3271, 3275, 3279,
            3322, 3326
        };

        std::vector<int> quadObjects =
        {
            13, 21, 29,
            159, 163,
            315, // 5 objects
            388, 455, 460, 486, 562, 703, 707,
            1140, 1144, 1152, 1160, 1164, 1168, 1172, 1176, 1180, 1224,
            1602, 1610, 1614, 1618, 1622, 1630, 1634, 1651, 1669, 1676, 1680, 1684, 1688, 1692,
            1760, 1764, 1768, 1772,
            2123, 2127, 2131, 2152, 2164, 2168, 2172,
            2408, 2486, 2490, 2494, 
            3039, 3043,
            3271, 3275, 3279,
            3322, 3326,
            3344, 3348
        };

        std::vector<int> quadVerticalObjects =
        {
            25
        };

        std::vector<int> horizontalObjects =
        {
            609,
            2969,
            3030, 3032, 3034
        };

        std::vector<int> verticalObjects =
        {
            334,
            997, 999, 1001,
            1349, 1351,
            2965
        };

        std::vector<int> fixDrawObjects =
        {
            16, // brick wall arch
            2867, 2868, 2869, 2870, 2871, 2872 // wall inset torches
        };

        std::vector<int> animatedObjects =
        {
            197,198,199,
            1489,1490,
            1491,1492,
            1493,1494,
            1497,1498,
            3158,3159,3160,3161,
            3268,3272,3276,
            3269,3273,3277,
            3270,3274,3278,
            3271,3275,3279,
            202,367,
            203,368,
            204,369,
            205,370,
            2868,2869,
            2871,2872,
            2968,2970,
            2974,2975,
            395,396,
            525,526,
            443,444,
            446,447,
            449,450,
            890,896,
            891,897,
            892,898,
            893,899,
            894,900,
            895,901,
            902,911,
            903,912,
            904,913,
            905,914,
            906,915,
            907,916,
            908,917,
            909,918,
            910,919,
            1123,1124,
            1153,1155,
            1154,1156,
            1570,1571,1572,1573,1574,
            1582,1583,1584,1585,1586,
            1879,1880,
            2124,2128,
            2125,2129,
            2126,2130,
            2127,2131,
            2664,2665,2666,2667,
            2668,2669,
            2670,2671,
            2672,2673,
            2675,2676,2677,2678,
            2679,2680,
            2681,2682,
            2683,2684,
            2686,2687,2688,2689,2690,2691,
            2693,2694,
            2695,2696,
            2697,2698,
            3178,3179,3180,3181,3182,3183,
            3319,3323,
            3320,3324,
            3321,3325,
            3322,3326
        };
    }

    enum CreatureTypes
    {
        human,

        demon,
        demonSkeleton,
        gameMaster,
        hero,
        monk,
        necromancer,
        orc,
        santaClaus,
        skeleton,
        spider,
        witch,
        zombie
    };

    enum CreatureSizes
    {
        small,
        medium,
        large
    };

    namespace CreatureSprites
    {
        std::vector<int> demon =
        {
            1480, 1484, 1476, 1488,
            1479, 1483, 1475, 1487,
            1478, 1482, 1474, 1486,
            1477, 1481, 1473, 1485
        };

        std::vector<int> hero = {1645, 1646, 1643, 1644};

        std::vector<int> gameMaster = {3330, 3329, 3327, 3328};

        std::vector<int> orc = {2863, 2862, 2864, 2861};

        std::vector<int> skeleton = {1404, 1407, 1405, 1406};

        std::vector<int> witch =
        {
            2884, 2888, 2882, 2886,
            2883, 2887, 2881, 2885
        };

        std::vector<int> zombie = {1838, 1841, 1839, 1840};
    }

    namespace CreatureCorpseSprites
    {
        std::vector<int> human = {491, 492, 493, 494, 495, 496, 497};

        std::vector<int> demon =
        {
            319, 318, 317, 316,
            323, 322, 321, 320,
            327, 326, 325, 324,
            331, 330, 329, 328
        };
    }

    //namespace AnimatedObjects
    //{
        //std::vector<int> bluePortal = {197, 198, 199};
    //}

    std::vector<std::vector<int>> animatedObjectsList;

    namespace Animations
    {
        // {id, numFrames}

        int waterSplash[2] = {335, 4};

        int hitBlood[2]  = {363, 4};
        int hitMiss[2]   = {920, 4};
        int hitBlock[2]  = {924, 3};
        int hitBlack[2]  = {1388, 8};
        int hitPoison[2] = {2132, 4};

        int urine[2] =  {1374, 7};
        int poison[2] = {1381, 7};

        int spellBlue[2]  = {1396, 8};
        int spellBlack[2] = {3244, 8};

        int fire[2]        = {1489, 2};
        int electricity[2] = {1497, 2};

        int particlesBlue[2]  = {1499, 5};
        int particlesRed[2]   = {1504, 5};
        int particlesGreen[2] = {1509, 5};

        int bubbleGreen[2] = {1570, 5};
        int bubbleRed[2]   = {1582, 5};
    }

    namespace AnimatedDecals
    {
        int corpse[2] = {491, 7};

        int poolRed[2]    = {745, 3};
        int poolBlue[2]   = {751, 3};
        int poolGreen[2]  = {757, 3};
        int poolOrange[2] = {763, 3};
        int poolYellow[2] = {769, 3};
        int poolPurple[2] = {775, 3};
        int poolWhite[2]  = {781, 3};

        int splatRed[2]    = {748, 3};
        int splatBlue[2]   = {754, 3};
        int splatGreen[2]  = {760, 3};
        int splatOrange[2] = {766, 3};
        int splatYellow[2] = {772, 3};
        int splatPurple[2] = {778, 3};
        int splatWhite[2]  = {784, 3};
    }

    namespace AnimationTimes
    {
        float default = 0.1;
        float decal   = 60.0;
    }

    namespace Projectiles
    {
        const int spellBlue  = 1829;
        const int spellBlack = 3252;

        const int spellFire[8] = {1831, 1834, 1836, 1833, 1830, 1832, 1837, 1835};

        const int spear[8] = {963, 965, 967, 969, 970, 964, 966, 968};

        const int bolt[8] = {972, 974, 976, 978, 979, 973, 975, 977};

        const int arrow[8]       = {980, 982, 984, 986, 987, 981, 983, 985};
        const int arrowFire[8]   = {1855, 1857, 1859, 1861, 1854, 1856, 1858, 1860};
        const int arrowPoison[8] = {1847, 1849, 1851, 1853, 1846, 1848, 1850, 1852};
    }

    enum ProjectileTypes
    {
        spellBlue,
        spellBlack,
        spellFire,
        spear,
        bolt,
        arrow,
        arrowFire,
        arrowPoison
    };

    namespace ProjectileRanges
    {
        const float default = 6.0;
    }

    namespace ProjectileSpeeds
    {
        const float default = 8.0;
    }

    namespace ProjectileDamages
    {
        const float default = 5.0;
    }

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

    enum TileMapTypes
    {
        tiles,
        edges,
        walls,
        objects
    };

    enum TileFlags
    {
        null             = 1 << 0,
        solid            = 1 << 1,
        blockProjectiles = 1 << 2,
        offset           = 1 << 3,
        light            = 1 << 4,
        water            = 1 << 5,
        lava             = 1 << 6,
        chair            = 1 << 7,
        ladder           = 1 << 8,
        moveAbove        = 1 << 9,
        moveBelow        = 1 << 10
    };

    namespace Sounds
    {
        sf::SoundBuffer death;

        sf::SoundBuffer arrowBlock;
    }

    float calculateDistance(float x1, float y1, float x2, float y2)
    {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    int calculateDistanceByTile(int x1, int y1, int x2, int y2)
    {
        return std::max(std::abs(x1 - x2), std::abs(y1 - y2)) / tibia::TILE_SIZE;
    }

    float calculateVolumeByDistance(float distance)
    {
        float volume = 100 - (distance * tibia::VOLUME_MULTIPLIER);

        if (volume < 0)
        {
            volume = 0;
        }

        //std::cout << "volume: " << volume << std::endl;

        return volume;
    }

    int getRandomNumber(int low, int high)
    {
        return std::rand() % ((high - low) + 1) + low;
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

    sf::Vector2u getTileCoordsByTileNumber(int tileNumber)
    {
        sf::Vector2u coords;

        coords.x = (tileNumber % tibia::MAP_SIZE) * tibia::TILE_SIZE;
        coords.y = (tileNumber / tibia::MAP_SIZE) * tibia::TILE_SIZE;

        return coords;
    }

    int roundUp(int number, int multiple) 
    { 
        if (multiple == 0)
        { 
            return number;
        } 

         int remainder = number % multiple;

         if (remainder == 0)
         {
            return number;
         }

        return number + multiple - remainder;
    }

    float getFloatInverse(float value)
    {
        unsigned int *i = (unsigned int*)&value;

        *i = 0x7F000000 - *i; 

        return value;
    }

    float getFloatNormal(float value)
    {
        if (value == 0) return 0;

        return (value < 0) ? -1 : 1;
    }

    float getFloatNormalEx(float value)
    {
        if (value == 0) return 0;

        if (value > 0 && value < 0.5) return 0;

        if (value < 0 && value > -0.5) return 0;

        return (value < 0) ? -1 : 1;
    }

    sf::Vector2f getNormalByVectors(sf::Vector2f origin, sf::Vector2f destination)
    {
        sf::Vector2f normal;

        normal = destination - origin;

        normal = normal / thor::length(normal);

        return normal;
    }

    sf::Vector2f getVectorByDirection(int direction)
    {
        sf::Vector2f vec(0, 0);

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
        int direction = -1;

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

    int getSpriteTileFlags(int id)
    {
        int flags = 0;

        for (auto spriteId : tibia::SpriteData::solid)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::solid;
                break;
            }
        }

        for (auto spriteId : tibia::SpriteData::blockProjectiles)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::blockProjectiles;
                break;
            }
        }

        for (auto spriteId : tibia::SpriteData::water)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::water;
                break;
            }
        }

        for (auto spriteId : tibia::SpriteData::lava)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::lava;
                break;
            }
        }

        for (auto spriteId : tibia::SpriteData::chairs)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::chair;
                break;
            }
        }

        for (auto spriteId : tibia::SpriteData::offsetObjects)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::offset;
                break;
            }
        }

        if (id == tibia::SpriteData::ladder)
        {
            flags |= tibia::TileFlags::ladder;
        }

        if (id == tibia::SpriteData::stairs)
        {
            flags |= tibia::TileFlags::moveAbove;
        }

        for (auto spriteId : tibia::SpriteData::holes)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::moveBelow;
                break;
            }
        }

        for (auto spriteId : tibia::SpriteData::lights)
        {
            if (id == spriteId)
            {
                flags |= tibia::TileFlags::light;
                break;
            }
        }

        return flags;
    }

}

#endif // TIBIA_TIBIA_HPP

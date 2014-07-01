#ifndef TIBIA_TIBIA_HPP
#define TIBIA_TIBIA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>

#include <SFML/Graphics.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

namespace tibia
{
    const int SPRITES_TOTAL = 3381;

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

    const int THING_DRAW_OFFSET = 8;

    const int LIGHT_WIDTH  = 480;
    const int LIGHT_HEIGHT = 352;

    namespace Textures
    {
        sf::Texture sprites;
        sf::Texture lights;
        sf::Texture background;
        sf::Texture cursor;
    }

    namespace Fonts
    {
        std::string default = "fonts/OpenSans.ttf";
    }

    namespace FontSizes
    {
        const int default = 48;
        const int small   = 16;
        const int game    = 24;
        const int title   = 128;
    }

    namespace BitmapFonts
    {
        std::string default = "images/font.png";
        std::string tiny    = "images/font2.png";

        sf::Vector2u defaultGlyphSize(18, 19);

        std::vector<int> defaultGlyphWidths =
        {
            9, 4, 8, 16, 9, 18, 15, 5, 7, 7, 9, 13, 5, 8, 4, 7,
            10, 8, 10, 10, 10, 9, 10, 10, 9, 10, 4, 5, 12, 14, 12, 9,
            14, 15, 13, 13, 14, 13, 12, 15, 15, 8, 9, 15, 13, 18, 15, 14,
            12, 14, 15, 11, 14, 15, 15, 18, 15, 15, 13, 15, 14, 15, 11, 13,
            4, 9, 11, 9, 11, 9, 10, 11, 11, 6, 7, 11, 6, 16, 11, 10,
            11, 11, 8, 8, 7, 11, 11, 15, 11, 12, 10, 9, 10, 11, 14, 11
        };
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

        sf::Color light(255, 255, 255, 192);

        sf::Color textYellow(255, 255, 64);

        sf::Color mainWindowColor = white;
        sf::Color windowBorderColor = black;

        sf::Color tileIsSolid(64, 64, 64);
        sf::Color tileIsWater(0, 0, 192);
        sf::Color tileIsLava(192, 0, 0);
        sf::Color tileIsMoveAboveOrBelow(255, 255, 0);
    }

    namespace GuiData
    {
        const int gameWindowX = 32;
        const int gameWindowY = 32;

        const int gameWindowWidth  = TILES_WIDTH;
        const int gameWindowHeight = TILES_HEIGHT;

        sf::IntRect gameWindowRect(gameWindowX, gameWindowY, gameWindowWidth, gameWindowHeight);

        const int miniMapWindowX = gameWindowX + gameWindowWidth + 32;
        const int miniMapWindowY = 32;

        const int miniMapWindowWidth  = 128;
        const int miniMapWindowHeight = 88;

        sf::IntRect miniMapWindowRect(miniMapWindowX, miniMapWindowY, miniMapWindowWidth, miniMapWindowHeight);

        const int creatureBarWidth  = 24;
        const int creatureBarHeight = 2;
    }

    namespace ZAxis
    {
        enum
        {
            floor,
            underGround,
            ground,
            aboveGround,
            ceiling
        };
    }

    namespace Directions
    {
        enum
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
    }

    namespace MovementSpeeds
    {
        const float default = 0.5;
        const float player  = 0.01; // 0.01
    }

    namespace Teams
    {
        enum
        {
            neutral,
            good,
            evil
        };
    }

    namespace TileMapTypes
    {
        enum
        {
            tiles,
            tileEdges
        };
    }

    namespace Outfits
    {
        std::vector<int> newbie = {0, 0, 0, 0};

        // 10
        std::vector<int> head = 
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
        std::vector<int> body = 
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
        std::vector<int> legs = 
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
        std::vector<int> feet = 
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

    namespace CreatureTypes
    {
        enum
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
    }

    namespace CreatureSizes
    {
        enum
        {
            small,
            medium,
            large
        };
    }

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

    namespace AnimationTimes
    {
        const float default     = 0.1;
        const float decal       = 60.0;
        const float corpseDecay = 60.0;
    }

    namespace SpriteFlags
    {
        enum
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
    }

    std::unordered_map<int, int> spriteFlags; // <int id, int flags>

    namespace SpriteData
    {
        const int guiTextIcons[] = {529, 530, 531, 532, 533, 534};

        const int corpse[] = {491, 492, 493, 494, 495, 496, 497};

        const int lever[] = {49, 50};

        const int stepTileStone[] = {398,  399};
        const int stepTileWood[]  = {1275, 1276};

        const int stepTileGrassHole[] = {478, 479};

        const int trough[]   = {52, 51, 685, 686, 687, 688, 689, 690};
        const int bucket[]   = {310, 678, 679, 680, 681, 682, 683, 684};
        const int cauldron[] = {1181, 1182, 1183, 1184, 1187, 1188, 1189, 1190};
        const int bottle[]   = {1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563};

        const int parcel[] = {1268, 1269};
        const int letter[] = {1270, 1271};

        const int label   = 1272;
        const int depot   = 1273;
        const int mailBox = 1274;

        const int blankRune = 1277;

        const int fishingRod = 1470;

        const int dustBin = 1472;

        const int presentBox = 2160;

        const int ladder = 455;

        const int stairs = 460;

        const int treeWall = 3344;

        std::vector<int> magicWalls = {3271, 3275, 3279};

        std::vector<int> water =
        {
            106, 107, 108, 109,
            110, 111, 112, 113
        };

        const int waterBegin = water.front();
        const int waterEnd   = water.back();

        std::vector<int> lava =
        {
            1785, 1786, 1787, 1788,
            1789, 1790, 1791, 1792
        };

        const int lavaBegin = lava.front();
        const int lavaEnd   = lava.back();

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
            3375
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
            3322, 3326,
            3380, 3381
        };

        // right to left, bottom then top
        std::vector<int> quadObjects =
        {
            13, 21, 29,
            56, 60, 68, 72,
            159, 163,
            286, 290,
            315, // 5 sprites
            388, 455, 460, 486, 562, 703, 707,
            566, 570, 576, 580, 584, 588,
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

        // bottom to top, right then left
        std::vector<int> quadVerticalObjects =
        {
            25
        };

        // right to left
        std::vector<int> horizontalObjects =
        {
            178, 182, 192,
            376,
            609,
            2497,
            2969,
            3030, 3032, 3034
        };

        // bottom to top
        std::vector<int> verticalObjects =
        {
            180, 186,
            334,
            997, 999, 1001,
            1349,
            2499,
            2965
        };

        // fix drawing order of objects on walls
        std::vector<int> fixDrawOrderObjects =
        {
            16, // brick wall arch
            2867, 2868, 2869, 2870, 2871, 2872 // wall inset torches
        };

        std::vector<int> animatedObjects =
        {
            197, 198, 199,
            202, 367,
            203, 368,
            204, 369,
            205, 370,
            395, 396,
            443, 444,
            446, 447,
            449, 450,
            525, 526,
            890, 896,
            891, 897,
            892, 898,
            893, 899,
            894, 900,
            895, 901,
            902, 911,
            903, 912,
            904, 913,
            905, 914,
            906, 915,
            907, 916,
            908, 917,
            909, 918,
            910, 919,
            1123, 1124,
            1154, 1156,
            1489, 1490,
            1491, 1492,
            1493, 1494,
            1497, 1498,
            1570, 1571, 1572, 1573, 1574,
            1582, 1583, 1584, 1585, 1586,
            1879, 1880,
            2127, 2131,
            2664, 2665, 2666, 2667,
            2668, 2669,
            2670, 2671,
            2672, 2673,
            2675, 2676, 2677, 2678,
            2679, 2680,
            2681, 2682,
            2683, 2684,
            2686, 2687, 2688, 2689, 2690, 2691,
            2693, 2694,
            2695, 2696,
            2697, 2698,
            2868, 2869,
            2871, 2872,
            2968, 2970,
            2974, 2975,
            3158, 3159, 3160, 3161,
            3178, 3179, 3180, 3181, 3182, 3183,
            3271, 3275, 3279,
            3322, 3326,
            3380, 3381
        };
    }

    std::vector<std::vector<int>> animatedObjectsList =
    {
        {197, 198, 199},
        {202, 367},
        {203, 368},
        {204, 369},
        {205, 370},
        {395, 396},
        {443, 444},
        {446, 447},
        {449, 450},
        {525, 526},
        {890, 896},
        {891, 897},
        {892, 898},
        {893, 899},
        {894, 900},
        {895, 901},
        {902, 911},
        {903, 912},
        {904, 913},
        {905, 914},
        {906, 915},
        {907, 916},
        {908, 917},
        {909, 918},
        {910, 919},
        {1123, 1124},
        {1154, 1156},
        {1489, 1490},
        {1491, 1492},
        {1493, 1494},
        {1497, 1498},
        {1570, 1571, 1572, 1573, 1574},
        {1582, 1583, 1584, 1585, 1586},
        {1879, 1880},
        {2127, 2131},
        {2664, 2665, 2666, 2667},
        {2668, 2669},
        {2670, 2671},
        {2672, 2673},
        {2675, 2676, 2677, 2678},
        {2679, 2680},
        {2681, 2682},
        {2683, 2684},
        {2686, 2687, 2688, 2689, 2690, 2691},
        {2693, 2694},
        {2695, 2696},
        {2697, 2698},
        {2868, 2869},
        {2871, 2872},
        {2968, 2970},
        {2974, 2975},
        {3158, 3159, 3160, 3161},
        {3178, 3179, 3180, 3181, 3182, 3183},
        {3271, 3275, 3279},
        {3322, 3326},
        {3380, 3381}
    };

} // tibia

#endif // TIBIA_TIBIA_HPP

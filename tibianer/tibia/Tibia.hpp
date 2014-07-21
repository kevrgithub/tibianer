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
    const int TEXTURE_SIZE_MAX = 4096;

    const int SPRITES_TOTAL = 3390;

    const int NUM_Z_LEVELS = 5;

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

    const int TILE_HEIGHT_MOVEMENT_DIFFERENCE = 2;

    const int THING_DRAW_OFFSET = 8;

    const int LIGHT_WIDTH  = 480;
    const int LIGHT_HEIGHT = 352;

    const int NUM_OBJECT_SPRITES = 8;

    const std::string CREATURE_NAME_DEFAULT = "Creature";
    const std::string CREATURE_NAME_PLAYER  = "Player";

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
        std::string console = "fonts/Inconsolata.ttf";
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
        std::string modern  = "images/font3.png";

        sf::Vector2u defaultGlyphSize(18, 19);

        // 16x6
        std::vector<int> defaultGlyphWidths =
        {
            6,  4,  8,  16, 9,  18, 15, 5,  7,  7,  9,  13, 5,  8,  4,  7,
            10, 8,  10, 10, 10, 9,  10, 10, 9,  10, 4,  5,  12, 14, 12, 9,
            14, 15, 13, 13, 14, 13, 12, 15, 15, 8,  9,  15, 13, 18, 15, 14,
            12, 14, 15, 11, 14, 15, 15, 18, 15, 15, 13, 15, 14, 15, 11, 13,
            4,  9,  11, 9,  11, 9,  10, 11, 11, 6,  7,  11, 6,  16, 11, 10,
            11, 11, 8,  8,  7,  11, 11, 15, 11, 12, 10, 9,  10, 11, 14, 11,
        };

        sf::Vector2u tinyGlyphSize(7, 10);

        std::vector<int> tinyGlyphWidths =
        {
            6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 3, 6, 3, 7,
            6, 5, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 7, 7, 7, 7,
            7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 6, 7, 6, 7, 7, 6,
            6, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
            7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 6, 7, 6, 7, 7, 6,
            6, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        };

        sf::Vector2u modernGlyphSize(16, 12);

        std::vector<int> modernGlyphWidths =
        {
            3,  4, 7, 9, 8,  10, 10, 4,  6, 6, 9, 9, 4, 9,  4,  8,
            8,  6, 8, 8, 8,  8,  8,  8,  8, 8, 4, 4, 9, 9,  9,  7,
            10, 9, 8, 8, 9,  8,  8,  9,  9, 6, 7, 8, 8, 10, 9,  9,
            8,  9, 9, 8, 10, 9,  8,  10, 8, 8, 8, 6, 8, 6,  10, 10,
            4,  8, 8, 7, 8,  8,  7,  8,  8, 4, 6, 8, 4, 10, 8,  8,
            8,  8, 7, 7, 7,  8,  8,  10, 8, 8, 7, 8, 4, 8,  9,  8,
        };
    }

    namespace MouseCursor
    {
        int width  = 32;
        int height = 32;
    }

    namespace MouseCursorTypes
    {
        enum
        {
            point,
            click,
            open,
            closed,
        };
    }

    namespace GameTextData
    {
        const float time = 5.0;
    }

    namespace FloatingTextData
    {
        const float time  = 1.0;
        const float speed = 60.0;
    }

    namespace StatusBarTextData
    {
        const float time = 5.0;

        sf::Vector2f position(240, 307); // 308 - 1
    }

    namespace AnimatedObjects
    {
        const float time = 1.0;
    }

    namespace DecayObjects
    {
        const float time = 30.0;
    }

    namespace Lights
    {
        int numLights = 4;

        sf::Vector2u size(480, 352);
    }

    namespace LightSizes
    {
        enum
        {
            small,
            medium,
            large,
            max,
        };
    }

    namespace LightBrightnesses
    {
        const int min = 32;
        const int max = 255;

        const int underground = min;
        const int day         = max;
        const int night       = min * 2;

        const float updateTime = 1;
    }

    namespace TimeOfDay
    {
        enum
        {
            day,
            night,
        };
    }

    std::unordered_map<std::string, int> umapTimeOfDay
    {
        {"day",   tibia::TimeOfDay::day},
        {"night", tibia::TimeOfDay::night},
    };

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

        sf::Color mouseCursorRed(255, 128, 128);
        sf::Color mouseCursorGreen(128, 255, 128);
        sf::Color mouseCursorBlue(128, 255, 255);
        sf::Color mouseCursorYellow(255, 255, 128);
        sf::Color mouseCursorTeal(128, 255, 255);
        sf::Color mouseCursorPink(255, 128, 255);

        sf::Color textWhite(255, 255, 255);
        sf::Color textRed(255, 64, 64);
        sf::Color textGreen(64, 255, 64);
        sf::Color textYellow(255, 255, 64);
        sf::Color textOrange(255, 128, 64);
        
        sf::Color mainWindowColor = white;
        sf::Color windowBorderColor = black;

        sf::Color miniMapDefault(153, 153, 153);
        sf::Color miniMapMountain(102, 102, 102);
        sf::Color miniMapSolid(255, 51, 0);
        sf::Color miniMapCave(153, 51, 0);
        sf::Color miniMapWood(128, 64, 0);
        sf::Color miniMapDirt(153, 102, 51);
        sf::Color miniMapGrass(0, 204, 0);
        sf::Color miniMapTree(0, 102, 0);
        sf::Color miniMapWater(51, 0, 204);
        sf::Color miniMapShallowWater(64, 128, 192);
        sf::Color miniMapLava(255, 128, 0);
        sf::Color miniMapSwamp(0, 255, 0);
        sf::Color miniMapIce(128, 255, 255);
        sf::Color miniMapSand(255, 255, 128);
        sf::Color miniMapMoveAboveOrBelow(255, 255, 0);
    }

    std::unordered_map<int, sf::Color> umapMiniMapColors =
    {
        {2,    tibia::Colors::miniMapWood},
        {38,   tibia::Colors::miniMapDirt},
        {43,   tibia::Colors::miniMapGrass},
        {116,  tibia::Colors::miniMapShallowWater},
        {118,  tibia::Colors::miniMapDirt},
        {119,  tibia::Colors::miniMapDirt},
        {120,  tibia::Colors::miniMapDirt},
        {121,  tibia::Colors::miniMapDirt},
        {122,  tibia::Colors::miniMapDirt},
        {123,  tibia::Colors::miniMapDirt},
        {124,  tibia::Colors::miniMapDirt},
        {125,  tibia::Colors::miniMapDirt},
        {126,  tibia::Colors::miniMapDirt},
        {127,  tibia::Colors::miniMapIce},
        {193,  tibia::Colors::miniMapDirt},
        {194,  tibia::Colors::miniMapDirt},
        {195,  tibia::Colors::miniMapDirt},
        {196,  tibia::Colors::miniMapDirt},
        {388,  tibia::Colors::miniMapWood},
        {461,  tibia::Colors::miniMapDirt},
        {462,  tibia::Colors::miniMapDirt},
        {463,  tibia::Colors::miniMapDirt},
        {464,  tibia::Colors::miniMapDirt},
        {465,  tibia::Colors::miniMapDirt},
        {466,  tibia::Colors::miniMapCave},
        {467,  tibia::Colors::miniMapCave},
        {468,  tibia::Colors::miniMapCave},
        {469,  tibia::Colors::miniMapCave},
        {470,  tibia::Colors::miniMapCave},
        {471,  tibia::Colors::miniMapCave},
        {472,  tibia::Colors::miniMapCave},
        {473,  tibia::Colors::miniMapCave},
        {474,  tibia::Colors::miniMapCave},
        {475,  tibia::Colors::miniMapCave},
        {476,  tibia::Colors::miniMapCave},
        {477,  tibia::Colors::miniMapCave},
        {1060, tibia::Colors::miniMapTree},
        {1066, tibia::Colors::miniMapMountain},
        {1067, tibia::Colors::miniMapMountain},
        {1068, tibia::Colors::miniMapMountain},
        {1069, tibia::Colors::miniMapMountain},
        {1070, tibia::Colors::miniMapMountain},
        {1075, tibia::Colors::miniMapGrass},
        {1076, tibia::Colors::miniMapGrass},
        {1077, tibia::Colors::miniMapGrass},
        {1078, tibia::Colors::miniMapGrass},
        {1079, tibia::Colors::miniMapGrass},
        {1080, tibia::Colors::miniMapGrass},
        {1081, tibia::Colors::miniMapGrass},
        {1082, tibia::Colors::miniMapGrass},
        {1083, tibia::Colors::miniMapGrass},
        {1084, tibia::Colors::miniMapGrass},
        {1085, tibia::Colors::miniMapGrass},
        {1086, tibia::Colors::miniMapGrass},
        {1092, tibia::Colors::miniMapCave},
        {1093, tibia::Colors::miniMapCave},
        {1094, tibia::Colors::miniMapCave},
        {1095, tibia::Colors::miniMapCave},
        {1096, tibia::Colors::miniMapCave},
        {1097, tibia::Colors::miniMapCave},
        {1098, tibia::Colors::miniMapCave},
        {1099, tibia::Colors::miniMapCave},
        {1100, tibia::Colors::miniMapCave},
        {1101, tibia::Colors::miniMapCave},
        {1102, tibia::Colors::miniMapCave},
        {1103, tibia::Colors::miniMapCave},
        {1140, tibia::Colors::miniMapTree},
        {1144, tibia::Colors::miniMapTree},
        {1152, tibia::Colors::miniMapWood},
        {1160, tibia::Colors::miniMapWood},
        {1164, tibia::Colors::miniMapTree},
        {1168, tibia::Colors::miniMapTree},
        {1172, tibia::Colors::miniMapTree},
        {1176, tibia::Colors::miniMapTree},
        {1180, tibia::Colors::miniMapTree},
        {1185, tibia::Colors::miniMapTree},
        {1186, tibia::Colors::miniMapWood},
        {1191, tibia::Colors::black},
        {1203, tibia::Colors::miniMapTree},
        {1495, tibia::Colors::miniMapCave},
        {1496, tibia::Colors::miniMapCave},
        {1564, tibia::Colors::miniMapSwamp},
        {1565, tibia::Colors::miniMapCave},
        {1566, tibia::Colors::miniMapCave},
        {1567, tibia::Colors::miniMapCave},
        {1610, tibia::Colors::miniMapTree},
        {1614, tibia::Colors::miniMapTree},
        {1618, tibia::Colors::miniMapTree},
        {1622, tibia::Colors::miniMapTree},
        {1623, tibia::Colors::miniMapWood},
        {1624, tibia::Colors::miniMapWood},
        {1625, tibia::Colors::miniMapWood},
        {1626, tibia::Colors::miniMapWood},
        {1630, tibia::Colors::miniMapTree},
        {1634, tibia::Colors::miniMapTree},
        {1635, tibia::Colors::miniMapTree},
        {1636, tibia::Colors::miniMapWood},
        {1637, tibia::Colors::miniMapTree},
        {1638, tibia::Colors::miniMapTree},
        {1651, tibia::Colors::miniMapTree},
        {1669, tibia::Colors::miniMapTree},
        {1676, tibia::Colors::miniMapTree},
        {1680, tibia::Colors::miniMapWood},
        {1684, tibia::Colors::miniMapWood},
        {1688, tibia::Colors::miniMapWood},
        {1692, tibia::Colors::miniMapWood},
        {1760, tibia::Colors::miniMapWood},
        {1764, tibia::Colors::miniMapWood},
        {1768, tibia::Colors::miniMapTree},
        {1772, tibia::Colors::miniMapTree},
        {1993, tibia::Colors::miniMapTree},
        {1994, tibia::Colors::miniMapTree},
        {1995, tibia::Colors::miniMapTree},
        {1996, tibia::Colors::miniMapTree},
        {1997, tibia::Colors::miniMapTree},
        {1998, tibia::Colors::miniMapTree},
        {1999, tibia::Colors::miniMapTree},
        {2000, tibia::Colors::miniMapTree},
        {2001, tibia::Colors::miniMapTree},
        {2002, tibia::Colors::miniMapTree},
        {2003, tibia::Colors::miniMapTree},
        {2004, tibia::Colors::miniMapTree},
        {2005, tibia::Colors::miniMapTree},
        {2006, tibia::Colors::miniMapTree},
        {2007, tibia::Colors::miniMapTree},
        {2008, tibia::Colors::miniMapTree},
        {2009, tibia::Colors::miniMapTree},
        {2010, tibia::Colors::miniMapTree},
        {2011, tibia::Colors::miniMapTree},
        {2012, tibia::Colors::miniMapTree},
        {2013, tibia::Colors::miniMapTree},
        {2014, tibia::Colors::miniMapTree},
        {2015, tibia::Colors::miniMapTree},
        {2016, tibia::Colors::miniMapTree},
        {2017, tibia::Colors::miniMapTree},
        {2018, tibia::Colors::miniMapTree},
        {2019, tibia::Colors::miniMapTree},
        {2020, tibia::Colors::miniMapTree},
        {2021, tibia::Colors::miniMapSand},
        {2022, tibia::Colors::miniMapSand},
        {2023, tibia::Colors::miniMapSand},
        {2024, tibia::Colors::miniMapSand},
        {2025, tibia::Colors::miniMapSand},
        {2026, tibia::Colors::miniMapSand},
        {2027, tibia::Colors::miniMapSand},
        {2028, tibia::Colors::miniMapSand},
        {2029, tibia::Colors::miniMapSand},
        {2030, tibia::Colors::miniMapSand},
        {2031, tibia::Colors::miniMapSand},
        {2032, tibia::Colors::miniMapSand},
        {2033, tibia::Colors::miniMapSand},
        {2034, tibia::Colors::miniMapShallowWater},
        {2035, tibia::Colors::miniMapShallowWater},
        {2036, tibia::Colors::miniMapShallowWater},
        {2037, tibia::Colors::miniMapShallowWater},
        {2038, tibia::Colors::miniMapShallowWater},
        {2039, tibia::Colors::miniMapShallowWater},
        {2040, tibia::Colors::miniMapShallowWater},
        {2041, tibia::Colors::miniMapShallowWater},
        {2042, tibia::Colors::miniMapShallowWater},
        {2043, tibia::Colors::miniMapShallowWater},
        {2044, tibia::Colors::miniMapShallowWater},
        {2045, tibia::Colors::miniMapShallowWater},
        {2046, tibia::Colors::miniMapShallowWater},
        {2123, tibia::Colors::miniMapTree},
        {2127, tibia::Colors::miniMapTree},
        {2131, tibia::Colors::miniMapTree},
        {2164, tibia::Colors::miniMapTree},
        {2168, tibia::Colors::miniMapTree},
        {2172, tibia::Colors::miniMapTree},
        {2185, tibia::Colors::miniMapSand},
        {2186, tibia::Colors::miniMapSand},
        {2187, tibia::Colors::miniMapSand},
        {2188, tibia::Colors::miniMapSand},
        {2191, tibia::Colors::miniMapSand},
        {2192, tibia::Colors::miniMapSand},
        {2193, tibia::Colors::miniMapSand},
        {2194, tibia::Colors::miniMapSand},
        {3254, tibia::Colors::miniMapMountain},
        {3255, tibia::Colors::miniMapMountain},
        {3256, tibia::Colors::miniMapMountain},
        {3257, tibia::Colors::miniMapMountain},
        {3322, tibia::Colors::miniMapTree},
        {3326, tibia::Colors::miniMapTree},
        {3344, tibia::Colors::miniMapWood},
        {3375, tibia::Colors::miniMapTree},
    };

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

    namespace MiniMapWindow
    {
        const int zoomMin     = 1;
        const int zoomDefault = 2;
        const int zoomMax     = 4;
    }

    namespace ZAxis
    {
        enum
        {
            floor,
            underGround,
            ground,
            aboveGround,
            ceiling,
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
            end   = downLeft,

            null,
        };
    }

    std::unordered_map<std::string, int> umapDirections
    {
        {"up",    tibia::Directions::up},
        {"right", tibia::Directions::right},
        {"down",  tibia::Directions::down},
        {"left",  tibia::Directions::left},
    };

    namespace MovementSpeeds
    {
        const float default = 0.5;
        const float player  = 0.1; // 0.2
    }

    namespace Teams
    {
        enum
        {
            neutral,
            good,
            evil,
        };
    }

    std::unordered_map<std::string, int> umapTeams
    {
        {"neutral", tibia::Teams::neutral},
        {"good",    tibia::Teams::good},
        {"evil",    tibia::Teams::evil},
    };

    namespace TileMapTypes
    {
        enum
        {
            tiles,
            tileEdges,
        };
    }

    namespace ObjectLayerTypes
    {
        enum
        {
            objects,
            creatures,
        };
    }

    namespace ObjectTypes
    {
        enum
        {
            null,
            sign,
            teleporter,
            door,
            bed,
            lever,
            changeMap,
            creature,
            book,
        };
    }

    std::unordered_map<std::string, int> umapObjectTypes
    {
        {"null",       tibia::ObjectTypes::null},
        {"sign",       tibia::ObjectTypes::sign},
        {"teleporter", tibia::ObjectTypes::teleporter},
        {"door",       tibia::ObjectTypes::door},
        {"bed",        tibia::ObjectTypes::bed},
        {"lever",      tibia::ObjectTypes::lever},
        {"changemap",  tibia::ObjectTypes::changeMap},
        {"creature",   tibia::ObjectTypes::creature},
        {"book",       tibia::ObjectTypes::book},
    };

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

    namespace CreatureStatusFlags
    {
        //
    }

    namespace CreatureEquipmentFlags
    {
        //
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
            zombie,
        };
    }

    std::unordered_map<std::string, int> umapCreatureTypes =
    {
        {"human", tibia::CreatureTypes::human},
        {"demon", tibia::CreatureTypes::demon},
        {"santa_claus", tibia::CreatureTypes::santaClaus},
    };

    std::unordered_map<int, std::string> umapCreatureNames =
    {
        {tibia::CreatureTypes::human, "Human"},
        {tibia::CreatureTypes::demon, "Demon"},
        {tibia::CreatureTypes::santaClaus, "Santa Claus"},
    };

    namespace CreatureSizes
    {
        enum
        {
            small,
            medium,
            large,
        };
    }

    std::unordered_map<int, int> umapCreatureSizes =
    {
        {tibia::CreatureTypes::human,      tibia::CreatureSizes::small},
        {tibia::CreatureTypes::demon,      tibia::CreatureSizes::large},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureSizes::medium},
    };

    namespace CreatureSprites
    {
        std::vector<int> demon =
        {
            1480, 1484, 1476, 1488,
            1479, 1483, 1475, 1487,
            1478, 1482, 1474, 1486,
            1477, 1481, 1473, 1485,
        };

        std::vector<int> hero = {1645, 1646, 1643, 1644};

        std::vector<int> gameMaster = {3330, 3329, 3327, 3328};

        std::vector<int> orc = {2863, 2862, 2864, 2861};

        std::vector<int> santaClaus =
        {
            2431, 2437, 2433, 2435,
            2430, 2436, 2432, 2434,
        };

        std::vector<int> skeleton = {1404, 1407, 1405, 1406};

        std::vector<int> witch =
        {
            2884, 2888, 2882, 2886,
            2883, 2887, 2881, 2885,
        };

        std::vector<int> zombie = {1838, 1841, 1839, 1840};
    }

    std::unordered_map<int, std::vector<int>> umapCreatureSprites =
    {
        {tibia::CreatureTypes::demon,      tibia::CreatureSprites::demon},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureSprites::santaClaus},
    };

    namespace CreatureCorpseSprites
    {
        std::vector<int> human = {491, 492, 493, 494, 495, 496, 497};

        std::vector<int> demon =
        {
            319, 318, 317, 316,
            323, 322, 321, 320,
            327, 326, 325, 324,
            331, 330, 329, 328,
        };
    }

    std::unordered_map<int, std::vector<int>> umapCreatureCorpseSprites =
    {
        {tibia::CreatureTypes::human, tibia::CreatureCorpseSprites::human},
        {tibia::CreatureTypes::demon, tibia::CreatureCorpseSprites::demon},
    };

    namespace AnimationTimes
    {
        const float default     = 0.1;
        const float decal       = 60.0;
        const float corpseDecay = 60.0;
    }

    namespace Animations
    {
        // {id, numFrames}

        std::vector<int> waterSplash = {335, 4};

        std::vector<int> hitBlood  = {363, 4};
        std::vector<int> hitMiss   = {920, 4};
        std::vector<int> hitBlock  = {924, 3};
        std::vector<int> hitBlack  = {1388, 8};
        std::vector<int> hitPoison = {2132, 4};

        std::vector<int> urine =  {1374, 7};
        std::vector<int> poison = {1381, 7};

        std::vector<int> spellBlue  = {1396, 8};
        std::vector<int> spellBlack = {3244, 8};

        std::vector<int> fire        = {1489, 2};
        std::vector<int> electricity = {1497, 2};

        std::vector<int> particlesBlue  = {1499, 5};
        std::vector<int> particlesRed   = {1504, 5};
        std::vector<int> particlesGreen = {1509, 5};

        std::vector<int> bubbleGreen = {1570, 5};
        std::vector<int> bubbleRed   = {1582, 5};

        std::vector<int> music = {3362, 5};

        std::vector<int>sparkle = {3376, 4};
    }

    namespace SpriteFlags
    {
        enum : unsigned int
        {
            null             = 1 << 0,
            solid            = 1 << 1,
            blockProjectiles = 1 << 2,
            offset           = 1 << 3,
            lightSource      = 1 << 4,
            water            = 1 << 5,
            lava             = 1 << 6,
            hasHeight        = 1 << 7,
            moveAbove        = 1 << 8,
            moveBelow        = 1 << 9,
            interactive      = 1 << 10,
            drawLast         = 1 << 11,
            transparent      = 1 << 12,
            animated         = 1 << 13,
            ignoreHeight     = 1 << 14,
            fixDrawOrder     = 1 << 15,
        };
    }

    // <id, flags>
    std::unordered_map<unsigned int, unsigned int> umapSpriteFlags;

    namespace SpriteData
    {
        std::vector<int> guiTextIcons = {529, 530, 531, 532, 533, 534};

        std::vector<int> corpse = {491, 492, 493, 494, 495, 496, 497};

        std::vector<int> counterVertical   = {1043, 1046};
        std::vector<int> counterHorizontal = {1044, 1048};

        std::vector<int> streetLamp = {486, 3385};

        std::vector<int> sign = {379, 380, 381, 382, 383, 384};

        std::vector<int> signTall = {3310, 3313};

        std::vector<int> chalkboard = {609, 2965};

        std::vector<int> doorVertical   = {549, 553};
        std::vector<int> doorHorizontal = {555, 558};

        std::vector<int> doorLockedVertical   = {3303, 3307};
        std::vector<int> doorLockedHorizontal = {3295, 3299};

        std::vector<int> lever = {49, 50};

        std::vector<int> torchBig    = {443, 444, 445};
        std::vector<int> torchMedium = {446, 447, 448};
        std::vector<int> torchSmall  = {449, 450, 451};

        std::vector<int> torchInsetHorizontal = {2867, 2868, 2869};
        std::vector<int> torchInsetVertical   = {2870, 2871, 2872};

        std::vector<int> torchHangingHorizontal = {2971, 2970, 2968};
        std::vector<int> torchHangingVertical   = {2972, 2975, 2974};

        std::vector<int> candleStick  = {394, 395, 396};
        std::vector<int> candleHolder = {524, 525, 526};

        std::vector<int> lantern = {1938, 1879, 1880};

        std::vector<int> digHole    = {1204, 1205};
        std::vector<int> digHoleIce = {3242, 3243};

        std::vector<int> stepTileStone     = {398,  399};
        std::vector<int> stepTileWood      = {1275, 1276};
        std::vector<int> stepTileGrassHole = {478, 479};

        std::vector<int> bedVertical   = {74, 75, 3260, 3261};
        std::vector<int> bedHorizontal = {1147, 1148, 3262, 3263};

        std::vector<int> stretcherVertical   = {1073, 1074, 3264, 3265};
        std::vector<int> stretcherHorizontal = {1071, 1072, 3266, 3267};

        std::vector<int> ovenDown  = {1003, 1154, 1156};
        std::vector<int> ovenLeft  = {1817, 1818, 1819};
        std::vector<int> ovenUp    = {1820, 1821, 1822};
        std::vector<int> ovenRight = {1824, 1826, 1828};

        std::vector<int> trough   = {52, 51, 685, 686, 687, 688, 689, 690};
        std::vector<int> bucket   = {310, 678, 679, 680, 681, 682, 683, 684};
        std::vector<int> cauldron = {1181, 1182, 1183, 1184, 1187, 1188, 1189, 1190};
        std::vector<int> bottle   = {1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563};

        std::vector<int> poolRed = {745, 746, 747};

        std::vector<int> splatRed = {748, 749, 750};

        std::vector<int> locker = {1264, 1265, 1266, 1267};

        std::vector<int> parcel = {1268, 1269};
        std::vector<int> letter = {1270, 1271};

        std::vector<int> magicWall = {3271, 3275, 3279};

        const int label   = 1272;
        const int depot   = 1273;
        const int mailBox = 1274;

        const int bush          = 1060;
        const int bushBlueberry = 3375;

        const int runeBlank = 1277;

        const int fishingRod = 1470;

        const int dustBin = 1472;

        const int presentBox = 2160;

        const int ladder = 455;

        const int stairs = 460;

        const int sewerGrate = 607;

        const int ropeUp = 3253;

        const int treeWall = 3344;

        const int mountainRampLeft  = 2103;
        const int mountainRampRight = 2097;

        const int mountainRampLeftMoveBelow  = 3386;
        const int mountainRampRightMoveBelow = 3387;

        std::vector<int> water =
        {
            106, 107, 108, 109,
            110, 111, 112, 113,
        };

        const int waterBegin = water.front();
        const int waterEnd   = water.back();

        std::vector<int> lava =
        {
            1785, 1786, 1787, 1788,
            1789, 1790, 1791, 1792,
        };

        const int lavaBegin = lava.front();
        const int lavaEnd   = lava.back();

        std::vector<int> solid =
        {
            1,
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
            165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
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
            549, 555,
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
            3375,
            3385,
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
            3348,
            3385,
        };

        std::vector<int> hasHeight =
        {
            37,
            39,
            44,
            46,
            51, 52,
            74, 75,
            88,
            114,
            129, 130, 131, 132, 133, 134, 135,
            159, 163,
            201,
            202, 203, 204, 205,
            298, 299,
            334,
            367, 368, 369, 370,
            376, 378,
            379, 380, 381, 382, 383, 384,
            390, 391, 392, 393,
            521, 522, 523,
            685, 686, 687, 688, 689, 690,
            961, 962,
            997, 999, 1001,
            1003,
            1004, 1005, 1009, 1015, 1021, 1025, 1029, 1033, 1038, 1042, 1043, 1044, 1046, 1048,
            1054, 1055,
            1060,
            1071, 1072, 1073, 1074,
            1122,
            1145, 1146,
            1147, 1148,
            1154, 1156,
            1181, 1182, 1183, 1184,
            1187, 1188, 1189, 1190,
            1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199,
            1203,
            1225,
            1264, 1265, 1266, 1267,
            1268, 1269,
            1273, 1274,
            1349, 1351,
            1465, 1466, 1467, 1468,
            1472,
            1495, 1496,
            1565, 1566,
            1591, 1592, 1593, 1594, 1595, 1596, 1597, 1598,
            1603, 1604,
            1639, 1640, 1641, 1642,
            1651,
            1670, 1671,
            1817, 1818, 1819, 1820, 1821, 1822,
            1824, 1826, 1828,
            1924, 1925, 1926, 1927,
            1939, 1940, 1941, 1942, 1943, 1944, 1945, 1946,
            2098, 2102,
            2664, 2665, 2666, 2667, 2668, 2669, 2670, 2671, 2672, 2673, 2674, 2675, 2676, 2677, 2678, 2679, 2680, 2681, 2682, 2683, 2684, 2685,
            2692, 2693, 2694,
            2704, 2706, 2708, 2710, 2711, 2714, 2717, 2718,
            2849, 2850, 2851, 2852, 2853, 2854, 2855, 2856, 2857, 2858, 2859, 2860,
            3030, 3032, 3034,
            3035,
            3184,
            3239, 3240,
            3260, 3261, 3262, 3263, 3264, 3265, 3266, 3267,
            3375,
        };

        std::vector<int> offsetObjects =
        {
            88,
            394, 395, 396, 397,
            524, 525, 526, 527,
            1004, 1005, 1043, 1044, 1046, 1048,
            1274,
            1652,
            1879, 1880,
            3348,
        };

        std::vector<int> moveAbove =
        {
            460,
            2097, 2103,
        };

        std::vector<int> moveBelow =
        {
            47, 456, 478, 479, 480, 481, 487,
            610, 611,
            691, 692, 693, 694, 695, 696, 697, 698, 699,
            1205,
            3241, 3243,
            3386, 3387,
        };

        std::vector<int> lightSource =
        {
            197, 198, 199,
            395, 396,
            443, 444, 446, 447, 449, 450,
            489,
            525, 526,
            703, 707,
            1049,
            1123, 1124,
            1154, 1156,
            1396, 1397, 1398, 1399, 1400, 1401, 1402, 1403,
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
            2968, 2970,
            2974, 2975,
            3178, 3179, 3180, 3181, 3182, 3183,
            3271, 3275, 3279,
            3322, 3326,
            3380, 3381,
            3385,
        };

        std::vector<int> interactive =
        {
            counterVertical[0],
            counterVertical[1],

            counterHorizontal[0],
            counterHorizontal[1],

            tibia::SpriteData::ovenDown[0],
            tibia::SpriteData::ovenDown[1],
            tibia::SpriteData::ovenDown[2],
            tibia::SpriteData::ovenDown[3],

            tibia::SpriteData::ovenLeft[0],
            tibia::SpriteData::ovenLeft[1],
            tibia::SpriteData::ovenLeft[2],
            tibia::SpriteData::ovenLeft[3],

            tibia::SpriteData::ovenUp[0],
            tibia::SpriteData::ovenUp[1],
            tibia::SpriteData::ovenUp[2],
            tibia::SpriteData::ovenUp[3],

            tibia::SpriteData::ovenRight[0],
            tibia::SpriteData::ovenRight[1],
            tibia::SpriteData::ovenRight[2],
            tibia::SpriteData::ovenRight[3],

            tibia::SpriteData::streetLamp[0],
            tibia::SpriteData::streetLamp[1],

            tibia::SpriteData::torchBig[0],
            tibia::SpriteData::torchBig[1],
            tibia::SpriteData::torchBig[2],

            tibia::SpriteData::torchMedium[0],
            tibia::SpriteData::torchMedium[1],
            tibia::SpriteData::torchMedium[2],

            tibia::SpriteData::torchSmall[0],
            tibia::SpriteData::torchSmall[1],
            //tibia::SpriteData::torchSmall[2], // not used, decays

            tibia::SpriteData::torchInsetHorizontal[0],
            tibia::SpriteData::torchInsetHorizontal[1],
            tibia::SpriteData::torchInsetHorizontal[2],

            tibia::SpriteData::torchInsetVertical[0],
            tibia::SpriteData::torchInsetVertical[1],
            tibia::SpriteData::torchInsetVertical[2],

            tibia::SpriteData::torchHangingHorizontal[0],
            tibia::SpriteData::torchHangingHorizontal[1],
            tibia::SpriteData::torchHangingHorizontal[2],

            tibia::SpriteData::torchHangingVertical[0],
            tibia::SpriteData::torchHangingVertical[1],
            tibia::SpriteData::torchHangingVertical[2],

            tibia::SpriteData::candleStick[0],
            tibia::SpriteData::candleStick[1],
            tibia::SpriteData::candleStick[2],

            tibia::SpriteData::candleHolder[0],
            tibia::SpriteData::candleHolder[1],
            tibia::SpriteData::candleHolder[2],

            tibia::SpriteData::lantern[0],
            tibia::SpriteData::lantern[1],
            tibia::SpriteData::lantern[2],

            tibia::SpriteData::sign[0],
            tibia::SpriteData::sign[1],
            tibia::SpriteData::sign[2],
            tibia::SpriteData::sign[3],
            tibia::SpriteData::sign[4],
            tibia::SpriteData::sign[5],

            tibia::SpriteData::signTall[0],
            tibia::SpriteData::signTall[1],

            tibia::SpriteData::chalkboard[0],
            tibia::SpriteData::chalkboard[1],

            tibia::SpriteData::locker[0],
            tibia::SpriteData::locker[1],
            tibia::SpriteData::locker[2],
            tibia::SpriteData::locker[3],

            tibia::SpriteData::depot,
            tibia::SpriteData::mailBox,

            tibia::SpriteData::lever[0],
            tibia::SpriteData::lever[1],

            tibia::SpriteData::doorVertical[0],
            tibia::SpriteData::doorVertical[1],

            tibia::SpriteData::doorHorizontal[0],
            tibia::SpriteData::doorHorizontal[1],

            tibia::SpriteData::doorLockedVertical[0],
            tibia::SpriteData::doorLockedVertical[1],

            tibia::SpriteData::doorLockedHorizontal[0],
            tibia::SpriteData::doorLockedHorizontal[1],

            tibia::SpriteData::bedVertical[0],
            tibia::SpriteData::bedVertical[1],
            tibia::SpriteData::bedVertical[2],
            tibia::SpriteData::bedVertical[3],

            tibia::SpriteData::bedHorizontal[0],
            tibia::SpriteData::bedHorizontal[1],
            tibia::SpriteData::bedHorizontal[2],
            tibia::SpriteData::bedHorizontal[3],

            tibia::SpriteData::stretcherVertical[0],
            tibia::SpriteData::stretcherVertical[1],
            tibia::SpriteData::stretcherVertical[2],
            tibia::SpriteData::stretcherVertical[3],

            tibia::SpriteData::stretcherHorizontal[0],
            tibia::SpriteData::stretcherHorizontal[1],
            tibia::SpriteData::stretcherHorizontal[2],
            tibia::SpriteData::stretcherHorizontal[3],

            tibia::SpriteData::ladder,
            tibia::SpriteData::ropeUp,

            tibia::SpriteData::sewerGrate,

            tibia::SpriteData::digHole[0],
            tibia::SpriteData::digHoleIce[0],

            tibia::SpriteData::bushBlueberry,
        };

        std::vector<int> transparent =
        {
            1, // pink square
            3386, 3387, // mountain ramps
            3389, // invisible
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
            553, 3303, 3307, // doors
            566, 570, 576, 580, 584, 588,
            937, 947, 951, 960,
            1009, 1015, 1021, 1025, 1029, 1033, 1038, 1042,
            1140, 1144, 1152, 1160, 1164, 1168, 1172, 1176, 1180, 1224,
            1602, 1610, 1614, 1618, 1622, 1630, 1634, 1651, 1669, 1676, 1680, 1684, 1688, 1692,
            1760, 1764, 1768, 1772,
            2123, 2127, 2131, 2152, 2164, 2168, 2172,
            2408, 2486, 2490, 2494, 
            3039, 3043,
            3271, 3275, 3279,
            3322, 3326,
            3344, 3348,
            3385,
        };

        // bottom to top, right then left
        std::vector<int> quadVerticalObjects =
        {
            25,
        };

        // right to left
        std::vector<int> horizontalObjects =
        {
            178, 182, 192,
            376,
            609,
            909, 918,
            940,
            1046,
            1067, 1069,
            1824, 1826, 1828,
            2497,
            2548,
            2969,
            3026,
            3030, 3032, 3034,
        };

        // bottom to top
        std::vector<int> verticalObjects =
        {
            180, 186,
            334,
            953,
            997, 999, 1001,
            1003,
            1048,
            1154, 1156,
            1349,
            2499,
            2965,
            2967,
            3028,
        };

        // fix drawing order of objects on walls
        std::vector<int> fixDrawOrderObjects =
        {
            1663, // wood arch
            2867, 2868, 2869, 2870, 2871, 2872, // torch inset
            2968, 2970, 2971, 2972, 2974, 2975, // torch hanging
        };

        std::vector<int> drawLastObjects =
        {
            16, // brick arch
            553, 558, // opened doors
            1659, 1665, // wood arch
            3299, 3307, // opened locked doors
        };

        std::vector<int> ignoreHeightObjects =
        {
            1043, 1044, 1046, 1048, // counters
        };

        std::vector<int> animatedObjects =
        {
            197, 198, 199,
            202, 367,
            203, 368,
            204, 369,
            205, 370,
            394, 395, 396,
            443, 444, 445,
            446, 447, 448,
            449, 450, 451,
            524, 525, 526,
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
            1003,
            1123, 1124,
            1154, 1156,
            1489, 1490,
            1491, 1492,
            1493, 1494,
            1497, 1498,
            1570, 1571, 1572, 1573, 1574,
            1582, 1583, 1584, 1585, 1586,
            1817, 1818, 1819,
            1820, 1821, 1822,
            1824, 1826, 1828,
            1879, 1880, 1938,
            2127, 2131,
            2664, 2665, 2666, 2667,
            2668, 2669,
            2670, 2671,
            2672, 2673,
            2674,
            2675, 2676, 2677, 2678,
            2679, 2680,
            2681, 2682,
            2683, 2684,
            2685,
            2686, 2687, 2688, 2689, 2690, 2691,
            2693, 2694,
            2695, 2696,
            2697, 2698,
            2700,
            2867, 2868, 2869,
            2870, 2871, 2872,
            2968, 2970, 2971,
            2972, 2974, 2975,
            3158, 3159, 3160, 3161,
            3178, 3179, 3180, 3181, 3182, 3183,
            3271, 3275, 3279,
            3322, 3326,
            3380, 3381,
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
        {1818, 1819},
        {1821, 1822},
        {1826, 1828},
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
        {3380, 3381},
    };

    std::vector<std::vector<int>> decayObjectsList =
    {
        // torches
        {443, 446, 449, 451},
        {444, 447, 450, 451},

        // pools and splats
        {745, 746, 747},
        {748, 749, 750},
        {751, 752, 753},
        {754, 755, 756},
        {757, 758, 759},
        {760, 761, 762},
        {763, 764, 765},
        {766, 767, 768},
        {769, 770, 771},
        {772, 773, 774},
        {775, 776, 777},
        {778, 779, 780},
        {781, 782, 783},
        {784, 785, 786},

        // fire field
        {1489, 1491, 1493},
        {1490, 1492, 1494},

        // electricity field
        {1497, 1498},

        // poison field
        {3158, 3159, 3160, 3161},

        // magic walls
        {3271},
        {3275},
        {3279},

        // tree wall
        {3344},
    };

} // tibia

#endif // TIBIA_TIBIA_HPP

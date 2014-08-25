#ifndef TIBIA_TIBIA_HPP
#define TIBIA_TIBIA_HPP

#include <cmath>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <SFML/Graphics.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

namespace tibia
{
    const unsigned int TEXTURE_SIZE_MAX = 2048; // video card maximum supported texture size

    const int SPRITES_TOTAL = 4096;

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

    const int TILE_HEIGHT_MAX = 4; // 5 - 1

    const int TILE_CLIMB_HEIGHT = 3;

    const int THING_DRAW_OFFSET = 8;

    const int LIGHT_WIDTH  = 480;
    const int LIGHT_HEIGHT = 352;

    const int NUM_CREATURE_SPRITES = 4;
    const int NUM_OBJECT_SPRITES   = 8;

    const int DRAW_INDEX_LAST = 256;

    const float VOLUME_MULTIPLIER = 12.0f;

    const int INVENTORY_ITEMS_MAX      = 4096; //96;
    const int INVENTORY_ITEM_COUNT_MAX = 100;

    const std::string CREATURE_NAME_DEFAULT = "Creature";
    const std::string CREATURE_NAME_PLAYER  = "Player";

    namespace Textures
    {
        sf::Texture cursor;
        sf::Texture loading;
        sf::Texture sprites;
        sf::Texture spritesNumbered;
        sf::Texture lights;
        sf::Texture lightsSmooth;
        sf::Texture background;
        sf::Texture bars;
        sf::Texture status;
        sf::Texture equipment;
        sf::Texture inventory;
        sf::Texture inventorySplit;
        sf::Texture combat;
        sf::Texture skills;
        sf::Texture book;
    }

    std::unordered_map<std::string, sf::Texture&> umapTextureFiles =
    {
        {"images/cursor.png",           tibia::Textures::cursor},
        {"images/loading.png",          tibia::Textures::loading},
        {"images/sprites.png",          tibia::Textures::sprites},
        {"images/sprites_numbered.png", tibia::Textures::spritesNumbered},
        {"images/lights.png",           tibia::Textures::lights},
        {"images/lights_smooth.png",    tibia::Textures::lightsSmooth},
        {"images/background.png",       tibia::Textures::background},
        {"images/bars.png",             tibia::Textures::bars},
        {"images/status.png",           tibia::Textures::status},
        {"images/equipment.png",        tibia::Textures::equipment},
        {"images/inventory.png",        tibia::Textures::inventory},
        {"images/inventory_split.png",  tibia::Textures::inventorySplit},
        {"images/combat.png",           tibia::Textures::combat},
        {"images/skills.png",           tibia::Textures::skills},
        {"images/book.png",             tibia::Textures::book},
    };

    namespace Fonts
    {
        namespace Default
        {
            std::string filename = "fonts/OpenSans.ttf";
        }

        namespace Console
        {
            std::string filename = "fonts/Inconsolata.ttf";
        }

        namespace System
        {
            std::string filename = "fonts/System.ttf";

            unsigned int characterSize = 13;
        }
    }

    namespace BitmapFonts
    {
        namespace Default
        {
            std::string filename = "images/font.png";

            sf::Vector2u glyphSize(18, 19);

            // 16x6
            std::vector<int> glyphWidths =
            {
                6,  4,  8,  16, 9,  18, 15, 5,  7,  7,  9,  13, 5,  8,  4,  7,
                10, 8,  10, 10, 10, 9,  10, 10, 9,  10, 4,  5,  12, 14, 12, 9,
                14, 15, 13, 13, 14, 13, 12, 15, 15, 8,  9,  15, 13, 18, 15, 14,
                12, 14, 15, 11, 14, 15, 15, 18, 15, 15, 13, 15, 14, 15, 11, 13,
                4,  9,  11, 9,  11, 9,  10, 11, 11, 6,  7,  11, 6,  16, 11, 10,
                11, 11, 8,  8,  7,  11, 11, 15, 11, 12, 10, 9,  10, 11, 14, 11,
            };
        }

        namespace Tiny
        {
            std::string filename = "images/font_tiny.png";

            sf::Vector2u glyphSize(7, 10);

            // 16x6
            std::vector<int> glyphWidths =
            {
                4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 3, 6, 3, 7,
                6, 5, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 7, 7, 7, 7,
                7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 6, 7, 6, 7, 7, 6,
                6, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 6, 7, 6, 7, 7, 6,
                6, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
            };
        }

        namespace Modern
        {
            std::string filename = "images/font_modern.png";

            sf::Vector2u glyphSize(16, 12);

            // 16x6
            std::vector<int> glyphWidths =
            {
                3,  4, 7, 9, 8,  10, 10, 4,  6, 6, 9, 9, 4, 9,  4,  8,
                8,  6, 8, 8, 8,  8,  8,  8,  8, 8, 4, 4, 9, 9,  9,  7,
                10, 9, 8, 8, 9,  8,  8,  9,  9, 6, 7, 8, 8, 10, 9,  9,
                8,  9, 9, 8, 10, 9,  8,  10, 8, 8, 8, 6, 8, 6,  10, 10,
                4,  8, 8, 7, 8,  8,  7,  8,  8, 4, 6, 8, 4, 10, 8,  8,
                8,  8, 7, 7, 7,  8,  8,  10, 8, 8, 7, 8, 4, 8,  9,  8,
            };
        }
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

    namespace BarsTypes
    {
        enum
        {
            red,
            blue,
            green,
            purple
        };
    }

    namespace BarsData
    {
        int width  = 202;
        int height = 16;
    }

    namespace GameTextData
    {
        const float time = 5.0f;
    }

    namespace FloatingTextData
    {
        const float time  = 1.0f;
        const float speed = 60.0f;
    }

    namespace StatusBarTextData
    {
        const float time = 5.0f;
    }

    namespace AnimatedObjects
    {
        const float time = 1.0f;
    }

    namespace DecayObjects
    {
        const float time = 30.0f;
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
            tiny,
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

        const float updateTime = 1.0f;
    }

    std::unordered_map<int, sf::Color> umapLightColors =
    {
        {3271, sf::Color(0, 255, 255, 192)},
        {3275, sf::Color(0, 255, 255, 192)},
        {3279, sf::Color(0, 255, 255, 192)},
    };

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

        namespace MouseCursor
        {
            sf::Color white(255, 255, 255);
            sf::Color red(255, 32, 32);
            sf::Color green(128, 255, 128);
            sf::Color blue(128, 255, 255);
            sf::Color yellow(255, 255, 128);
            sf::Color teal(128, 255, 255);
            sf::Color pink(255, 128, 255);
        }

        namespace Text
        {
            sf::Color white(255, 255, 255);
            sf::Color red(255, 64, 64);
            sf::Color green(64, 255, 64);
            sf::Color yellow(255, 255, 64);
            sf::Color orange(255, 128, 64);
            sf::Color teal(64, 255, 255);
        }

        namespace MiniMap
        {
            sf::Color default(153, 153, 153);
            sf::Color mountain(102, 102, 102);
            sf::Color solid(255, 51, 0);
            sf::Color cave(153, 51, 0);
            sf::Color wood(128, 64, 0);
            sf::Color dirt(153, 102, 51);
            sf::Color grass(0, 204, 0);
            sf::Color tree(0, 102, 0);
            sf::Color water(51, 0, 204);
            sf::Color shallowWater(64, 128, 192);
            sf::Color lava(255, 128, 0);
            sf::Color swamp(0, 255, 0);
            sf::Color ice(128, 255, 255);
            sf::Color snow(192, 192, 192);
            sf::Color sand(255, 255, 128);
            sf::Color moveAboveOrBelow(255, 255, 0);
        }
    }

    std::unordered_map<int, sf::Color> umapMiniMapColors =
    {
        {2,    tibia::Colors::MiniMap::wood},
        {38,   tibia::Colors::MiniMap::dirt},
        {43,   tibia::Colors::MiniMap::grass},
        {116,  tibia::Colors::MiniMap::shallowWater},
        {118,  tibia::Colors::MiniMap::dirt},
        {119,  tibia::Colors::MiniMap::dirt},
        {120,  tibia::Colors::MiniMap::dirt},
        {121,  tibia::Colors::MiniMap::dirt},
        {122,  tibia::Colors::MiniMap::dirt},
        {123,  tibia::Colors::MiniMap::dirt},
        {124,  tibia::Colors::MiniMap::dirt},
        {125,  tibia::Colors::MiniMap::dirt},
        {126,  tibia::Colors::MiniMap::dirt},
        {127,  tibia::Colors::MiniMap::ice},
        {193,  tibia::Colors::MiniMap::dirt},
        {194,  tibia::Colors::MiniMap::dirt},
        {195,  tibia::Colors::MiniMap::dirt},
        {196,  tibia::Colors::MiniMap::dirt},
        {388,  tibia::Colors::MiniMap::wood},
        {461,  tibia::Colors::MiniMap::dirt},
        {462,  tibia::Colors::MiniMap::dirt},
        {463,  tibia::Colors::MiniMap::dirt},
        {464,  tibia::Colors::MiniMap::dirt},
        {465,  tibia::Colors::MiniMap::dirt},
        {466,  tibia::Colors::MiniMap::cave},
        {467,  tibia::Colors::MiniMap::cave},
        {468,  tibia::Colors::MiniMap::cave},
        {469,  tibia::Colors::MiniMap::cave},
        {470,  tibia::Colors::MiniMap::cave},
        {471,  tibia::Colors::MiniMap::cave},
        {472,  tibia::Colors::MiniMap::cave},
        {473,  tibia::Colors::MiniMap::cave},
        {474,  tibia::Colors::MiniMap::cave},
        {475,  tibia::Colors::MiniMap::cave},
        {476,  tibia::Colors::MiniMap::cave},
        {477,  tibia::Colors::MiniMap::cave},
        {1060, tibia::Colors::MiniMap::tree},
        {1066, tibia::Colors::MiniMap::mountain},
        {1067, tibia::Colors::MiniMap::mountain},
        {1068, tibia::Colors::MiniMap::mountain},
        {1069, tibia::Colors::MiniMap::mountain},
        {1070, tibia::Colors::MiniMap::mountain},
        {1075, tibia::Colors::MiniMap::grass},
        {1076, tibia::Colors::MiniMap::grass},
        {1077, tibia::Colors::MiniMap::grass},
        {1078, tibia::Colors::MiniMap::grass},
        {1079, tibia::Colors::MiniMap::grass},
        {1080, tibia::Colors::MiniMap::grass},
        {1081, tibia::Colors::MiniMap::grass},
        {1082, tibia::Colors::MiniMap::grass},
        {1083, tibia::Colors::MiniMap::grass},
        {1084, tibia::Colors::MiniMap::grass},
        {1085, tibia::Colors::MiniMap::grass},
        {1086, tibia::Colors::MiniMap::grass},
        {1092, tibia::Colors::MiniMap::cave},
        {1093, tibia::Colors::MiniMap::cave},
        {1094, tibia::Colors::MiniMap::cave},
        {1095, tibia::Colors::MiniMap::cave},
        {1096, tibia::Colors::MiniMap::cave},
        {1097, tibia::Colors::MiniMap::cave},
        {1098, tibia::Colors::MiniMap::cave},
        {1099, tibia::Colors::MiniMap::cave},
        {1100, tibia::Colors::MiniMap::cave},
        {1101, tibia::Colors::MiniMap::cave},
        {1102, tibia::Colors::MiniMap::cave},
        {1103, tibia::Colors::MiniMap::cave},
        {1140, tibia::Colors::MiniMap::tree},
        {1144, tibia::Colors::MiniMap::tree},
        {1152, tibia::Colors::MiniMap::wood},
        {1160, tibia::Colors::MiniMap::wood},
        {1164, tibia::Colors::MiniMap::tree},
        {1168, tibia::Colors::MiniMap::tree},
        {1172, tibia::Colors::MiniMap::tree},
        {1176, tibia::Colors::MiniMap::tree},
        {1180, tibia::Colors::MiniMap::tree},
        {1185, tibia::Colors::MiniMap::tree},
        {1186, tibia::Colors::MiniMap::wood},
        {1191, tibia::Colors::black},
        {1203, tibia::Colors::MiniMap::tree},
        {1495, tibia::Colors::MiniMap::cave},
        {1496, tibia::Colors::MiniMap::cave},
        {1564, tibia::Colors::MiniMap::swamp},
        {1565, tibia::Colors::MiniMap::cave},
        {1566, tibia::Colors::MiniMap::cave},
        {1567, tibia::Colors::MiniMap::cave},
        {1606, tibia::Colors::MiniMap::snow},
        {1610, tibia::Colors::MiniMap::tree},
        {1614, tibia::Colors::MiniMap::tree},
        {1618, tibia::Colors::MiniMap::tree},
        {1622, tibia::Colors::MiniMap::tree},
        {1623, tibia::Colors::MiniMap::wood},
        {1624, tibia::Colors::MiniMap::wood},
        {1625, tibia::Colors::MiniMap::wood},
        {1626, tibia::Colors::MiniMap::wood},
        {1630, tibia::Colors::MiniMap::tree},
        {1634, tibia::Colors::MiniMap::tree},
        {1635, tibia::Colors::MiniMap::tree},
        {1636, tibia::Colors::MiniMap::wood},
        {1637, tibia::Colors::MiniMap::tree},
        {1638, tibia::Colors::MiniMap::tree},
        {1651, tibia::Colors::MiniMap::tree},
        {1669, tibia::Colors::MiniMap::tree},
        {1676, tibia::Colors::MiniMap::tree},
        {1680, tibia::Colors::MiniMap::wood},
        {1684, tibia::Colors::MiniMap::wood},
        {1688, tibia::Colors::MiniMap::wood},
        {1692, tibia::Colors::MiniMap::wood},
        {1760, tibia::Colors::MiniMap::wood},
        {1764, tibia::Colors::MiniMap::wood},
        {1768, tibia::Colors::MiniMap::tree},
        {1772, tibia::Colors::MiniMap::tree},
        {1993, tibia::Colors::MiniMap::tree},
        {1994, tibia::Colors::MiniMap::tree},
        {1995, tibia::Colors::MiniMap::tree},
        {1996, tibia::Colors::MiniMap::tree},
        {1997, tibia::Colors::MiniMap::tree},
        {1998, tibia::Colors::MiniMap::tree},
        {1999, tibia::Colors::MiniMap::tree},
        {2000, tibia::Colors::MiniMap::tree},
        {2001, tibia::Colors::MiniMap::tree},
        {2002, tibia::Colors::MiniMap::tree},
        {2003, tibia::Colors::MiniMap::tree},
        {2004, tibia::Colors::MiniMap::tree},
        {2005, tibia::Colors::MiniMap::tree},
        {2006, tibia::Colors::MiniMap::tree},
        {2007, tibia::Colors::MiniMap::tree},
        {2008, tibia::Colors::MiniMap::tree},
        {2009, tibia::Colors::MiniMap::tree},
        {2010, tibia::Colors::MiniMap::tree},
        {2011, tibia::Colors::MiniMap::tree},
        {2012, tibia::Colors::MiniMap::tree},
        {2013, tibia::Colors::MiniMap::tree},
        {2014, tibia::Colors::MiniMap::tree},
        {2015, tibia::Colors::MiniMap::tree},
        {2016, tibia::Colors::MiniMap::tree},
        {2017, tibia::Colors::MiniMap::tree},
        {2018, tibia::Colors::MiniMap::tree},
        {2019, tibia::Colors::MiniMap::tree},
        {2020, tibia::Colors::MiniMap::tree},
        {2021, tibia::Colors::MiniMap::sand},
        {2022, tibia::Colors::MiniMap::sand},
        {2023, tibia::Colors::MiniMap::sand},
        {2024, tibia::Colors::MiniMap::sand},
        {2025, tibia::Colors::MiniMap::sand},
        {2026, tibia::Colors::MiniMap::sand},
        {2027, tibia::Colors::MiniMap::sand},
        {2028, tibia::Colors::MiniMap::sand},
        {2029, tibia::Colors::MiniMap::sand},
        {2030, tibia::Colors::MiniMap::sand},
        {2031, tibia::Colors::MiniMap::sand},
        {2032, tibia::Colors::MiniMap::sand},
        {2033, tibia::Colors::MiniMap::sand},
        {2034, tibia::Colors::MiniMap::shallowWater},
        {2035, tibia::Colors::MiniMap::shallowWater},
        {2036, tibia::Colors::MiniMap::shallowWater},
        {2037, tibia::Colors::MiniMap::shallowWater},
        {2038, tibia::Colors::MiniMap::shallowWater},
        {2039, tibia::Colors::MiniMap::shallowWater},
        {2040, tibia::Colors::MiniMap::shallowWater},
        {2041, tibia::Colors::MiniMap::shallowWater},
        {2042, tibia::Colors::MiniMap::shallowWater},
        {2043, tibia::Colors::MiniMap::shallowWater},
        {2044, tibia::Colors::MiniMap::shallowWater},
        {2045, tibia::Colors::MiniMap::shallowWater},
        {2046, tibia::Colors::MiniMap::shallowWater},
        {2123, tibia::Colors::MiniMap::tree},
        {2127, tibia::Colors::MiniMap::tree},
        {2131, tibia::Colors::MiniMap::tree},
        {2164, tibia::Colors::MiniMap::tree},
        {2168, tibia::Colors::MiniMap::tree},
        {2172, tibia::Colors::MiniMap::tree},
        {2185, tibia::Colors::MiniMap::sand},
        {2186, tibia::Colors::MiniMap::sand},
        {2187, tibia::Colors::MiniMap::sand},
        {2188, tibia::Colors::MiniMap::sand},
        {2191, tibia::Colors::MiniMap::sand},
        {2192, tibia::Colors::MiniMap::sand},
        {2193, tibia::Colors::MiniMap::sand},
        {2194, tibia::Colors::MiniMap::sand},
        {3254, tibia::Colors::MiniMap::mountain},
        {3255, tibia::Colors::MiniMap::mountain},
        {3256, tibia::Colors::MiniMap::mountain},
        {3257, tibia::Colors::MiniMap::mountain},
        {3322, tibia::Colors::MiniMap::tree},
        {3326, tibia::Colors::MiniMap::tree},
        {3337, tibia::Colors::MiniMap::snow},
        {3344, tibia::Colors::MiniMap::wood},
        {3375, tibia::Colors::MiniMap::tree},
    };

    namespace GuiData
    {
        namespace GameWindow
        {
            int x = 51;
            int y = 23;

            int width  = tibia::TILES_WIDTH;
            int height = tibia::TILES_HEIGHT;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);
        }

        namespace OptionsButton
        {
            int x = 13;
            int y = 19;

            int width  = 24;
            int height = 24;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);
        }

        namespace ChatLogWindow
        {
            int x = 11;
            int y = 363;

            int width  = 496;
            int height = 70;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            int numLinesVisible = 4;

            int maxLines = 32; // every line is drawn

            int textHeight = 16;

            int textOffset = 2;

            namespace Buttons
            {
                namespace ScrollDown
                {
                    int x = 471;
                    int y = 401;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace ScrollUp
                {
                    int x = 471;
                    int y = 363;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }
            }
        }

        namespace TabButtons
        {
            namespace State
            {
                enum
                {
                    inventory,
                    status,
                    combat,
                    miniMap,
                };
            }

            namespace Inventory
            {
                int x = 518;
                int y = 132;

                int width  = 24;
                int height = 24;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace Status
            {
                int x = 547;
                int y = 132;

                int width  = 24;
                int height = 24;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace Combat
            {
                int x = 576;
                int y = 132;

                int width  = 24;
                int height = 24;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace MiniMap
            {
                int x = 605;
                int y = 132;

                int width  = 24;
                int height = 24;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }
        }

        namespace MiniMapWindow
        {
            int x = 519;
            int y = 11;

            int width  = 110;
            int height = 110;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            int zoomMin     = 1;
            int zoomDefault = 2;
            int zoomMax     = 4;
        }

        namespace StatusWindow
        {
            int x = 519;
            int y = 11;

            int width  = 110;
            int height = 110;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            namespace Icons
            {
                int width  = 32;
                int height = 32;
            }

            namespace Stats
            {
                namespace Hp
                {
                    int x = 541;
                    int y = 14;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace Cap
                {
                    int x = 576;
                    int y = 14;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace Exp
                {
                    int x = 522;
                    int y = 49;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace Mp
                {
                    int x = 594;
                    int y = 49;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace Level
                {
                    int x = 540;
                    int y = 84;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace magicLevel
                {
                    int x = 576;
                    int y = 84;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }
            }
        }

        namespace EquipmentWindow
        {
            int x = 519;
            int y = 11;

            int width  = 110;
            int height = 110;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            namespace Slots
            {
                namespace Container
                {
                    int x = 601;
                    int y = 15;

                    int width  = 24;
                    int height = 24;

                    float scale = 0.75f; // (24 * 100) / 32

                    sf::Vector2f position(x, y);
                }
            }
        }

        namespace InventoryWindow
        {
            int x = 515;
            int y = 163;

            int width  = 118;
            int height = 310;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            namespace Slots
            {
                int width  = 32;
                int height = 32;

                namespace Window
                {
                    int x = 523;
                    int y = 206;

                    int width  = 102;
                    int height = 263;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                int size = 35; // 32 + 3

                int distanceBetweenSlots = 3; // white space between slots

                int numSlotsHorizontal = 3;

                int numSlotsVisible  = 21; // last 3 are half visible
                int numSlotsVisible2 = 9;  // for split-inventory window
            }

            namespace Buttons
            {
                namespace ScrollDown
                {
                    int x = 523;
                    int y = 171;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace ScrollUp
                {
                    int x = 593;
                    int y = 171;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace ScrollDown2
                {
                    int x = 523;
                    int y = 329;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace ScrollUp2
                {
                    int x = 593;
                    int y = 329;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }
            }

            namespace Icons
            {
                namespace Container
                {
                    int x = 558;
                    int y = 171;

                    sf::Vector2f position(x, y);
                }

                namespace Container2
                {
                    int x = 558;
                    int y = 329;

                    sf::Vector2f position(x, y);
                }
            }
        }

        namespace SkillsWindow
        {
            int x = 515;
            int y = 163;

            int width  = 118;
            int height = 231;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            namespace Text
            {
                int startX = 626;
                int startY = 169;

                int distanceBetweenText = 12;

                int alignTextRightOffset = 2;
            }
        }

        namespace OutfitButtons
        {
            int width  = 32;
            int height = 32;

            int borderSize = 3;

            namespace Head
            {
                int x = 521;
                int y = 401;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace Body
            {
                int x = 558;
                int y = 401;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace Legs
            {
                int x = 521;
                int y = 438;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace Feet
            {
                int x = 558;
                int y = 438;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace All
            {
                int x = 595;
                int y = 419;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }
        }

        namespace CombatWindow
        {
            int x = 515;
            int y = 163;

            int width  = 118;
            int height = 231;

            sf::Vector2f position(x, y);

            sf::IntRect rect(x, y, width, height);

            namespace Creatures
            {
                int width  = 32;
                int height = 32;

                namespace Window
                {
                    int x = 519;
                    int y = 207;

                    int width  = 110;
                    int height = 183;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);

                }

                float timeSecondsToShowDeadCreatures = 5.0f;

                int numCreaturesVisible = 5;

                int numCreaturePixelsHalfVisible = 8;

                int creatureIconLeftOffset = 2;

                int creatureTextLeftOffsetFromIcon = 4;

                int distanceBetweenText = 3;

                int distanceBetweenCreatures = 3;
            }

            namespace Buttons
            {
                namespace ScrollDown
                {
                    int x = 532;
                    int y = 171;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }

                namespace ScrollUp
                {
                    int x = 584;
                    int y = 171;

                    int width  = 32;
                    int height = 32;

                    sf::Vector2f position(x, y);

                    sf::IntRect rect(x, y, width, height);
                }
            }
        }

        namespace CombatButtons
        {
            namespace State
            {
                enum
                {
                    offense,
                    normal,
                    defense,
                };
            }

            namespace FullOffense
            {
                int x = 521;
                int y = 401;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace HalfOffenseDefense
            {
                int x = 558;
                int y = 401;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }

            namespace FullDefense
            {
                int x = 595;
                int y = 401;

                int width  = 32;
                int height = 32;

                sf::Vector2f position(x, y);

                sf::IntRect rect(x, y, width, height);
            }
        }

        namespace StatusEffectIcons
        {
            int x = 481;
            int y = 19;

            sf::Vector2f position(x, y);

            int distanceBetweenIcons = 28;
        }

        namespace StatusBarText
        {
            int x = 259;
            int y = 298;

            sf::Vector2f position(x, y);
        }

        namespace Bars
        {
            namespace Hp
            {
                int x = 51;
                int y = 323;

                sf::Vector2f position(x, y);
            }

            namespace Mp
            {
                int x = 265;
                int y = 323;

                sf::Vector2f position(x, y);
            }
        }

        namespace CreatureBars
        {
            int width  = 24;
            int height = 2;
        }
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
        const float default = 0.5f;
        const float player  = 0.1f; // 0.2f
    }

    namespace Teams
    {
        enum
        {
            neutral,
            good,
            evil,
            other,
        };
    }

    std::unordered_map<std::string, int> umapTeams
    {
        {"neutral", tibia::Teams::neutral},
        {"good",    tibia::Teams::good},
        {"evil",    tibia::Teams::evil},
        {"other",   tibia::Teams::other},
    };

    namespace Vocations
    {
        enum
        {
            none,
            knight,
            paladin,
            sorcerer,
            druid,
        };
    }

    std::unordered_map<int, std::string> umapVocations =
    {
        {tibia::Vocations::none,     "None"},
        {tibia::Vocations::knight,   "Knight"},
        {tibia::Vocations::paladin,  "Paladin"},
        {tibia::Vocations::sorcerer, "Sorcerer"},
        {tibia::Vocations::druid,    "Druid"},
    };

    namespace HealthStates
    {
        enum
        {
            healthy,        // 91 -> 100%
            barelyWounded,  // 76 -> 90%
            lightlyWounded, // 51 -> 75%
            heavilyWounded, // 26 -> 50%
            critical,       // 11 -> 25%
            nearlyDead,     // 1  -> 10%
            dead,           //       0%

            unknown,
        };
    }

    std::unordered_map<int, std::string> umapHealthStates =
    {
        {tibia::HealthStates::healthy,        "healthy"},
        {tibia::HealthStates::barelyWounded,  "barely wounded"},
        {tibia::HealthStates::lightlyWounded, "lightly wounded"},
        {tibia::HealthStates::heavilyWounded, "heavily wounded"},
        {tibia::HealthStates::critical,       "critical"},
        {tibia::HealthStates::nearlyDead,     "nearly dead"},
        {tibia::HealthStates::dead,           "dead"},

        {tibia::HealthStates::unknown,        "unknown"},
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
            playMusic,
            playSound,
        };
    }

    std::unordered_map<std::string, int> umapObjectTypes
    {
        {"null",        tibia::ObjectTypes::null},
        {"sign",        tibia::ObjectTypes::sign},
        {"teleporter",  tibia::ObjectTypes::teleporter},
        {"door",        tibia::ObjectTypes::door},
        {"bed",         tibia::ObjectTypes::bed},
        {"lever",       tibia::ObjectTypes::lever},
        {"changemap",   tibia::ObjectTypes::changeMap},
        {"creature",    tibia::ObjectTypes::creature},
        {"book",        tibia::ObjectTypes::book},
        {"playmusic",   tibia::ObjectTypes::playMusic},
        {"playsound",   tibia::ObjectTypes::playSound},
    };

    namespace Outfits
    {
        // newbie outfit
        std::vector<int> default = {1, 1, 0, 0}; // head, body, legs, feet

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

    namespace OutfitPieces
    {
        enum
        {
            head,
            body,
            legs,
            feet,
        };
    }

    namespace ModifyHpTypes
    {
        enum
        {
            none,
            black,
            blood,
            fire,
            electricity,
            poison,
            poisoned,
            spellBlue,
            spellBlack,
            heal,
        };
    }

    std::unordered_map<int, sf::Color> umapModifyHpTextColors =
    {
        {tibia::ModifyHpTypes::fire,        tibia::Colors::Text::orange},
        {tibia::ModifyHpTypes::electricity, tibia::Colors::Text::teal},
        {tibia::ModifyHpTypes::poison,      tibia::Colors::Text::green},
        {tibia::ModifyHpTypes::poisoned,    tibia::Colors::Text::green},
    };

    std::unordered_map<int, int> umapModifyHpOnTouchTypes =
    {
        {1489, tibia::ModifyHpTypes::fire},
        {1490, tibia::ModifyHpTypes::fire},
        {1491, tibia::ModifyHpTypes::fire},
        {1492, tibia::ModifyHpTypes::fire},
        //{1493, tibia::ModifyHpTypes::fire},
        //{1494, tibia::ModifyHpTypes::fire},

        {1497, tibia::ModifyHpTypes::electricity},
        {1498, tibia::ModifyHpTypes::electricity},

        {3158, tibia::ModifyHpTypes::poisoned},
        {3159, tibia::ModifyHpTypes::poisoned},
        {3160, tibia::ModifyHpTypes::poisoned},
        {3161, tibia::ModifyHpTypes::poisoned},
    };

    std::unordered_map<int, int> umapModifyHpOnTouchDamages =
    {
        {1489, 25},
        {1490, 25},
        {1491, 10},
        {1492, 10},
        //{1493, 1},
        //{1494, 1},

        {1497, 25},
        {1498, 25},

        {3158, 0},
        {3159, 0},
        {3160, 0},
        {3161, 0},
    };

    namespace CreatureStatusEffects
    {
        enum
        {
            burning,
            cursed,
            dazzled,
            drowning,
            drunk,
            electrified,
            freezing,
            hasted,
            magicShielded,
            poisoned,
            slowed,
            strengthened,
        };
    }

    std::unordered_map<int, int> umapModifyHpOnTouchStatusEffects =
    {
        {1489, tibia::CreatureStatusEffects::burning},
        {1490, tibia::CreatureStatusEffects::burning},
        {1491, tibia::CreatureStatusEffects::burning},
        {1492, tibia::CreatureStatusEffects::burning},
        //{1493, tibia::CreatureStatusEffects::burning},
        //{1494, tibia::CreatureStatusEffects::burning},

        {1497, tibia::CreatureStatusEffects::electrified},
        {1498, tibia::CreatureStatusEffects::electrified},

        {3158, tibia::CreatureStatusEffects::poisoned},
        {3159, tibia::CreatureStatusEffects::poisoned},
        {3160, tibia::CreatureStatusEffects::poisoned},
        {3161, tibia::CreatureStatusEffects::poisoned},
    };

    std::unordered_map<int, std::string> umapCreatureStatusEffectsNames =
    {
        {tibia::CreatureStatusEffects::burning,       "Burning"},
        {tibia::CreatureStatusEffects::cursed,        "Cursed"},
        {tibia::CreatureStatusEffects::dazzled,       "Dazzled"},
        {tibia::CreatureStatusEffects::drowning,      "Drowning"},
        {tibia::CreatureStatusEffects::drunk,         "Drunk"},
        {tibia::CreatureStatusEffects::electrified,   "Electrified"},
        {tibia::CreatureStatusEffects::freezing,      "Freezing"},
        {tibia::CreatureStatusEffects::hasted,        "Hasted"},
        {tibia::CreatureStatusEffects::magicShielded, "Magic Shielded"},
        {tibia::CreatureStatusEffects::poisoned,      "Poisoned"},
        {tibia::CreatureStatusEffects::slowed,        "Slowed"},
        {tibia::CreatureStatusEffects::strengthened,  "Strengthened"},
    };

    std::unordered_map<int, int> umapCreatureStatusEffectsModifyHpTypes =
    {
        {tibia::CreatureStatusEffects::burning,       tibia::ModifyHpTypes::fire},
        {tibia::CreatureStatusEffects::cursed,        tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::dazzled,       tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::drowning,      tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::drunk,         tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::electrified,   tibia::ModifyHpTypes::electricity},
        {tibia::CreatureStatusEffects::freezing,      tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::hasted,        tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::magicShielded, tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::poisoned,      tibia::ModifyHpTypes::poisoned},
        {tibia::CreatureStatusEffects::slowed,        tibia::ModifyHpTypes::none},
        {tibia::CreatureStatusEffects::strengthened,  tibia::ModifyHpTypes::none},
    };

    std::unordered_map<int, int> umapCreatureStatusEffectsDamages =
    {
        {tibia::CreatureStatusEffects::burning,       10},
        {tibia::CreatureStatusEffects::cursed,        0},
        {tibia::CreatureStatusEffects::dazzled,       0},
        {tibia::CreatureStatusEffects::drowning,      10},
        {tibia::CreatureStatusEffects::drunk,         0},
        {tibia::CreatureStatusEffects::electrified,   10},
        {tibia::CreatureStatusEffects::freezing,      10},
        {tibia::CreatureStatusEffects::hasted,        0},
        {tibia::CreatureStatusEffects::magicShielded, 0},
        {tibia::CreatureStatusEffects::poisoned,      2},
        {tibia::CreatureStatusEffects::slowed,        0},
        {tibia::CreatureStatusEffects::strengthened,  0},
    };

    std::unordered_map<int, int> umapCreatureStatusEffectsTicks =
    {
        {tibia::CreatureStatusEffects::burning,       10},
        {tibia::CreatureStatusEffects::cursed,        10},
        {tibia::CreatureStatusEffects::dazzled,       10},
        {tibia::CreatureStatusEffects::drowning,      10},
        {tibia::CreatureStatusEffects::drunk,         10},
        {tibia::CreatureStatusEffects::electrified,   10},
        {tibia::CreatureStatusEffects::freezing,      10},
        {tibia::CreatureStatusEffects::hasted,        10},
        {tibia::CreatureStatusEffects::magicShielded, 10},
        {tibia::CreatureStatusEffects::poisoned,      10},
        {tibia::CreatureStatusEffects::slowed,        10},
        {tibia::CreatureStatusEffects::strengthened,  10},
    };

    std::unordered_map<int, sf::Time> umapCreatureStatusEffectsTimesPerTick =
    {
        {tibia::CreatureStatusEffects::burning,       sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::cursed,        sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::dazzled,       sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::drowning,      sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::drunk,         sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::electrified,   sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::freezing,      sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::hasted,        sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::magicShielded, sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::poisoned,      sf::seconds(5.0f)},
        {tibia::CreatureStatusEffects::slowed,        sf::seconds(1.0f)},
        {tibia::CreatureStatusEffects::strengthened,  sf::seconds(1.0f)},
    };

    namespace CreatureTypes
    {
        enum
        {
            human,

            bear,
            behemoth,
            beholder,
            cacodemon, // Doom
            cyclops,
            citizenMale,
            citizenFemale,
            demon,
            demonSkeleton,
            gameMaster,
            giantSpider,
            ghoul,
            dragon,
            dragonLord,
            hero,
            lion,
            monk,
            necromancer,
            orc,
            poisonSpider,
            polarBear,
            santaClaus,
            skeleton,
            spider,
            stoneGolem,
            witch,
        };
    }

    std::unordered_map<std::string, int> umapCreatureTypes =
    {
        {"human", tibia::CreatureTypes::human},

        {"bear", tibia::CreatureTypes::bear},
        {"behemoth", tibia::CreatureTypes::behemoth},
        {"beholder", tibia::CreatureTypes::beholder},
        {"cacodemon", tibia::CreatureTypes::cacodemon},
        {"cyclops", tibia::CreatureTypes::cyclops},
        {"citizen_male", tibia::CreatureTypes::citizenMale},
        {"citizen_female", tibia::CreatureTypes::citizenFemale},
        {"demon", tibia::CreatureTypes::demon},
        {"demon_skeleton", tibia::CreatureTypes::demonSkeleton},
        {"gamemaster", tibia::CreatureTypes::gameMaster},
        {"giant_spider", tibia::CreatureTypes::giantSpider},
        {"ghoul", tibia::CreatureTypes::ghoul},
        {"dragon", tibia::CreatureTypes::dragon},
        {"dragon_lord", tibia::CreatureTypes::dragonLord},
        {"hero", tibia::CreatureTypes::hero},
        {"lion", tibia::CreatureTypes::lion},
        {"monk", tibia::CreatureTypes::monk},
        {"necromancer", tibia::CreatureTypes::necromancer},
        {"orc", tibia::CreatureTypes::orc},
        {"poison_spider", tibia::CreatureTypes::poisonSpider},
        {"polar_bear", tibia::CreatureTypes::polarBear},
        {"santa_claus", tibia::CreatureTypes::santaClaus},
        {"skeleton", tibia::CreatureTypes::skeleton},
        {"spider", tibia::CreatureTypes::spider},
        {"stone_golem", tibia::CreatureTypes::stoneGolem},
        {"witch", tibia::CreatureTypes::witch},
    };

    std::unordered_map<int, std::string> umapCreatureNames =
    {
        {tibia::CreatureTypes::human, "human"},

        {tibia::CreatureTypes::bear, "bear"},
        {tibia::CreatureTypes::behemoth, "behemoth"},
        {tibia::CreatureTypes::beholder, "beholder"},
        {tibia::CreatureTypes::cacodemon, "cacodemon"},
        {tibia::CreatureTypes::cyclops, "cyclops"},
        {tibia::CreatureTypes::citizenMale, "citizen"},
        {tibia::CreatureTypes::citizenFemale, "citizen"},
        {tibia::CreatureTypes::demon, "demon"},
        {tibia::CreatureTypes::demonSkeleton, "demon skeleton"},
        {tibia::CreatureTypes::gameMaster, "gamemaster"},
        {tibia::CreatureTypes::giantSpider, "giant spider"},
        {tibia::CreatureTypes::ghoul, "ghoul"},
        {tibia::CreatureTypes::dragon, "dragon"},
        {tibia::CreatureTypes::dragonLord, "dragon lord"},
        {tibia::CreatureTypes::hero, "hero"},
        {tibia::CreatureTypes::lion, "lion"},
        {tibia::CreatureTypes::monk, "monk"},
        {tibia::CreatureTypes::necromancer, "necromancer"},
        {tibia::CreatureTypes::orc, "orc"},
        {tibia::CreatureTypes::poisonSpider, "poison spider"},
        {tibia::CreatureTypes::polarBear, "polar bear"},
        {tibia::CreatureTypes::santaClaus, "Santa Claus"},
        {tibia::CreatureTypes::skeleton, "skeleton"},
        {tibia::CreatureTypes::spider, "spider"},
        {tibia::CreatureTypes::stoneGolem, "stone golem"},
        {tibia::CreatureTypes::witch, "witch"},
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
        {tibia::CreatureTypes::human, tibia::CreatureSizes::small},
        {tibia::CreatureTypes::bear, tibia::CreatureSizes::medium},
        {tibia::CreatureTypes::cacodemon, tibia::CreatureSizes::large},
        {tibia::CreatureTypes::citizenMale, tibia::CreatureSizes::small},
        {tibia::CreatureTypes::citizenFemale, tibia::CreatureSizes::small},
        {tibia::CreatureTypes::demon, tibia::CreatureSizes::large},
        {tibia::CreatureTypes::poisonSpider, tibia::CreatureSizes::small},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureSizes::medium},
    };

    namespace CreatureSprites
    {
        // {up, right, down, left}

        std::vector<int> human = {3390, 3390, 3390, 3390};

        std::vector<int> bear =
        {
            // animation 1
            1802, 1794, 1806, 1798,
            1801, 1793, 1805, 1797,

            // animation 2
            1804, 1796, 1808, 1800,
            1803, 1795, 1807, 1799,
        };

        std::vector<int> cacodemon =
        {
            // animation 1
            3396, 3416, 3436, 3456,
            3395, 3415, 3435, 3455,
            3394, 3414, 3434, 3454,
            3393, 3413, 3433, 3453,

            // animation 2
            3400, 3420, 3440, 3460,
            3399, 3419, 3439, 3459,
            3398, 3418, 3438, 3458,
            3397, 3417, 3437, 3457,

            // animation 3
            3404, 3424, 3444, 3464,
            3403, 3423, 3443, 3463,
            3402, 3422, 3442, 3462,
            3401, 3421, 3441, 3461,

            // animation 4
            3408, 3428, 3448, 3468,
            3407, 3427, 3447, 3467,
            3406, 3426, 3446, 3466,
            3405, 3425, 3445, 3465,

            // animation 5
            3412, 3432, 3452, 3472,
            3411, 3431, 3451, 3471,
            3410, 3430, 3450, 3470,
            3409, 3429, 3449, 3469,
        };

        std::vector<int> citizenMale   = {3350, 3351, 3349, 3352};
        std::vector<int> citizenFemale = {3354, 3355, 3353, 3356};

        std::vector<int> demon =
        {
            1480, 1484, 1476, 1488,
            1479, 1483, 1475, 1487,
            1478, 1482, 1474, 1486,
            1477, 1481, 1473, 1485,
        };

        std::vector<int> hero = {1645, 1646, 1643, 1644};

        std::vector<int> gameMaster = {3330, 3329, 3327, 3328};

        std::vector<int> ghoul = {1838, 1841, 1839, 1840};

        std::vector<int> orc = {2863, 2862, 2864, 2861};

        std::vector<int> poisonSpider =
        {
            //animation 1
            1514, 1518, 1522, 302,

            //animation 2
            1515, 1519, 1523, 303,

            //animation 3
            1516, 1520, 1524, 304,

            //animation 4
            1517, 1521, 1525, 305,
        };

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
    }

    std::unordered_map<int, std::vector<int>> umapCreatureSprites =
    {
        {tibia::CreatureTypes::human, tibia::CreatureSprites::human},

        {tibia::CreatureTypes::bear, tibia::CreatureSprites::bear},
        {tibia::CreatureTypes::cacodemon, tibia::CreatureSprites::cacodemon},
        {tibia::CreatureTypes::citizenMale, tibia::CreatureSprites::citizenMale},
        {tibia::CreatureTypes::citizenFemale, tibia::CreatureSprites::citizenFemale},
        {tibia::CreatureTypes::demon, tibia::CreatureSprites::demon},
        {tibia::CreatureTypes::poisonSpider, tibia::CreatureSprites::poisonSpider},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureSprites::santaClaus},
    };

    std::unordered_map<int, int> umapCreatureNumAnimations =
    {
        {tibia::CreatureTypes::human, 1},

        {tibia::CreatureTypes::bear, 2},
        {tibia::CreatureTypes::cacodemon, 5},
        {tibia::CreatureTypes::citizenMale, 1},
        {tibia::CreatureTypes::citizenFemale, 1},
        {tibia::CreatureTypes::demon, 1},
        {tibia::CreatureTypes::poisonSpider, 4},
        {tibia::CreatureTypes::santaClaus, 1},
    };

    namespace CreatureCorpseSizes
    {
        enum
        {
            small,
            medium,
            large,
        };
    }

    std::unordered_map<int, int> umapCreatureCorpseSizes =
    {
        {tibia::CreatureTypes::human, tibia::CreatureCorpseSizes::small},

        {tibia::CreatureTypes::bear, tibia::CreatureCorpseSizes::medium},
        {tibia::CreatureTypes::cacodemon, tibia::CreatureCorpseSizes::large},
        {tibia::CreatureTypes::citizenMale, tibia::CreatureCorpseSizes::small},
        {tibia::CreatureTypes::citizenFemale, tibia::CreatureCorpseSizes::small},
        {tibia::CreatureTypes::demon, tibia::CreatureCorpseSizes::large},
        {tibia::CreatureTypes::poisonSpider, tibia::CreatureCorpseSizes::small},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureCorpseSizes::small},
    };

    namespace CreatureCorpseSprites
    {
        std::vector<int> human = {491, 492, 493, 494, 495, 496, 497};

        std::vector<int> bear =
        {
            1810, 1809,
            1812, 1811,
            1814, 1813,
            1816, 1815,
        };

        std::vector<int> cacodemon =
        {
            3476, 3475, 3474, 3473,
            3480, 3479, 3478, 3477,
            3484, 3483, 3482, 3481,
            3488, 3487, 3486, 3485,
            3492, 3491, 3490, 3489,
            3496, 3495, 3494, 3493,
        };

        std::vector<int> citizenMale   = {3357, 3358, 3359};
        std::vector<int> citizenFemale = {3360, 3361, 3359};

        std::vector<int> demon =
        {
            319, 318, 317, 316,
            323, 322, 321, 320,
            327, 326, 325, 324,
            331, 330, 329, 328,
        };

        std::vector<int> poisonSpider = {3068, 3069, 3070};
    }

    std::unordered_map<int, std::vector<int>> umapCreatureCorpseSprites =
    {
        {tibia::CreatureTypes::human, tibia::CreatureCorpseSprites::human},
        
        {tibia::CreatureTypes::bear, tibia::CreatureCorpseSprites::bear},
        {tibia::CreatureTypes::cacodemon, tibia::CreatureCorpseSprites::cacodemon},
        {tibia::CreatureTypes::citizenMale, tibia::CreatureCorpseSprites::citizenMale},
        {tibia::CreatureTypes::citizenFemale, tibia::CreatureCorpseSprites::citizenFemale},
        {tibia::CreatureTypes::demon, tibia::CreatureCorpseSprites::demon},
        {tibia::CreatureTypes::poisonSpider, tibia::CreatureCorpseSprites::poisonSpider},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureCorpseSprites::human},
    };

    namespace CreatureBloodTypes
    {
        enum
        {
            none,
            red,
            green,
        };
    }

    std::unordered_map<int, int> umapCreatureBloodTypes =
    {
        {tibia::CreatureTypes::human, tibia::CreatureBloodTypes::red},

        {tibia::CreatureTypes::bear, tibia::CreatureBloodTypes::red},
        {tibia::CreatureTypes::cacodemon, tibia::CreatureBloodTypes::red},
        {tibia::CreatureTypes::citizenMale, tibia::CreatureBloodTypes::red},
        {tibia::CreatureTypes::citizenFemale, tibia::CreatureBloodTypes::red},
        {tibia::CreatureTypes::demon, tibia::CreatureBloodTypes::red},
        {tibia::CreatureTypes::poisonSpider, tibia::CreatureBloodTypes::green},
        {tibia::CreatureTypes::santaClaus, tibia::CreatureBloodTypes::red},
    };

    namespace AnimationTimes
    {
        const float default           = 0.1f;
        const float corpseDecay       = 60.0f;
        const float creatureAnimation = 1.0f;
    }

    namespace Animations
    {
        // {id, numFrames}

        std::vector<int> corpse = {491, 7};

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

        std::vector<int> sparkle = {3376, 4};
    }

    std::unordered_map<int, std::vector<int>> umapModifyHpOnTouchAnimations =
    {
        {1489, tibia::Animations::fire},
        {1490, tibia::Animations::fire},
        {1491, tibia::Animations::fire},
        {1492, tibia::Animations::fire},
        //{1493, tibia::Animations::fire},
        //{1494, tibia::Animations::fire},

        {1497, tibia::Animations::electricity},
        {1498, tibia::Animations::electricity},

        {3158, tibia::Animations::poison},
        {3159, tibia::Animations::poison},
        {3160, tibia::Animations::poison},
        {3161, tibia::Animations::poison},
    };

    std::unordered_map<int, std::vector<int>> umapModifyHpAnimations =
    {
        {tibia::ModifyHpTypes::black,       tibia::Animations::hitBlack},
        {tibia::ModifyHpTypes::blood,       tibia::Animations::hitBlood},
        {tibia::ModifyHpTypes::fire,        tibia::Animations::fire},
        {tibia::ModifyHpTypes::electricity, tibia::Animations::electricity},
        {tibia::ModifyHpTypes::poison,      tibia::Animations::hitPoison},
        {tibia::ModifyHpTypes::poisoned,    tibia::Animations::poison},
        {tibia::ModifyHpTypes::spellBlue,   tibia::Animations::spellBlue},
        {tibia::ModifyHpTypes::spellBlack,  tibia::Animations::spellBlack},
        {tibia::ModifyHpTypes::heal,        tibia::Animations::particlesRed},
    };

    std::unordered_map<int, std::vector<int>> umapCreatureBloodTypeAnimations =
    {
        {tibia::CreatureBloodTypes::none,  tibia::Animations::hitBlack},
        {tibia::CreatureBloodTypes::red,   tibia::Animations::hitBlood},
        {tibia::CreatureBloodTypes::green, tibia::Animations::hitPoison},
    };

    namespace Projectiles
    {
        int spellBlue  = 1829;
        int spellBlack = 3252;

        std::vector<int> spellFire = {1831, 1834, 1836, 1833, 1830, 1832, 1837, 1835};

        std::vector<int> spear = {963, 965, 967, 969, 970, 964, 966, 968};

        std::vector<int> bolt = {972, 974, 976, 978, 979, 973, 975, 977};

        std::vector<int> arrow       = {980, 982, 984, 986, 987, 981, 983, 985};
        std::vector<int> arrowFire   = {1855, 1857, 1859, 1861, 1854, 1856, 1858, 1860};
        std::vector<int> arrowPoison = {1847, 1849, 1851, 1853, 1846, 1848, 1850, 1852};

        int cacodemon = 3497;
    }

    namespace ProjectileTypes
    {
        enum
        {
            spellBlue,
            spellBlack,
            spellFire,
            spear,
            bolt,
            arrow,
            arrowFire,
            arrowPoison,
            cacodemon
        };
    }

    namespace ProjectileRanges
    {
        const float default = 6.0f;
    }

    namespace ProjectileSpeeds
    {
        const float default = 240.0f;
    }

    namespace ProjectileDamages
    {
        const float default = 10.0f;
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
            moveable         = 1 << 16,
            modifyHpOnTouch  = 1 << 17,
            stackable        = 1 << 18,
            pickupable       = 1 << 19,
            food             = 1 << 20,
            instrument       = 1 << 21,
            currency         = 1 << 22,
        };
    }

    // <id, flags>
    std::unordered_map<unsigned int, unsigned int> umapSpriteFlags;

    namespace SpriteData
    {
        std::vector<int> corpse = {491, 492, 493, 494, 495, 496, 497}; // decaying

        std::vector<int> chair = {522, 1054, 521, 1055}; // up, right, down, left

        std::vector<int> counterVertical   = {1043, 1046}; // closed, open
        std::vector<int> counterHorizontal = {1044, 1048};

        std::vector<int> streetLamp = {486, 3385}; // off, on

        std::vector<int> sign = {379, 380, 381, 382, 383, 384};

        std::vector<int> signTall = {3310, 3313};

        std::vector<int> chalkboard = {609, 2965};

        std::vector<int> doorVertical   = {549, 553}; // closed, open
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

        std::vector<int> digHole    = {1204, 1205}; // closed, open
        std::vector<int> digHoleIce = {3242, 3243};

        std::vector<int> stepTileStone     = {398,  399};  // deactivated, activated
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

        std::vector<int> poolRed   = {745, 746, 747};
        std::vector<int> poolGreen = {757, 758, 759};

        std::vector<int> splatRed =   {748, 749, 750};
        std::vector<int> splatGreen = {760, 761, 762};

        std::vector<int> locker = {1267, 1264, 1265, 1266}; // up, right, down, left

        std::vector<int> parcel = {1268, 1269};
        std::vector<int> letter = {1270, 1271};

        std::vector<int> magicWall = {3271, 3275, 3279};

        std::vector<int> portal = {197, 198, 199, 3391};

        const int bag      = 45;
        const int backpack = 200;

        const int label   = 1272;
        const int depot   = 1273;
        const int mailBox = 1274;

        const int bush          = 1060;
        const int bushBlueberry = 3375;

        const int runeBlank = 1277;

        const int spellBook = 511;

        const int watch = 498;

        const int broom      = 41;
        const int scythe     = 73;
        const int rake       = 332;
        const int pick       = 374;
        const int hoe        = 668;
        const int shovel     = 669;
        const int rope       = 995;
        const int fishingRod = 1470;
        const int machete    = 3238;

        const int keySilver = 1061;
        const int keyBlue   = 1062;
        const int keyGreen  = 1063;
        const int keyRed    = 1064;
        const int keyWhite  = 1065;
        const int keyGold   = 2184;

        const int keyRing = 1876;

        const int dustBin = 1472;

        const int presentBox = 2160;

        const int ladder = 455;

        const int stairs = 460;

        const int sewerGrate = 607;

        const int ropeUp = 3253;

        const int treeWall = 3344;

        const int mountainRampLeft  = 2103;
        const int mountainRampRight = 2097;

        namespace Gui
        {
            std::vector<int> scrollButtonDown  = {63, 61};
            std::vector<int> scrollButtonUp    = {64, 62};
            std::vector<int> scrollButtonLeft  = {3554, 3552};
            std::vector<int> scrollButtonRight = {3555, 3553};

            const int optionsButton = 3559;

            std::vector<int> tabButtonInventory = {3544, 3545};
            std::vector<int> tabButtonStatus    = {3546, 3547};
            std::vector<int> tabButtonCombat    = {3548, 3549};
            std::vector<int> tabButtonMiniMap   = {3550, 3551};

            const int outfitButtonHead = 3521;
            const int outfitButtonBody = 3522;
            const int outfitButtonLegs = 3523;
            const int outfitButtonFeet = 3524;
            const int outfitButtonAll  = 3525;

            std::vector<int> combatButtonFullOffense        = {3534, 3535};
            std::vector<int> combatButtonHalfOffenseDefense = {3536, 3537};
            std::vector<int> combatButtonFullDefense        = {3538, 3539};

            const int statusEffectBurning     = 3556;
            const int statusEffectElectrified = 3557;
            const int statusEffectPoisoned    = 3558;
        }

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
            394, 395, 396, 397, // candles
            524, 525, 526, 527, // candles
            1004, 1005, 1043, 1044, 1046, 1048,
            1274,
            1652,
            1879, 1880,
            1938,
            3348,
        };

        std::vector<int> moveable =
        {
            3, 4, 5, 6, 7, 8, 9, 37, 41, 45, 46, 51, 52,
            73, 114, 115, 117, 128,
            152, 153, 154, 164,
            200, 201, 206, 207,
            259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 280, 281, 282, 295, 296, 297, 298, 299, 300, 301, 306, 307, 308, 309, 310, 319,
            323, 327, 331, 332, 374,
            389, 394, 395, 396, 397, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 440, 443, 444, 445, 446, 447, 448,
            449, 450, 451, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511,
            514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 535,
            //
            668, 669, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690,
            708, 709, 710, 711, 712, 713, 714, 715, 732, 733, 734, 735, 744,
            787, 792, 793,
            //
            927, 928, 929,
            971, 988, 989, 990, 991, 992, 993, 994, 995, 1010, 1011, 1016, 1017,
            1034, 1049, 1050, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1061, 1062, 1063, 1064, 1065, 1088,
            1089, 1090, 1091, 1108, 1109, 1118, 1119, 1120, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135, 1136,
            1181, 1182, 1183, 1184, 1187, 1188, 1189, 1190, 1200, 1201, 1202, 1214, 1215, 1216,
            1217, 1218, 1219, 1220, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1242, 1244, 1245, 1246, 1247, 1248, 1249, 1250, 1251, 1252, 1263, 1268, 1269, 1270, 1271, 1272, 1277, 1278, 1279, 1280,
            1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1300, 1301, 1302, 1303, 1304, 1305, 1306, 1307, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1320, 1321, 1322, 1323, 1324, 1325, 1326, 1327, 1328, 1329, 1330, 1331, 1332, 1333, 1334, 1335, 1336, 1337, 1338, 1339, 1340, 1341, 1342, 1343, 1344,
            1345, 1346, 1347, 1354, 1408,
            1417, 1418, 1419, 1420, 1429, 1430, 1431, 1432, 1452, 1456, 1460, 1464, 1469, 1470, 1471,
            //
            1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1568, 1569, 1575, 1576, 1577, 1578, 1579, 1580, 1581, 1587, 1588, 1589, 1590,
            1605, 1647, 1652,
            1670, 1671,
            //
            1810, 1812, 1814, 1816, 1842, 1843, 1844, 1845,
            1862, 1863, 1864, 1865, 1866, 1867, 1868, 1869, 1870, 1871, 1872, 1873, 1874, 1875, 1876, 1877, 1878, 1879, 1880, 1881, 1882, 1883, 1884, 1885, 1886, 1887, 1888, 1889, 1890, 1891, 1892, 1893, 1894, 1895, 1896, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1904, 1917, 1918, 1919, 1920,
            1921, 1922, 1923, 1928, 1929, 1930, 1931, 1932, 1933, 1934, 1935, 1936, 1937, 1938,
            2086, 2090, 2094,
            2119, 2160,
            2182, 2183, 2184, 2195, 2196, 2197, 2198, 2199, 2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2210, 2211, 2212, 2213, 2214, 2215, 2216, 2217, 2218, 2219, 2220, 2221, 2222, 2223, 2224, 2225, 2226, 2227, 2228, 2229, 2230, 2231, 2232, 2233, 2234, 2235, 2236, 2237, 2238, 2239, 2240,
            2241, 2242, 2243, 2244, 2245, 2246, 2247, 2248, 2269, 2270, 2272, 2276, 2280, 2284, 2287, 2288, 2293, 2294, 2295,
            2317, 2319, 2321, 2323, 2332, 2333, 2334, 2335, 2336, 2345, 2346, 2347, 2348, 2349, 2358, 2359, 2360, 2361, 2362,
            2371, 2372, 2377, 2378, 2383, 2384, 2402, 2404, 2413, 2414, 2427, 2428, 2429,
            2450, 2451, 2452, 2465, 2466, 2467, 2468, 2469, 2470, 2471, 2472, 2473, 2474, 2475, 2476, 2477, 2478, 2479, 2480, 2481, 2482, 2495,
            2500, 2501, 2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509, 2510, 2511, 2512, 2513, 2514, 2515, 2516, 2517, 2518, 2519, 2520, 2521, 2522, 2523, 2524, 2525, 2535, 2536, 2537, 2538, 2539, 2540, 2541, 2542, 2543, 2544, 2545, 2546,
            2609, 2610, 2611, 2612, 2613, 2614, 2615, 2616, 2617, 2618, 2619, 2620, 2621, 2622,
            //
            2719, 2720, 2721, 2722, 2723, 2724,
            2760, 2764, 2768, 2773, 2806, 2807, 2808, 2809,
            2818, 2819, 2828, 2829, 2830, 2835, 2836, 2845, 2846, 2847, 2848, 2865, 2866,
            2889, 2890, 2891, 2892, 2893, 2894, 2895, 2896, 2932, 2936, 2940, 2944,
            2951, 2952, 2953, 2954, 2955, 2960, 2961, 2962, 2963, 2976, 2977, 2978, 2979, 2980, 2981, 2982, 2983, 2984, 2985, 2994, 2995, 3004, 3005, 3006, 3007, 3008,
            3009, 3010, 3011, 3012, 3013, 3014, 3015, 3016, 3017, 3018, 3019, 3020, 3021, 3022, 3023, 3024, 3035, 3048, 3049, 3050, 3051, 3068, 3069, 3070, 3071, 3072,
            3073, 3074, 3075, 3076, 3077, 3078, 3085, 3090, 3095, 3096, 3097, 3098, 3111, 3112, 3113, 3137, 3141, 3145, 3149, 3154, 3155, 3156, 3157, 3166, 3167, 3168, 3169, 3187, 3188, 3189, 3190, 3191, 3192, 3193, 3194, 3195, 3196, 3197, 3198, 3199, 3200,
            3201, 3202, 3203, 3204, 3205, 3206, 3207, 3208, 3209, 3230, 3231, 3238, 3239, 3240,
            3280, 3281, 3282, 3283, 3284, 3285, 3286, 3287, 3288, 3289, 3290, 3291, 3314, 3315, 3316, 3317, 3318, 3322, 3326,
            3331, 3332, 3333, 3334, 3335, 3336, 3357, 3358, 3359, 3360, 3361, 3367, 3368, 3369, 3370, 3371, 3372, 3373, 3374, 3380, 3381,
        };

        std::vector<int> stackable =
        {
            410, 411, 412, 413, 414, 415, 416, 417,
        };

        std::vector<int> pickupable =
        {
            3, 4, 5, 6, 7, 8, 9, 41, 45, 73, 115, 117, 128, 164, 200, 206,
            207, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 280, 281, 282,
            295, 296, 297, 298, 299, 300, 301, 306, 307, 308, 309, 310, 332, 374, 389, 410,
            411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 445, 488, 490, 498, 499,
            500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511,
            //514, // book
            515, 516, 517,
            518, 519, 520, 528, 668, 669, 678, 679, 680, 681, 682, 683, 684, 708, 709, 710,
            711, 712, 744, 787, 927, 928, 929, 971, 988, 989, 990, 991, 992, 993, 994, 995,
            1010, 1011, 1016, 1034, 1049, 1050, 1051, 1052, 1053, 1056, 1057, 1058, 1059, 1061, 1062, 1063,
            1064, 1065, 1088, 1089, 1090, 1091, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132,
            1133, 1134, 1135, 1136, 1200, 1201, 1202, 1219,
            //1220, // book
            1225, 1226, 1227, 1228, 1229, 1230, 1231,
            1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1242, 1244, 1245, 1246, 1247, 1248, 1249,
            1250, 1251, 1252, 1263, 1268, 1269, 1270, 1271, 1272, 1277, 1278, 1279, 1280, 1281, 1282, 1283,
            1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299,
            1300, 1301, 1302, 1303, 1304, 1305, 1306, 1307, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315,
            1316, 1317, 1318, 1319, 1320, 1321, 1322, 1323, 1324, 1325, 1326, 1327, 1328, 1329, 1330, 1331,
            1332, 1333, 1334, 1335, 1336, 1337, 1338, 1339, 1340, 1341, 1342, 1343, 1344, 1345, 1346, 1347,
            1354, 1469, 1470, 1471, 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1568, 1569, 1575, 1576,
            1577, 1578, 1579, 1580, 1581, 1587, 1588, 1589, 1590, 1605, 1647, 1652, 1862, 1863, 1864, 1865,
            1866, 1867, 1868, 1869, 1870, 1871, 1872, 1873, 1874, 1875, 1876, 1877, 1878, 1881, 1882, 1883,
            1884, 1885, 1886, 1887, 1888, 1889, 1890, 1891, 1892, 1893, 1894, 1895, 1896, 1897, 1898, 1899,
            1900, 1901, 1902, 1903, 1904, 1917, 1918, 1919, 1920, 1921, 1922, 1923, 1928, 1929, 1930, 1931,
            1932, 1933, 1934, 1935, 1936, 1937, 2119, 2160, 2182, 2183, 2184, 2195, 2196, 2197, 2198, 2199,
            2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2210, 2211, 2212, 2213, 2214, 2215, 2216, 2217,
            2218, 2219, 2220, 2221, 2222, 2223, 2224, 2225, 2226, 2227, 2228, 2229, 2230, 2231, 2232, 2233, 2234, 2235,
            2236, 2237, 2238, 2239, 2240, 2241, 2242, 2243, 2244, 2245, 2246, 2247, 2248, 2269, 2270, 2287, 2288, 2334,
            2335, 2336, 2347, 2348, 2349, 2360, 2361, 2362,
            //2468, 2469, 2470, 2471, 2472, 2473, 2474, 2475, 2476, 2477, 2478, 2479, 2480, 2481, 2482, // book
            2500, 2501, 2502, 2503, 2504, 2505, 2506, 2507, 2508, 2509,
            2510, 2511, 2512, 2513, 2514, 2515, 2516, 2517, 2518, 2519, 2520, 2521, 2522, 2523, 2524, 2525, 2535, 2536,
            2537, 2538, 2539, 2540, 2541, 2542, 2543, 2544, 2545, 2546, 2609, 2610, 2611, 2612, 2613, 2614, 2615, 2616,
            2617, 2618, 2619, 2620, 2621, 2622, 2719, 2720, 2721, 2722, 2723, 2724, 2889, 2890, 2891, 2892, 2893, 2894,
            2895, 2896, 2945, 2946, 2947, 2948, 2949, 2950, 2951, 2952, 2953, 2954, 2955, 2960, 2961, 2962, 2963, 2976,
            2977, 2978, 2979, 2980, 2981, 2982, 2983, 2984, 2985, 3007, 3015, 3017, 3018, 3019, 3020, 3021, 3022, 3023,
            3024, 3071, 3072, 3073, 3074, 3075, 3076, 3077, 3078, 3085, 3090, 3187, 3188, 3189, 3190, 3191, 3192, 3193,
            3194, 3195, 3196, 3197, 3198, 3199, 3200, 3201, 3202, 3203, 3204, 3205, 3206, 3207, 3208, 3209, 3238, 3280,
            3281, 3282, 3283, 3284, 3285, 3286, 3287, 3288, 3289, 3290, 3291,
            //3314, 3315, 3316, 3317, 3318, // book
            3331, 3332, 3333, 3334, 3335, 3336, 3367,
            3368, 3369, 3370, 3371, 3372, 3373, 3374, 3380, 3381,
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
            1829,
            1830, 1831, 1832, 1833, 1834, 1835, 1836, 1837,
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
            3252,
            3271, 3275, 3279,
            3322, 3326,
            3380, 3381,
            3385,
            3391,
            3497,
        };

        std::vector<int> interactive =
        {
            tibia::SpriteData::chair[0],
            tibia::SpriteData::chair[1],
            tibia::SpriteData::chair[2],
            tibia::SpriteData::chair[3],

            tibia::SpriteData::counterVertical[0],
            tibia::SpriteData::counterVertical[1],

            tibia::SpriteData::counterHorizontal[0],
            tibia::SpriteData::counterHorizontal[1],

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
            1,    // pink square
            1087, // pink square
            3388, // invisible tile
        };

        // right to left, bottom then top
        std::vector<int> quadObjects =
        {
            13, 21, 29,
            56, 60, 68, 72,
            159, 163,
            286, 290,
            315, // 5 sprites for well
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

        // right to left
        std::vector<int> horizontalObjects =
        {
            178, 182, 192,
            376,
            609,
            909, 918,
            940,
            1046,
            1067,
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
            3254, 3255, 3256, 3257, // mountain wall tops
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
            1043, 1044, 1046, 1048, // counter tops
        };

        std::vector<int> modifyHpOnTouchObjects =
        {
            // fire field
            1489, 1490,
            1491, 1492,
            //1493, 1494,

            // electricity field
            1497, 1498,

            // poison field
            3158, 3159, 3160, 3161,
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
            3391,
        };
    }

    std::vector<std::vector<int>> animatedObjectsList =
    {
        {197, 198, 199, 3391},
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

        // corpse
        {491, 492, 493, 494, 495, 496, 497},

        // portals
        {197},
        {198},
        {199},
        {3391},
    };

    std::unordered_map<int, std::vector<int>> umapCreatureBloodTypesSplats =
    {
        {tibia::CreatureBloodTypes::red,   tibia::SpriteData::splatRed},
        {tibia::CreatureBloodTypes::green, tibia::SpriteData::splatGreen},
    };

    std::unordered_map<int, std::vector<int>> umapCreatureBloodTypesPools =
    {
        {tibia::CreatureBloodTypes::red,   tibia::SpriteData::poolRed},
        {tibia::CreatureBloodTypes::green, tibia::SpriteData::poolGreen},
    };

    std::unordered_map<int, int> umapCreatureStatusEffectsIcons =
    {
        {tibia::CreatureStatusEffects::burning,     tibia::SpriteData::Gui::statusEffectBurning},
        {tibia::CreatureStatusEffects::electrified, tibia::SpriteData::Gui::statusEffectElectrified},
        {tibia::CreatureStatusEffects::poisoned,    tibia::SpriteData::Gui::statusEffectPoisoned},
    };

    namespace Sounds
    {
        sf::SoundBuffer teleport;

        namespace Creatures
        {
            namespace Human
            {
                sf::SoundBuffer death;
            }

            namespace Cacodemon
            {
                sf::SoundBuffer death;
            }
        }

        namespace Instruments
        {
            sf::SoundBuffer harp;
        }
    }

    std::unordered_map<std::string, sf::SoundBuffer&> umapSoundFiles =
    {
        {"sounds/teleport.wav", tibia::Sounds::teleport},

        {"sounds/creatures/human/death.wav", tibia::Sounds::Creatures::Human::death},

        {"sounds/creatures/cacodemon/death.wav", tibia::Sounds::Creatures::Cacodemon::death},

        {"sounds/instruments/harp.wav", tibia::Sounds::Instruments::harp},
    };

    std::unordered_map<int, sf::SoundBuffer&> umapCreatureDeathSounds =
    {
        {tibia::CreatureTypes::human, tibia::Sounds::Creatures::Human::death},

        {tibia::CreatureTypes::bear, tibia::Sounds::Creatures::Human::death},
        {tibia::CreatureTypes::cacodemon, tibia::Sounds::Creatures::Cacodemon::death},
        {tibia::CreatureTypes::citizenMale, tibia::Sounds::Creatures::Human::death},
        {tibia::CreatureTypes::citizenFemale, tibia::Sounds::Creatures::Human::death},
        {tibia::CreatureTypes::demon, tibia::Sounds::Creatures::Human::death},
        {tibia::CreatureTypes::poisonSpider, tibia::Sounds::Creatures::Human::death},
        {tibia::CreatureTypes::santaClaus, tibia::Sounds::Creatures::Human::death},
    };

    namespace CreatureAddInventoryItemResult
    {
        enum
        {
            fail,
            success,
            inventoryItemCountMax,
            inventoryItemsMax,
        };
    }

} // namespace tibia

#endif // TIBIA_TIBIA_HPP

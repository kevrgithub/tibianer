#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <ctime>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <thread>
#include <chrono>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "stlastar.h"

#include "pugixml.hpp"

#include "lua.hpp"

#include "iup.h"

#include "utility.hpp"

#include "tibia/Tibia.hpp"
#include "tibia/Game.hpp"

#include "resource.h"

lua_State* g_luaState;

tibia::Game g_game;

#include "tibia/LuaFunctions.hpp"

std::string g_gameTitle = "Tibianer (WORK IN PROGRESS)";

std::string g_configFile = "cfg/config.cfg";

sf::RenderWindow g_mainWindow;

sf::Uint32 g_windowStyle = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

unsigned int g_windowWidth  = 640;
unsigned int g_windowHeight = 480;

bool g_windowIsFocused = true;

bool g_windowIsMouseInside = false;

unsigned int g_windowFrameRateLimit = 60;

bool g_showMapSelect = true;

std::string g_mapFile = "test.tmx";

bool loadConfig()
{
    if (utility::fileExists(g_configFile) == false)
    {
        return false;
    }

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(g_configFile, pt);

    g_windowFrameRateLimit = pt.get<unsigned int>("Window.FrameRateLimit", g_windowFrameRateLimit);

    g_showMapSelect = pt.get<bool>("Map.ShowMapSelect", g_showMapSelect);

    g_mapFile = pt.get<std::string>("Map.File", g_mapFile);

    g_game.options.isGameShowFloatingTextEnabled = pt.get<bool>("Game.ShowFloatingText", g_game.options.isGameShowFloatingTextEnabled);
    g_game.options.isGameShowNamesEnabled        = pt.get<bool>("Game.ShowNames",        g_game.options.isGameShowNamesEnabled);
    g_game.options.isGameShowCreatureBarsEnabled = pt.get<bool>("Game.ShowCreatureBars", g_game.options.isGameShowCreatureBarsEnabled);
    g_game.options.isGameSmoothLightingEnabled   = pt.get<bool>("Game.ShowCreatureBars", g_game.options.isGameSmoothLightingEnabled);
    g_game.options.isGameFriendlyFireEnabled     = pt.get<bool>("Game.FriendlyFire",     g_game.options.isGameFriendlyFireEnabled);

    g_game.options.isAudioSoundEnabled           = pt.get<bool>("Audio.Sound",           g_game.options.isAudioSoundEnabled);
    g_game.options.isAudioMusicEnabled           = pt.get<bool>("Audio.Music",           g_game.options.isAudioMusicEnabled);

    g_game.options.isCheatInfiniteHealthEnabled  = pt.get<bool>("Cheats.InfiniteHealth", g_game.options.isCheatInfiniteHealthEnabled);
    g_game.options.isCheatInfiniteManaEnabled    = pt.get<bool>("Cheats.InfiniteMana",   g_game.options.isCheatInfiniteManaEnabled);

    return true;
}

void createMainWindow()
{
    g_mainWindow.create(sf::VideoMode(g_windowWidth, g_windowHeight), g_gameTitle, g_windowStyle);

    sf::Image windowIcon;
    if (windowIcon.loadFromFile("images/icon.png") == true)
    {
        g_mainWindow.setIcon(32, 32, windowIcon.getPixelsPtr());
    }

    if (g_windowFrameRateLimit > 0)
    {
        g_mainWindow.setFramerateLimit(g_windowFrameRateLimit);
    }
}

sf::View calculateMainWindowView(const sf::Vector2u &windowSize, const sf::Vector2u &designedSize)
{
    sf::FloatRect viewPort(0.0f, 0.0f, 1.0f, 1.0f);

    float screenWidth  = windowSize.x / static_cast<float>(designedSize.x);
    float screenHeight = windowSize.y / static_cast<float>(designedSize.y);

    if(screenWidth > screenHeight)
    {
        viewPort.width = screenHeight / screenWidth;
        viewPort.left = (1.0f - viewPort.width) / 2.0f;
    }
    else if(screenWidth < screenHeight)
    {
        viewPort.height = screenWidth / screenHeight;
        viewPort.top = (1.0f - viewPort.height) / 2.0f;
    }

    sf::View view(sf::FloatRect(0, 0, designedSize.x, designedSize.y));
    view.setViewport(viewPort);

    return view;
}

bool doSaveScreenshot()
{
    sf::Image screenshot = g_mainWindow.capture();

    std::stringstream screenshotName;

    for (std::uint64_t i = 0; i < sizeof(std::uint64_t); i++)
    {
        screenshotName.str("");

        screenshotName << "screenshots/screenshot" << i << ".png";

        if (utility::fileExists(screenshotName.str()) == false)
        {
            return screenshot.saveToFile(screenshotName.str());
        }
    }

    return false;
}

void mapSelectCallback(Ihandle *ih, const char* filename, const char* status)
{
    if (strcmp(status, "OK") == 0)
    {
        g_mapFile = filename;

        IupExitLoop();
    }
    else if (strcmp(status, "FINISH") == 0)
    {
        IupExitLoop();
    }
}

void showMapSelect()
{
    Ihandle *dialogMapSelect;

    dialogMapSelect = IupFileDlg();
    IupSetAttribute(dialogMapSelect, "DIALOGTYPE", "OPEN");
    IupSetAttribute(dialogMapSelect, "TITLE", "Select Map");
    IupSetAttribute(dialogMapSelect, "DIRECTORY", ".\\maps");
    IupSetAttributes(dialogMapSelect, "FILTER = \"*.tmx\", FILTERINFO = \"Tiled Map Files (*.tmx)\"");
    IupSetCallback(dialogMapSelect, "FILE_CB", (Icallback)mapSelectCallback);

    IupPopup(dialogMapSelect, IUP_CENTER, IUP_CENTER);

    IupMainLoop();
}

int main(int argc, char* argv[])
{
    g_luaState = luaL_newstate();

    luaL_openlibs(g_luaState);

    tibia::LuaFunctions::registerFunctions();

    g_game.setLuaState(g_luaState);

    IupOpen(&argc, &argv);

    std::cout << g_gameTitle << std::endl;

    if (argc == 2)
    {
        std::cout << argv[1] << std::endl;
    }

    g_game.setMainWindow(&g_mainWindow);

    unsigned int maximumTextureSize = sf::Texture::getMaximumSize();
    if (maximumTextureSize < tibia::TEXTURE_SIZE_MAX)
    {
        std::cout << "Your computer supports a maximum texture size of " << maximumTextureSize << std::endl;
        std::cout << "Error: This game requires at least " << tibia::TEXTURE_SIZE_MAX << " texture size" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading config" << std::endl;
    if (loadConfig() == false)
    {
        std::cout << "Error: Failed to load config" << std::endl;
    }

    std::cout << "Initializing random number seed" << std::endl;
    std::srand(std::time(0));

    std::cout << "Creating main window" << std::endl;
    createMainWindow();

    std::cout << "Loading fonts" << std::endl;
    if (g_game.loadFonts() == false)
    {
        std::cout << "Error: Failed to load fonts" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading bitmap fonts" << std::endl;
    if (g_game.loadBitmapFonts() == false)
    {
        std::cout << "Error: Failed to load bitmap fonts" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading sounds" << std::endl;
    if (g_game.loadSounds() == false)
    {
        std::cout << "Error: Failed to load sounds" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Drawing loading screen" << std::endl;

    sf::Texture loadingTexture;
    if (loadingTexture.loadFromFile("images/loading.png") == false)
    {
        std::cout << "Error: Failed to load loading texture" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Sprite loadingSprite;
    loadingSprite.setTexture(loadingTexture);

    tibia::BitmapFontText loadingText;
    loadingText.setText(g_game.getBitmapFontDefault(), "Loading...", tibia::Colors::Text::white, true);
    loadingText.setPosition
    (
        g_mainWindow.getSize().x / 2,
        g_mainWindow.getSize().y - (loadingText.getVertexArray()->getBounds().height + 4)
    );

    g_mainWindow.clear(tibia::Colors::black);
    g_mainWindow.draw(loadingSprite);
    g_mainWindow.draw(loadingText);
    g_mainWindow.display();

    std::cout << "Loading textures" << std::endl;
    if (g_game.loadTextures() == false)
    {
        std::cout << "Error: Failed to load textures" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading sprite flags" << std::endl;
    g_game.loadSpriteFlags();

    std::cout << "Loading map" << std::endl;

    std::stringstream mapFilePath;

    if (g_showMapSelect == true)
    {
        showMapSelect();

        mapFilePath << g_mapFile; // full path
    }
    else
    {
        mapFilePath << "maps/" << g_mapFile;
    }

    std::cout << "Map file: " << g_mapFile << std::endl;

    auto timeLoadMapBegin = std::chrono::high_resolution_clock::now();

    if (g_game.loadMap(mapFilePath.str()) == false)
    {
        std::cout << "Error: Failed to load map" << " (" << g_mapFile << ")" << std::endl;
        return EXIT_FAILURE;
    }

    if (g_game.getMap()->isCompressed() == true)
    {
        std::cout << "Map is compressed" << std::endl;
    }

    auto timeLoadMapEnd = std::chrono::high_resolution_clock::now();
    auto timeLoadMapElapsed = timeLoadMapEnd - timeLoadMapBegin;

    std::cout << "Time elapsed loading map: " << std::chrono::duration_cast<std::chrono::seconds>(timeLoadMapElapsed).count() << "s" << std::endl;

    std::cout << "Loading map scripts" << std::endl;
    g_game.loadMapScripts();

    std::cout << "Loading windows" << std::endl;
    g_game.loadWindows();

    std::cout << "Creating player" << std::endl;
    g_game.createPlayer();

    std::cout << "Starting main loop" << std::endl;

    sf::Clock* clockDelta                   = g_game.getClockDelta();
    sf::Clock* clockGame                    = g_game.getClockGame();
    sf::Clock* clockAnimatedWaterAndObjects = g_game.getClockAnimatedWaterAndObjects();

    bool doEnterGame = true;

    while (g_mainWindow.isOpen())
    {
        IupLoopStep();

        sf::Time timeGame = clockGame->getElapsedTime();

        sf::Time timeAnimatedWaterAndObjects = clockAnimatedWaterAndObjects->getElapsedTime();

        if (timeAnimatedWaterAndObjects.asSeconds() >= tibia::AnimatedObjects::time)
        {
            g_game.doAnimatedWater();
            g_game.doAnimatedObjects();

            clockAnimatedWaterAndObjects->restart();
        }

        if (doEnterGame == true)
        {
            g_game.doEnterGame();

            doEnterGame = false;
        }

        g_mainWindow.clear(tibia::Colors::black);

        sf::Sprite background;
        background.setTexture(tibia::Textures::background);
        g_mainWindow.draw(background);

        g_game.updateMouseWindowPosition(&g_mainWindow);
        g_game.updateMouseTile();

        if (g_windowIsMouseInside == true)
        {
            g_game.updateMouseCursor(&g_mainWindow);
        }

        if (g_windowIsFocused == true)
        {
            g_game.handleKeyboardInput();
            g_game.handleMouseInput();
        }

        g_game.updateLightBrightness();

        g_game.drawGameWindow(&g_mainWindow);

        g_game.drawChatLogWindow(&g_mainWindow);

        g_game.drawChatInputText(&g_mainWindow);

        g_game.drawTabButtons(&g_mainWindow);

        if (g_game.gui.topStatus == true)
        {
            g_game.drawStatusWindow(&g_mainWindow);
        }
        else if (g_game.gui.topEquipment == true)
        {
            g_game.drawEquipmentWindow(&g_mainWindow);
        }
        else if (g_game.gui.topMiniMap == true)
        {
            g_game.drawMiniMapWindow(&g_mainWindow);
            g_game.updateMiniMapWindow();
        }

        if (g_game.gui.bottomInventory == true)
        {
            g_game.drawInventoryWindow(&g_mainWindow);
        }
        else if (g_game.gui.bottomSkills == true)
        {
            g_game.drawSkillsWindow(&g_mainWindow);
            g_game.drawOutfitButtons(&g_mainWindow);
        }
        else if (g_game.gui.bottomCombat == true)
        {
            g_game.drawCombatWindow(&g_mainWindow);
            g_game.drawCombatButtons(&g_mainWindow);
        }

        g_game.drawBars(&g_mainWindow);

        g_game.drawStatusBarText(&g_mainWindow);

        g_game.drawStatusEffectIcons(&g_mainWindow);

        g_game.updateSounds();

        g_mainWindow.display();

        sf::Event event;
        while (g_mainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                g_mainWindow.close();
            }

            else if (event.type == sf::Event::LostFocus)
            {
                g_windowIsFocused = false;
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                g_windowIsFocused = true;
            }

            //else if (event.type == sf::Event::Resized)
            //{
                // aspect ratio correction
                //g_mainWindow.setView(calculateMainWindowView(g_mainWindow.getSize(), sf::Vector2u(g_windowWidth, g_windowHeight)));
            //}

            else if (event.type == sf::Event::TextEntered)
            {
                g_game.handleTextEnteredEvent(event);
            }

            else if (event.type == sf::Event::KeyPressed)
            {
                // quit
                if (event.key.code == sf::Keyboard::Escape)
                {
                    g_mainWindow.close();
                }
                // take screenshot
                else if (event.key.code == sf::Keyboard::Key::F12)
                {
                    if (doSaveScreenshot() == true)
                    {
                        std::cout << "Saved screenshot successfully" << std::endl;
                    }
                    else
                    {
                        std::cout << "Save screenshot failed" << std::endl;
                    }
                }

                g_game.handleKeyboardEvent(event);
            }

            else if (event.type == sf::Event::MouseEntered)
            {
                g_windowIsMouseInside = true;
            }
            else if (event.type == sf::Event::MouseLeft)
            {
                g_windowIsMouseInside = false;
            }

            else if (event.type == sf::Event::MouseButtonPressed)
            {
                g_game.handleMouseEvent(event);
            }

            else if (event.type == sf::Event::MouseWheelMoved)
            {
                g_game.handleMouseWheelEvent(event);
            }
        }

        // delta time
        sf::Time timeDelta = clockDelta->restart();

        // 1.0f / 60.0f = 0.0166666666666667

        //if (timeDelta.asSeconds() < 0.01)
        //{
            //timeDelta = sf::seconds(0.005);
        //}

        //std::cout << "timeDelta: " << timeDelta.asSeconds() << std::endl;

        g_game.setTimeDelta(timeDelta);
    }

    IupClose();

    lua_close(g_luaState);

    return EXIT_SUCCESS;
}

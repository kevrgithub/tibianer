#include <cstdlib>
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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "utility.hpp"

#include "tibia/Tibia.hpp"
#include "tibia/Game.hpp"

#include "resource.h"

tibia::Game g_game;

std::string gameTitle = "Tibianer";

std::string configFile = "cfg/config.cfg";

sf::RenderWindow mainWindow;

sf::Uint32 windowStyle = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

unsigned int windowWidth  = 640;
unsigned int windowHeight = 480;

bool windowIsFocused = true;

bool windowIsFullscreen = false;

unsigned int windowFrameRateLimit = 60;

bool mouseUseDefaultCursor = true;

float zoomLevel  = 1;
float zoomFactor = 0.4;

std::string mapFile = "test.tmx";

bool loadConfig()
{
    if (utility::fileExists(configFile) == false)
    {
        return false;
    }

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(configFile, pt);

    windowIsFullscreen = pt.get<bool>("Window.Fullscreen", windowIsFullscreen);

    windowFrameRateLimit = pt.get<unsigned int>("Window.FrameRateLimit", windowFrameRateLimit);

    mouseUseDefaultCursor = pt.get<bool>("Mouse.UseDefaultCursor", mouseUseDefaultCursor);

    mapFile = pt.get<std::string>("Map.File", mapFile);

    return true;
}

void doMainWindowFullScreen()
{
    mainWindow.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
    mainWindow.setPosition(sf::Vector2i(0, 0));
}

void createMainWindow(bool fullscreen)
{
    sf::Uint32 tempStyle = windowStyle;

    if (fullscreen == true)
    {
        tempStyle = sf::Style::None;
    }

    mainWindow.create(sf::VideoMode(windowWidth, windowHeight), gameTitle, tempStyle);

    if (fullscreen == true)
    {
        doMainWindowFullScreen();
    }

    sf::Image windowIcon;
    if (windowIcon.loadFromFile("images/icon.png") == true)
    {
        mainWindow.setIcon(32, 32, windowIcon.getPixelsPtr());
    }

    if (windowFrameRateLimit > 0)
    {
        mainWindow.setFramerateLimit(windowFrameRateLimit);
    }

    if (mouseUseDefaultCursor == false)
    {
        mainWindow.setMouseCursorVisible(false);
    }
}

bool doSaveScreenshot()
{
    sf::Image screenshot = mainWindow.capture();

    std::stringstream screenshotName;

    for (unsigned int i = 0; i < sizeof(unsigned int); i++)
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

int main(int argc, char* argv[])
{
    std::cout << gameTitle << std::endl;

    if (argc == 2)
    {
        std::cout << argv[1] << std::endl;
    }

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
    createMainWindow(false);

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
    loadingText.setText(g_game.getBitmapFontDefault(), "Loading...", tibia::Colors::textWhite, true);
    loadingText.setPosition
    (
        mainWindow.getSize().x / 2,
        mainWindow.getSize().y - (loadingText.getVertexArray()->getBounds().height + 4)
    );

    mainWindow.clear(tibia::Colors::black);
    mainWindow.draw(loadingSprite);
    mainWindow.draw(loadingText);
    mainWindow.display();

    std::cout << "Creating game windows" << std::endl;
    if (g_game.createWindows() == false)
    {
        std::cout << "Error: Failed to create game windows" << std::endl;
        return EXIT_FAILURE;
    }

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
    mapFilePath << "maps/" << mapFile;

    if (g_game.loadMap(mapFilePath.str()) == false)
    {
        std::cout << "Error: Failed to load map" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading game view" << std::endl;
    sf::View* gameView = g_game.getGameWindowView();

    std::cout << "Creating player" << std::endl;
    g_game.createPlayer();

    std::cout << "Updating main window" << std::endl;
    createMainWindow(windowIsFullscreen);

    std::cout << "Starting main loop" << std::endl;

    sf::Clock* clockDelta                   = g_game.getClockDelta();
    sf::Clock* clockGame                    = g_game.getClockGame();
    sf::Clock* clockAnimatedWaterAndObjects = g_game.getClockAnimatedWaterAndObjects();

    bool doEnterGame = true;

    while (mainWindow.isOpen())
    {
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

        mainWindow.clear(tibia::Colors::mainWindowColor);

        sf::Sprite background;
        background.setTexture(tibia::Textures::background);
        background.setPosition(0, 0);
        mainWindow.draw(background);

        g_game.updateMouseWindowPosition(&mainWindow);
        g_game.updateMouseTile();

        if (windowIsFocused == true)
        {
            g_game.handleKeyboardInput();
            g_game.handleMouseInput();
        }

        g_game.updateLightBrightness();

        g_game.drawGameWindow(&mainWindow);

        g_game.drawMiniMapWindow(&mainWindow);
        g_game.updateMiniMapWindow();

        if (mouseUseDefaultCursor == false)
        {
            g_game.drawMouseCursor(&mainWindow);
        }

        g_game.updateSounds();

        mainWindow.display();

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }

            else if (event.type == sf::Event::LostFocus)
            {
                windowIsFocused = false;
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                windowIsFocused = true;
            }

            else if (event.type == sf::Event::KeyPressed)
            {
                // quit
                if (event.key.code == sf::Keyboard::Escape)
                {
                    mainWindow.close();
                }
                // toggle fullscreen
                else if (event.key.code == sf::Keyboard::F11 || (event.key.alt == true && event.key.code == sf::Keyboard::Return))
                {
                    utility::toggleBool(windowIsFullscreen);

                    createMainWindow(windowIsFullscreen);
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

            else if (event.type == sf::Event::MouseButtonPressed)
            {
                g_game.handleMouseEvent(event);
            }

            else if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta > 0)
                {
                    zoomLevel -= zoomFactor;

                    if (zoomLevel < 1) zoomLevel = 1;

                    gameView->setSize(sf::Vector2f(tibia::TILES_WIDTH * zoomLevel, tibia::TILES_HEIGHT * zoomLevel));
                }
                else if (event.mouseWheel.delta < 0)
                {
                    zoomLevel += zoomFactor;

                    gameView->setSize(sf::Vector2f(tibia::TILES_WIDTH * zoomLevel, tibia::TILES_HEIGHT * zoomLevel));
                }
            }
        }

        // delta time
        sf::Time timeDelta = clockDelta->restart();

        // 1.0f / 60.0f = 0.0166666666666667

        if (timeDelta.asSeconds() < 0.01)
        {
            timeDelta = sf::seconds(0.005);
        }

        //std::cout << "timeDelta: " << timeDelta.asSeconds() << std::endl;

        g_game.setTimeDelta(timeDelta);
    }

    return EXIT_SUCCESS;
}

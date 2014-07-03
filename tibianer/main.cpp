#include <cstdlib>
#include <ctime>
#include <cmath>
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

std::string gameTitle = "Tibianer";

std::string fileCfgConfig   = "cfg/config.cfg";
std::string fileDataSprites = "data/sprites.ini";
std::string fileDataNames   = "data/names.txt";

sf::Uint32 windowStyle = sf::Style::Titlebar | sf::Style::Close;

unsigned int windowWidth  = 640;
unsigned int windowHeight = 480;

bool windowIsFullscreen = false;

unsigned int windowFrameRateLimit = 60;

bool mouseUseDefaultCursor = true;

float zoomLevel  = 1;
float zoomFactor = 0.4;

bool loadConfig()
{
    if (fileExists(fileCfgConfig) == false)
    {
        return false;
    }

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(fileCfgConfig, pt);

    windowIsFullscreen   = pt.get<bool>("Window.Fullscreen", windowIsFullscreen);
    windowFrameRateLimit = pt.get<unsigned int>("Window.FrameRateLimit", windowFrameRateLimit);

    mouseUseDefaultCursor = pt.get<bool>("Mouse.UseDefaultCursor", mouseUseDefaultCursor);

    return true;
}

int main()
{
    std::cout << "Loading config" << std::endl;
    if (loadConfig() == false)
    {
        std::cout << "Error: Failed to load config" << std::endl;
    }

    std::cout << "Initializing random number seed" << std::endl;
    std::srand(std::time(0));

    std::cout << "Creating main window" << std::endl;

    if (windowIsFullscreen == true)
    {
        windowStyle |= sf::Style::Fullscreen;
    }

    sf::RenderWindow mainWindow;
    mainWindow.create(sf::VideoMode(windowWidth, windowHeight), gameTitle, windowStyle);

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

    tibia::Game game;

    std::cout << "Loading fonts" << std::endl;
    if (game.loadFonts() == false)
    {
        std::cout << "Error: Failed to load fonts" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture loadingTexture;
    sf::Sprite loadingSprite;

    bool useLoadingTexture = false;

    if (loadingTexture.loadFromFile("images/tibia.png") == true)
    {
        useLoadingTexture = true;

        loadingSprite.setTexture(loadingTexture);

        loadingSprite.setPosition
        (
            0,
            (windowHeight / 2) - (loadingSprite.getLocalBounds().height / 2)
        );
    }

    sf::Text titleText(gameTitle, *game.getFontDefault(), tibia::FontSizes::title);
    titleText.setColor(tibia::Colors::yellow);
    titleText.setPosition
    (
        (windowWidth  / 2) - (titleText.getLocalBounds().width  / 2),
        (windowHeight / 4) - (titleText.getLocalBounds().height / 2)
    );

    sf::Text loadingText("Loading...", *game.getFontDefault(), tibia::FontSizes::default);
    loadingText.setColor(tibia::Colors::white);
    loadingText.setPosition
    (
        (windowWidth  / 2)                  - (loadingText.getLocalBounds().width  / 2),
        (windowHeight - (windowHeight / 4)) - (loadingText.getLocalBounds().height / 2)
    );

    mainWindow.clear(tibia::Colors::black);

    if (useLoadingTexture == true)
    {
        mainWindow.draw(loadingSprite);
    }
    else
    {
        mainWindow.draw(titleText);
        mainWindow.draw(loadingText);
    }

    mainWindow.display();

    std::cout << "Creating game windows" << std::endl;
    if (game.createWindows() == false)
    {
        std::cout << "Error: Failed to create game windows" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading textures" << std::endl;
    if (game.loadTextures() == false)
    {
        std::cout << "Error: Failed to load textures" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading sprite flags" << std::endl;
    game.loadSpriteFlags();

    std::cout << "Loading map" << std::endl;
    if (game.loadMap("maps/test.tmx") == false)
    {
        std::cout << "Error: Failed to load map" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading game window and view" << std::endl;
    sf::RenderTexture* gameWindow = game.getGameWindow();
    sf::View* gameView = game.getGameWindowView();

    std::cout << "Creating player" << std::endl;
    game.createPlayer();

    std::cout << "Starting main loop" << std::endl;

    sf::Clock* clockGame                    = game.getClock();
    sf::Clock* clockAnimatedWaterAndObjects = game.getClockAnimatedWaterAndObjects();

    while (mainWindow.isOpen())
    {
        sf::Time elapsedTime = clockGame->getElapsedTime();

        sf::Time timeAnimatedWaterAndObjects = clockAnimatedWaterAndObjects->getElapsedTime();

        if (timeAnimatedWaterAndObjects.asSeconds() >= 1.0)
        {
            game.doAnimatedWater();
            game.doAnimatedObjects();

            clockAnimatedWaterAndObjects->restart();
        }

        mainWindow.clear(tibia::Colors::mainWindowColor);

        sf::Sprite background;
        background.setTexture(tibia::Textures::background);
        background.setPosition(0, 0);
        mainWindow.draw(background);

        game.updateMouseTile(&mainWindow);

        game.drawGameWindow(&mainWindow);

        if (mouseUseDefaultCursor == false)
        {
            game.drawMouseCursor(&mainWindow);
        }

        mainWindow.display();

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }

            if (event.type == sf::Event::MouseWheelMoved)
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

            if (event.type == sf::Event::MouseButtonPressed)
            {
                game.handleMouseInput(event);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    mainWindow.close();
                }

                game.handleKeyboardInput(event);
            }
        }
    }

    return EXIT_SUCCESS;
}

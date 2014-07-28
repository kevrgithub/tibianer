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

tibia::Game game;

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
    if (fileExists(configFile) == false)
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

        if (fileExists(screenshotName.str()) == false)
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
    if (game.loadFonts() == false)
    {
        std::cout << "Error: Failed to load fonts" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading bitmap fonts" << std::endl;
    if (game.loadBitmapFonts() == false)
    {
        std::cout << "Error: Failed to load bitmap fonts" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading sounds" << std::endl;
    if (game.loadSounds() == false)
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
    loadingText.setText(game.getBitmapFontDefault(), "Loading...", tibia::Colors::textWhite, true);
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

    std::stringstream mapFilePath;
    mapFilePath << "maps/" << mapFile;

    if (game.loadMap(mapFilePath.str()) == false)
    {
        std::cout << "Error: Failed to load map" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading game view" << std::endl;
    sf::View* gameView = game.getGameWindowView();

    std::cout << "Creating player" << std::endl;
    game.createPlayer();

    std::cout << "Updating main window" << std::endl;
    createMainWindow(windowIsFullscreen);

    std::cout << "Starting main loop" << std::endl;

    sf::Clock* clockDelta                   = game.getClockDelta();
    sf::Clock* clockGame                    = game.getClockGame();
    sf::Clock* clockAnimatedWaterAndObjects = game.getClockAnimatedWaterAndObjects();

    bool doEnterGame = true;

    while (mainWindow.isOpen())
    {
        sf::Time timeGame = clockGame->getElapsedTime();

        sf::Time timeAnimatedWaterAndObjects = clockAnimatedWaterAndObjects->getElapsedTime();

        if (timeAnimatedWaterAndObjects.asSeconds() >= tibia::AnimatedObjects::time)
        {
            game.doAnimatedWater();
            game.doAnimatedObjects();

            clockAnimatedWaterAndObjects->restart();
        }

        if (doEnterGame == true)
        {
            game.doEnterGame();

            doEnterGame = false;
        }

        mainWindow.clear(tibia::Colors::mainWindowColor);

        sf::Sprite background;
        background.setTexture(tibia::Textures::background);
        background.setPosition(0, 0);
        mainWindow.draw(background);

        game.updateMouseWindowPosition(&mainWindow);
        game.updateMouseTile();

        if (windowIsFocused == true)
        {
            game.handleKeyboardInput();
            game.handleMouseInput();
        }

        game.updateLightBrightness();

        game.drawGameWindow(&mainWindow);

        game.drawMiniMapWindow(&mainWindow);
        game.updateMiniMapWindow();

        if (mouseUseDefaultCursor == false)
        {
            game.drawMouseCursor(&mainWindow);
        }

        game.updateSounds();

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
                    toggleBool(windowIsFullscreen);

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

                game.handleKeyboardEvent(event);
            }

            else if (event.type == sf::Event::MouseButtonPressed)
            {
                game.handleMouseEvent(event);
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
        game.setTimeDelta(clockDelta->restart());
    }

    return EXIT_SUCCESS;
}

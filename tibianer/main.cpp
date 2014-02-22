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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Game.hpp"
#include "tibia/Tile.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/Text.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/Sprite.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Player.hpp"
#include "tibia/Animation.hpp"
#include "tibia/Projectile.hpp"

std::string gameTitle = "Tibianer";

std::string fileOptions = "data/options.ini";
std::string fileSprites = "data/sprites.ini";

sf::Uint32 windowStyle = sf::Style::Default;

int windowWidth  = 640;
int windowHeight = 480;

bool windowIsFullscreen = false;

unsigned int framerateLimit = 60;

float zoomLevel  = 1;
float zoomFactor = 0.4;

bool fileExists(std::string filename)
{
    std::ifstream file(filename.c_str());

    if (file.good())
    {
        file.close();
        return true;
    }
    else
    {
        file.close();
        return false;
    }
}

void loadOptions()
{
    if (fileExists(fileOptions) == false)
    {
        return;
    }

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(fileOptions, pt);

    windowIsFullscreen = pt.get<bool>("Window.Fullscreen", windowIsFullscreen);
}

int main()
{
    std::cout << "Loading options" << std::endl;

    loadOptions();

    std::cout << "Initializing random number seed" << std::endl;
    std::srand(std::time(0));

    std::cout << "Creating main window" << std::endl;

    if (windowIsFullscreen == true)
    {
        windowStyle |= sf::Style::Fullscreen;
    }

    sf::RenderWindow mainWindow;
    mainWindow.create(sf::VideoMode(windowWidth, windowHeight), gameTitle, windowStyle);
    //mainWindow.setFramerateLimit(framerateLimit);

    tibia::Game game;

    std::cout << "Loading fonts" << std::endl;
    if (game.loadFonts() == false)
    {
        std::cout << "Error: Failed to load fonts" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text titleText(gameTitle, *game.getFontMartel(), tibia::FontSizes::title);
    titleText.setColor(tibia::Colors::yellow);
    titleText.setPosition
    (
        (windowWidth  / 2) - (titleText.getLocalBounds().width  / 2),
        (windowHeight / 4) - (titleText.getLocalBounds().height / 2)
    );

    sf::Text loadingText("Loading...", *game.getFont(), tibia::FontSizes::default);
    loadingText.setColor(tibia::Colors::white);
    loadingText.setPosition
    (
        (windowWidth  / 2)                  - (loadingText.getLocalBounds().width  / 2),
        (windowHeight - (windowHeight / 4)) - (loadingText.getLocalBounds().height / 2)
    );

    mainWindow.clear(tibia::Colors::black);
    mainWindow.draw(titleText);
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

    std::cout << "Loading sounds" << std::endl;
    if (game.loadSounds() == false)
    {
        std::cout << "Error: Failed to load sounds" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading creatures" << std::endl;
    game.loadCreatures();

    for (int i = 0; i < 100; i++)
    {
        std::stringstream creatureName;

        creatureName << "Good Guy #" << i + 1;

        tibia::Game::creatureSharedPtr creatureGood(new tibia::Creature);
        creatureGood->setName(creatureName.str());
        creatureGood->setTeam(tibia::Teams::good);
        creatureGood->setHasOutfit(true);
        creatureGood->setOutfitRandom();
        creatureGood->setCoords(11, 8);

        game.spawnCreature(creatureGood);

        creatureName.str("");

        creatureName << "Evil Guy #" << i + 1;

        tibia::Game::creatureSharedPtr creatureEvil(new tibia::Creature);
        creatureEvil->setName(creatureName.str());
        creatureEvil->setTeam(tibia::Teams::evil);
        creatureEvil->setHasOutfit(true);
        creatureEvil->setOutfitRandom();
        creatureEvil->setCoords(61, 20);

        game.spawnCreature(creatureEvil);
    }

    tibia::Game::creatureSharedPtr creatureGoodLeader(new tibia::Creature);
    creatureGoodLeader->setName("Good Leader");
    creatureGoodLeader->setTeam(tibia::Teams::good);
    creatureGoodLeader->setType(tibia::CreatureTypes::gameMaster);
    creatureGoodLeader->setPropertiesByType();
    creatureGoodLeader->setCoords(9, 12);

    game.spawnCreature(creatureGoodLeader);

    tibia::Game::creatureSharedPtr creatureEvilLeader(new tibia::Creature);
    creatureEvilLeader->setName("Evil Leader");
    creatureEvilLeader->setTeam(tibia::Teams::evil);
    creatureEvilLeader->setType(tibia::CreatureTypes::hero);
    creatureEvilLeader->setPropertiesByType();
    creatureEvilLeader->setCoords(63, 20);

    game.spawnCreature(creatureEvilLeader);

    std::cout << "Loading animated objects" << std::endl;
    game.loadAnimatedObjects();

    std::cout << "Loading map" << std::endl;
    if (game.loadMap("maps/test.xml") == false)
    {
        std::cout << "Error: Failed to load map" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading player" << std::endl;

    tibia::Creature* player = game.getPlayer();
    player->setCoords(10, 10);

    bool playerMovementReady = true;

    std::cout << "Loading game window and view" << std::endl;

    sf::RenderTexture* gameWindow = game.getWindow();

    sf::View* gameView = game.getWindowView();

    std::cout << "Starting main loop" << std::endl;

    sf::Clock* clockGame                    = game.getClock();
    sf::Clock* clockAnimatedWaterAndObjects = game.getClockAnimatedWaterAndObjects();
    sf::Clock* clockCreatureLogic           = game.getClockCreatureLogic();
    sf::Clock* clockMiniMap                 = game.getClockMiniMap();

    sf::Clock clockDebugInfo;

    sf::Clock clockFramesPerSecond;
    int numFrames = 0;
    int framesPerSecond = 0;

    bool doEnterGameAnimation = true;

    bool doUpdateMiniMap = true;

    while (mainWindow.isOpen())
    {
        sf::Time elapsedTime = clockGame->getElapsedTime();

        sf::Time timeDebugInfo = clockDebugInfo.getElapsedTime();

        if (timeDebugInfo.asSeconds() >= 1.0)
        {
            //std::cout << "elapsed time: " << elapsedTime.asSeconds() << std::endl;

            std::cout << "player x,y,z:       " << player->getX()     << "," << player->getY()     << "," << player->getZ() << std::endl;
            std::cout << "player tile x,y:    " << player->getTileX() << "," << player->getTileY()                          << std::endl;

            std::cout << "player tile number: " << player->getTileNumber() << std::endl;

            clockDebugInfo.restart();
        }

        mainWindow.clear(tibia::Colors::mainWindowColor);

        sf::Time timeAnimatedWaterAndObjects = clockAnimatedWaterAndObjects->getElapsedTime();

        if (timeAnimatedWaterAndObjects.asSeconds() >= 1.0)
        {
            game.doAnimatedWater();
            game.doAnimatedObjects();

            clockAnimatedWaterAndObjects->restart();
        }

        sf::Time timeCreatureLogic = clockCreatureLogic->getElapsedTime();

        if (timeCreatureLogic.asSeconds() >= 1.0)
        {
            game.doCreatureLogic();

            clockCreatureLogic->restart();
        }

        game.updateAnimatedDecals();

        game.updatePlayer();
        game.updateCreatures();

        game.updateProjectiles();

        game.updateAnimations();

        game.drawGameWindow(&mainWindow);

        if (doUpdateMiniMap == true)
        {
            game.updateMiniMapWindow();

            doUpdateMiniMap = false;
        }

        game.drawMiniMapWindow(&mainWindow);

        sf::Time timeMiniMap = clockMiniMap->getElapsedTime();

        if (timeMiniMap.asSeconds() >= 0.1)
        {
            game.updateMiniMapWindow();

            clockMiniMap->restart();
        }

        game.updateSounds();

        if (doEnterGameAnimation == true)
        {
            game.spawnAnimation(tibia::Animations::spellBlue, game.getPlayer()->getX(), game.getPlayer()->getY(), game.getPlayer()->getZ());

            doEnterGameAnimation = false;
        }

        numFrames++;

        sf::Time timeFramesPerSecond = clockFramesPerSecond.getElapsedTime();

        if (timeFramesPerSecond.asSeconds() >= 1)
        {
            framesPerSecond = numFrames;

            numFrames = 0;

            clockFramesPerSecond.restart();
        }

        std::stringstream ssFramesPerSecond;
        ssFramesPerSecond << "FPS: " << framesPerSecond;

        sf::Text textFramesPerSecond;
        textFramesPerSecond.setString(ssFramesPerSecond.str());
        textFramesPerSecond.setFont(*game.getFontSmall());
        textFramesPerSecond.setCharacterSize(tibia::FontSizes::small);
        textFramesPerSecond.setColor(tibia::Colors::pink);
        textFramesPerSecond.setPosition(8, 0);

        mainWindow.draw(textFramesPerSecond);

        mainWindow.display();

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
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

            switch (event.type)
            {
                case sf::Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                            mainWindow.close();
                            break;

                        case sf::Keyboard::Up:
                        case sf::Keyboard::Right:
                        case sf::Keyboard::Down:
                        case sf::Keyboard::Left:
                            game.updatePlayer();
                            game.handleCreatureMovement(player, tibia::getDirectionByKey(event.key.code), event.key.control);
                            break;

                        case sf::Keyboard::O:
                            player->setOutfitRandom();

                            std::cout << "player head: " << player->getOutfitHead() << std::endl;
                            std::cout << "player body: " << player->getOutfitBody() << std::endl;
                            std::cout << "player legs: " << player->getOutfitLegs() << std::endl;
                            std::cout << "player feet: " << player->getOutfitFeet() << std::endl;
                            break;

                        case sf::Keyboard::Z:
                            player->setZ(tibia::getRandomNumber(tibia::ZAxis::underGround, tibia::ZAxis::aboveGround));

                            std::cout << "player z: " << player->getZ() << std::endl;
                            break;

                        case sf::Keyboard::H:
                            game.showGameText("You say:\nHello!", tibia::FontSizes::game, tibia::Colors::yellow);
                            break;

                        case sf::Keyboard::A:
                            game.spawnAnimation(tibia::Animations::spellBlue, player->getTileX(), player->getTileY(), player->getZ(), 1.0);
                            break;

                        case sf::Keyboard::D:
                            game.spawnAnimatedDecal(tibia::AnimatedDecals::poolRed, player->getTileX(), player->getTileY(), player->getZ(), 30.0);
                            game.spawnAnimatedDecal(tibia::AnimatedDecals::corpse,  player->getTileX(), player->getTileY(), player->getZ(), 30.0);
                            break;

                        case sf::Keyboard::B:
                            game.spawnProjectile
                            (
                                player,
                                tibia::ProjectileTypes::spellBlue,
                                player->getDirection(),
                                sf::Vector2f(player->getTileX(), player->getTileY()),
                                sf::Vector2f
                                (
                                    player->getTileX() + (tibia::getVectorByDirection(player->getDirection()).x * tibia::TILE_SIZE),
                                    player->getTileY() + (tibia::getVectorByDirection(player->getDirection()).y * tibia::TILE_SIZE)
                                )
                            );
                            break;

                        case sf::Keyboard::F:
                            game.spawnProjectile
                            (
                                player,
                                tibia::ProjectileTypes::spellFire,
                                player->getDirection(),
                                sf::Vector2f(player->getTileX(), player->getTileY()),
                                sf::Vector2f
                                (
                                    player->getTileX() + (tibia::getVectorByDirection(player->getDirection()).x * tibia::TILE_SIZE),
                                    player->getTileY() + (tibia::getVectorByDirection(player->getDirection()).y * tibia::TILE_SIZE)
                                )
                            );
                            break;

                        case sf::Keyboard::P:
                            game.spawnProjectile
                            (
                                player,
                                tibia::ProjectileTypes::arrowPoison,
                                player->getDirection(),
                                sf::Vector2f(player->getTileX(), player->getTileY()),
                                sf::Vector2f
                                (
                                    player->getTileX() + (tibia::getVectorByDirection(player->getDirection()).x * tibia::TILE_SIZE),
                                    player->getTileY() + (tibia::getVectorByDirection(player->getDirection()).y * tibia::TILE_SIZE)
                                )
                            );
                            break;

                        case sf::Keyboard::S:
                            for (int i = tibia::Directions::begin; i < tibia::Directions::end + 1; i++)
                            {
                                game.spawnProjectile
                                (
                                    player,
                                    tibia::ProjectileTypes::spear,
                                    i,
                                    sf::Vector2f(player->getTileX(), player->getTileY()),
                                    sf::Vector2f
                                    (
                                        player->getTileX() + (tibia::getVectorByDirection(i).x * tibia::TILE_SIZE),
                                        player->getTileY() + (tibia::getVectorByDirection(i).y * tibia::TILE_SIZE)
                                    )
                                );
                            }
                            break;

                        case sf::Keyboard::C:
                            for (auto creature : game.getCreaturesList())
                            {
                                std::cout  << "name: " << creature->getName() << std::endl;

                                creature->setOutfitRandom();

                                float distance = creature->getDistanceFromPlayer();

                                std::cout << "distance: " << distance << std::endl;

                                float volume = tibia::calculateVolumeByDistance(distance);

                                std::cout << "volume: " << volume << std::endl;
                            }
                            break;
                    }
                }
                break;

                case sf::Event::MouseButtonPressed:
                {
                    sf::Vector2i mouseWindowPosition = sf::Mouse::getPosition(mainWindow);

                    std::cout << "mouse x,y: " << mouseWindowPosition.x << "," << mouseWindowPosition.y << std::endl;

                    sf::Vector2f mouseTilePositionFloat = gameWindow->mapPixelToCoords(mouseWindowPosition);

                    if (mouseTilePositionFloat.x < 0) mouseTilePositionFloat.x = 0;
                    if (mouseTilePositionFloat.y < 0) mouseTilePositionFloat.y = 0;

                    mouseTilePositionFloat.x -= tibia::GuiData::gameWindowX;
                    mouseTilePositionFloat.y -= tibia::GuiData::gameWindowY;

                    sf::Vector2u mouseTilePosition;
                    mouseTilePosition.x = mouseTilePositionFloat.x;
                    mouseTilePosition.y = mouseTilePositionFloat.y;

                    while (mouseTilePosition.x > 0 && mouseTilePosition.x % tibia::TILE_SIZE != 0) mouseTilePosition.x--;
                    while (mouseTilePosition.y > 0 && mouseTilePosition.y % tibia::TILE_SIZE != 0) mouseTilePosition.y--;

                    std::cout << "mouse tile x,y: " << mouseTilePosition.x << "," << mouseTilePosition.y << std::endl;

                    int mouseTileNumber = tibia::getTileNumberByTileCoords(mouseTilePosition.x, mouseTilePosition.y);

                    std::cout << "mouse tile number: " << mouseTileNumber << std::endl;

                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Button::Left:
                        {
                            sf::Vector2f playerTilePositionFloat;
                            playerTilePositionFloat.x = player->getTileX();
                            playerTilePositionFloat.y = player->getTileY();

                            sf::Vector2f playerMovementNormal = tibia::getNormalByVectors(playerTilePositionFloat, mouseTilePositionFloat);

                            std::cout << playerMovementNormal.x << "," << playerMovementNormal.y << std::endl;

                            playerMovementNormal.x = tibia::getFloatNormalEx(playerMovementNormal.x);
                            playerMovementNormal.y = tibia::getFloatNormalEx(playerMovementNormal.y);

                            std::cout << playerMovementNormal.x << "," << playerMovementNormal.y << std::endl;

                            int playerMovementDirection = tibia::getDirectionByVector(playerMovementNormal);

                            game.handleCreatureMovement(player, playerMovementDirection);
                            break;
                        }

                        case sf::Mouse::Button::Right:
                        {
                            if (game.doCreatureUseLadder(player, mouseTilePosition) == true)
                            {
                                break;
                            }

                            if (game.doCreatureUseLever(player, mouseTilePosition) == true)
                            {
                                break;
                            }

                            if (player->getTileNumber() == mouseTileNumber)
                            {
                                break;
                            }

                            game.spawnProjectile
                            (
                                player,
                                tibia::ProjectileTypes::arrow,
                                player->getDirection(),
                                sf::Vector2f(player->getTileX(), player->getTileY()),
                                sf::Vector2f
                                (
                                    mouseTilePosition.x,
                                    mouseTilePosition.y
                                ),
                                true
                            );

                            break;
                        }
                    }
                }
                break;
            }

            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }
        }
    }

    return EXIT_SUCCESS;
}

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
#include "tibia/Animation.hpp"
#include "tibia/Projectile.hpp"

std::string gameTitle = "Tibianer";

std::string fileOptions = "data/options.ini";
std::string fileSprites = "data/sprites.ini";

sf::Uint32 windowStyle = sf::Style::Titlebar | sf::Style::Close;

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

    std::cout << "Loading creatures" << std::endl;
    game.loadCreatures();

    for (int i = 0; i < 100; i++)
    {
        std::stringstream creatureName;

        creatureName << "Good Guy #" << i + 1;

        tibia::Game::creatureSharedPtr creatureGood = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
        creatureGood->setName(creatureName.str());
        creatureGood->setTeam(tibia::Teams::good);
        creatureGood->setHasOutfit(true);
        creatureGood->setOutfitRandom();
        creatureGood->setCoords(11, 8);

        game.spawnCreature(creatureGood);

        creatureName.str("");

        creatureName << "Evil Guy #" << i + 1;

        tibia::Game::creatureSharedPtr creatureEvil = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
        creatureEvil->setName(creatureName.str());
        creatureEvil->setTeam(tibia::Teams::evil);
        creatureEvil->setHasOutfit(true);
        creatureEvil->setOutfitRandom();
        creatureEvil->setCoords(61, 20);

        game.spawnCreature(creatureEvil);
    }

    tibia::Game::creatureSharedPtr creatureGoodLeader = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
    creatureGoodLeader->setName("Good Leader");
    creatureGoodLeader->setType(tibia::CreatureTypes::gameMaster);
    creatureGoodLeader->setTeam(tibia::Teams::good);
    creatureGoodLeader->setPropertiesByType();
    creatureGoodLeader->setHpMax(1000);
    creatureGoodLeader->setHp(1000);
    creatureGoodLeader->setCoords(9, 12);

    game.spawnCreature(creatureGoodLeader);

    tibia::Game::creatureSharedPtr creatureAvatar = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
    creatureAvatar->setName("Good Avatar");
    creatureAvatar->setTeam(tibia::Teams::good);
    creatureAvatar->setType(tibia::CreatureTypes::hero);
    creatureAvatar->setPropertiesByType();
    creatureAvatar->setHpMax(1000);
    creatureAvatar->setHp(1000);
    creatureAvatar->setCoords(11, 12);

    game.spawnCreature(creatureAvatar);

    tibia::Game::creatureSharedPtr creatureWitch = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
    creatureWitch->setName("Evil Witch");
    creatureWitch->setTeam(tibia::Teams::evil);
    creatureWitch->setType(tibia::CreatureTypes::witch);
    creatureWitch->setPropertiesByType();
    creatureWitch->setHpMax(1000);
    creatureWitch->setHp(1000);
    creatureWitch->setCoords(64, 20);

    game.spawnCreature(creatureWitch);

    for (int i = 0; i < 10; i++)
    {
        tibia::Game::creatureSharedPtr creatureDemon = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
        creatureDemon->setName("Evil Demon");
        creatureDemon->setTeam(tibia::Teams::evil);
        creatureDemon->setType(tibia::CreatureTypes::demon);
        creatureDemon->setPropertiesByType();
        creatureDemon->setHpMax(2000);
        creatureDemon->setHp(2000);
        creatureDemon->setCoords(66, 20);

        game.spawnCreature(creatureDemon);
    }

    for (int i = 0; i < 25; i++)
    {
        std::stringstream creatureName;

        creatureName << "Zombie #" << i + 1;

        tibia::Game::creatureSharedPtr creatureZombie = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
        creatureZombie->setName(creatureName.str());
        creatureZombie->setTeam(tibia::Teams::evil);
        creatureZombie->setType(tibia::CreatureTypes::zombie);
        creatureZombie->setPropertiesByType();
        creatureZombie->setHpMax(50);
        creatureZombie->setHp(50);
        creatureZombie->setCoords(80, 19);

        game.spawnCreature(creatureZombie);

        creatureName.str("");

        creatureName << "Skeleton #" << i + 1;

        tibia::Game::creatureSharedPtr creatureSkeleton = std::make_shared<tibia::Creature>(0, 0, tibia::ZAxis::ground);
        creatureSkeleton->setName(creatureName.str());
        creatureSkeleton->setTeam(tibia::Teams::evil);
        creatureSkeleton->setType(tibia::CreatureTypes::skeleton);
        creatureSkeleton->setPropertiesByType();
        creatureSkeleton->setHpMax(25);
        creatureSkeleton->setHp(25);
        creatureSkeleton->setCoords(59, 20);

        game.spawnCreature(creatureSkeleton);
    }

    std::cout << "Loading objects" << std::endl;
    game.loadObjects();

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
            std::cout << "elapsed time:       " << elapsedTime.asSeconds() << std::endl;

            std::cout << "player x,y,z:       " << player->getX()     << "," << player->getY()     << "," << player->getZ() << std::endl;
            std::cout << "player tile x,y:    " << player->getTileX() << "," << player->getTileY()                          << std::endl;

            std::cout << "player tile number: " << player->getTileNumber() << std::endl;

            //std::vector<std::vector<int>>* groundTiles = game.getMap()->tileMapGroundTiles.getTilesArray();

            //int playerTileId = (*groundTiles)[player->getX()][player->getY()];

            //std::cout << "player tile id:     " << playerTileId << std::endl;

            //std::cout << "player is near water: " << game.isPlayerNearWater() << std::endl;

            //std:: cout << "view x,y center: " << gameView->getCenter().x - (tibia::TILE_SIZE / 2) << "," << gameView->getCenter().y - (tibia::TILE_SIZE / 2) << std::endl;

            std::cout << "creatures:          " << game.getCreaturesList().size()      << std::endl;
            std::cout << "animated decals:    " << game.getAnimatedDecalsList().size() << std::endl;

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

        game.updateObjects();

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
            game.spawnAnimation(game.getPlayer()->getTileX(), game.getPlayer()->getTileY(), game.getPlayer()->getZ(), tibia::Animations::spellBlue);

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
                            game.spawnAnimation(player->getTileX(), player->getTileY(), player->getZ(), tibia::Animations::spellBlue, 1.0);
                            break;

                        case sf::Keyboard::D:
                            game.spawnAnimatedDecal(player->getTileX(), player->getTileY(), player->getZ(), tibia::AnimatedDecals::poolRed, 30.0);
                            game.spawnAnimatedDecal(player->getTileX(), player->getTileY(), player->getZ(), tibia::AnimatedDecals::corpse,  30.0);
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

                    sf::Vector2f mouseTilePositionFloat = gameWindow->mapPixelToCoords(mouseWindowPosition, *gameView);

                    if (mouseTilePositionFloat.x < 0) mouseTilePositionFloat.x = 0;
                    if (mouseTilePositionFloat.y < 0) mouseTilePositionFloat.y = 0;

                    mouseTilePositionFloat.x -= tibia::GuiData::gameWindowX;
                    mouseTilePositionFloat.y -= tibia::GuiData::gameWindowY;

                    if
                    (
                        mouseTilePositionFloat.x < 0 ||
                        mouseTilePositionFloat.y < 0 ||
                        mouseTilePositionFloat.x > tibia::MAP_TILE_XY_MAX ||
                        mouseTilePositionFloat.y > tibia::MAP_TILE_XY_MAX
                    )
                    {
                        break;
                    }

                    sf::Vector2u mouseTilePosition;
                    mouseTilePosition.x = mouseTilePositionFloat.x;
                    mouseTilePosition.y = mouseTilePositionFloat.y;

                    while (mouseTilePosition.x > 0 && mouseTilePosition.x % tibia::TILE_SIZE != 0) mouseTilePosition.x--;
                    while (mouseTilePosition.y > 0 && mouseTilePosition.y % tibia::TILE_SIZE != 0) mouseTilePosition.y--;

                    std::cout << "mouse tile x,y: " << mouseTilePosition.x << "," << mouseTilePosition.y << std::endl;

                    int mouseTileNumber = tibia::getTileNumberByTileCoords(mouseTilePosition.x, mouseTilePosition.y);

                    std::cout << "mouse tile number: " << mouseTileNumber << std::endl;

                    if (mouseTileNumber < 0)
                    {
                        break;
                    }

                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Button::Left:
                        {
                            if (tibia::GuiData::gameWindowRect.contains(mouseWindowPosition))
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
                            }

                            break;
                        }

                        case sf::Mouse::Button::Right:
                        {
                            if (tibia::GuiData::gameWindowRect.contains(mouseWindowPosition))
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
                            }

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

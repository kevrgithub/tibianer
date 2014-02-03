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

#include "boost_foreach.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Game.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/Text.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/Sprite.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Player.hpp"

int windowWidth  = 640;
int windowHeight = 480;

float zoomFactor = 0.8;

void renderingThread(sf::RenderWindow* window, tibia::Game* game)
{
    window->setActive(true);

    sf::Clock* clockAnimatedWaterAndObjects = game->getClockAnimatedWaterAndObjects();

    while (window->isOpen())
    {
        sf::Time timeAnimatedWaterAndObjects = clockAnimatedWaterAndObjects->getElapsedTime();

        if (timeAnimatedWaterAndObjects.asSeconds() >= 1.0)
        {
            game->doAnimatedWater();
            game->doAnimatedObjects();

            clockAnimatedWaterAndObjects->restart();
        }

        window->clear(tibia::Colors::mainWindowColor);

        game->getPlayer()->update();
        game->updateCreatures();

        //game->showGameText("You died.", 32, sf::Color(255, 255, 255));

        game->drawGameWindow(window);

        window->display();
    }
}

int main()
{
    //sf::Vector2f vecA;
    //vecA.x = 0;
    //vecA.y = 0;

    //sf::Vector2f vecB;
    //vecB.x = windowWidth;
    //vecB.y = windowHeight;

    //sf::Vector2f vecDiff;
    //vecDiff = vecB - vecA;

    //vecDiff = vecDiff / thor::length(vecDiff);

    std::cout << "Initializing random number seed" << std::endl;
    std::srand(std::time(0));

    std::cout << "Creating main window" << std::endl;
    sf::RenderWindow mainWindow(sf::VideoMode(windowWidth, windowHeight), "Tibianer");
    mainWindow.setFramerateLimit(30);

    tibia::Game game;

    std::cout << "Loading font" << std::endl;
    if (game.loadFont(tibia::Fonts::default) == false)
    {
        std::cout << "Error: Failed to load font" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text loadingText("Loading...", *game.getFont(), tibia::Fonts::defaultSize);
    loadingText.setColor(sf::Color(255, 255, 255));
    loadingText.setPosition
    (
        (windowWidth  / 2) - (loadingText.getLocalBounds().width  / 2),
        (windowHeight / 2) - (loadingText.getLocalBounds().height / 2)
    );

    mainWindow.clear(sf::Color(0, 0, 0));
    mainWindow.draw(loadingText);
    mainWindow.display();

    std::cout << "Creating game window" << std::endl;
    if (game.createGameWindow() == false)
    {
        std::cout << "Error: Failed to create game window" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading textures" << std::endl;
    if (tibia::loadTextures() == false)
    {
        std::cout << "Error: Failed to load textures" << std::endl;
        return EXIT_FAILURE;
    }

    sf::SoundBuffer sb;
    sb.loadFromFile("sounds/everquest/snd2/button_1.wav");

    sf::Sound sound;
    sound.setBuffer(sb);
    //sound.play();

    std::cout << "Loading creatures" << std::endl;
    game.loadCreatures();
    game.spawnCreature();

    std::vector<std::shared_ptr<tibia::Creature>> creatures = game.getCreaturesList();
    std::vector<std::shared_ptr<tibia::Creature>>::iterator creatures_it;

    std::cout << "Loading animated objects" << std::endl;
    game.loadAnimatedObjects();

    //std::auto_ptr<tibia::Sprite> vecSprite2(new tibia::Sprite);
    //vecSprite2->setId(128);
    //vecSprite2->setPosition(32, 32);

    //tibia::Sprite vecSprite;
    //vecSprite.setId(128);
    //vecSprite.setPosition(0, 0);

    std::cout << "Loading map" << std::endl;
    if (game.loadMap("maps/tibia.xml") == false)
    {
        std::cout << "Error: Failed to load map" << std::endl;
        return EXIT_FAILURE;
    }

    //std::cout << "Creating threads" << std::endl;

    //sf::Thread threadAnimatedWater(&tibia::TileMap::doAnimatedWater, &map.tileMapGroundTiles);
    //sf::Thread threadAnimatedObjects(&tibia::Map::doAnimatedObjects, &map);

    //sf::Font myfont;
    //if (!myfont.loadFromFile("c:/windows/fonts/arial.ttf"))
        //return 1;

    //std::string temp_text = "Tibianer!";

    //sf::Text mytext(temp_text, myfont, 48);
    //mytext.setPosition(1024, 1024);

    //tibia::Sprite mysprite;
    //mysprite.setId(491);
    //mysprite.setPosition(0, 0);

    //std::string test_text = "Hello, world!\nThe quick brown Fox jumps over the lazy Dog.\nWhat?";

    //tibia::BitmapFontText bft;
    //bft.load(sf::Color(0, 255, 0), test_text, "images/font.png", sf::Vector2u(8, 8), false);
    //bft.setPosition(100, 100);

    std::cout << "Loading player" << std::endl;
    tibia::Player* player = game.getPlayer();
    player->setCoords(10, 10);
    //player->setPosition(64, 64);

    bool playerMovementReady = true;

    std::cout << "Loading view" << std::endl;
    sf::View* view = game.getWindowView();

    //sf::RenderTexture light_rt;
    //light_rt.create(tibia::MAP_SIZE * tibia::TILE_SIZE, tibia::MAP_SIZE * tibia::TILE_SIZE);

    //sf::RectangleShape light_rect;
    //light_rect.setPosition(0, 0);
    //light_rect.setSize(sf::Vector2f(tibia::MAP_SIZE * tibia::TILE_SIZE, tibia::MAP_SIZE * tibia::TILE_SIZE));

    std::cout << "Starting rendering loop" << std::endl;

    mainWindow.clear(sf::Color(0, 0, 0));
    mainWindow.setActive(false);

    sf::Thread threadStartRendering(std::bind(&renderingThread, &mainWindow, &game));
    threadStartRendering.launch();

    std::cout << "Starting main loop" << std::endl;

    sf::Clock* clock = game.getClock();

    sf::Clock clockOneSecond;
    clockOneSecond.restart();

    while (mainWindow.isOpen())
    {
        sf::Time elapsedTime = clock->restart();

        sf::Time timeOneSecond = clockOneSecond.getElapsedTime();

        if (timeOneSecond.asSeconds() >= 1.0)
        {
            //std::cout << "elapsed time: " << elapsedTime.asSeconds() << std::endl;

            std::cout << "player x,y,z: " << player->getX() << "," << player->getY() << "," << player->getZ() << std::endl;
            std::cout << "player tile x,y: " << player->getTileX() << "," << player->getTileY() << std::endl;

            //std::cout << "player->getMovementSpeed(): " << player->getMovementSpeed() << std::endl;

            std::cout << "player tile number: " << game.getCreatureTileNumber(player) << std::endl;

            clockOneSecond.restart();
        }

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta > 0)
                {
                    view->zoom(zoomFactor);
                }
                else if (event.mouseWheel.delta < 0)
                {
                    view->zoom(1.0 + (1.0 - zoomFactor));
                }
            }

            switch (event.type)
            {
                case sf::Event::KeyPressed:
                {
                    int direction = -1;

                    switch (event.key.code)
                    {
                        case sf::Keyboard::Up:
                            direction = tibia::Directions::up;

                            if (event.key.control == false)
                            {
                                if (game.checkCollision(player, direction) == false)
                                {
                                    player->doMove(direction);
                                }
                            }

                            player->doTurn(direction);
                            player->setIsSitting(game.checkIsSitting(player));
                            break;

                        case sf::Keyboard::Right:
                            direction = tibia::Directions::right;

                            if (event.key.control == false)
                            {
                                if (game.checkCollision(player, direction) == false)
                                {
                                    player->doMove(direction);
                                }
                            }

                            player->doTurn(direction);
                            player->setIsSitting(game.checkIsSitting(player));
                            break;

                        case sf::Keyboard::Down:
                            direction = tibia::Directions::down;

                            if (event.key.control == false)
                            {
                                if (game.checkCollision(player, direction) == false)
                                {
                                    player->doMove(direction);
                                }
                            }

                            player->doTurn(direction);
                            player->setIsSitting(game.checkIsSitting(player));
                            break;

                        case sf::Keyboard::Left:
                            direction = tibia::Directions::left;

                            if (event.key.control == false)
                            {
                                if (game.checkCollision(player, direction) == false)
                                {
                                    player->doMove(direction);
                                }
                            }

                            player->doTurn(direction);
                            player->setIsSitting(game.checkIsSitting(player));
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
                            game.showGameText("You say:\nHello!", tibia::Fonts::gameSize, tibia::Colors::yellow);
                            break;

                        case sf::Keyboard::C:
                            for (creatures_it = creatures.begin(); creatures_it != creatures.end(); creatures_it++)
                            {
                                std::cout  << "name: " << (*creatures_it)->getName() << std::endl;

                                (*creatures_it)->setOutfitRandom();

                                int playerX = player->getX();
                                int playerY = player->getY();

                                int creatureX = (*creatures_it)->getX();
                                int creatureY = (*creatures_it)->getY();

                                std::cout << playerX << "," << playerY << " | " << creatureX << "," << creatureY <<  std::endl;

                                float distance = tibia::calculateDistance(playerX, playerY, creatureX, creatureY);

                                //distance /= tibia::TILE_SIZE;

                                std::cout << "distance: " << distance << std::endl;

                                float volume = 100 - (distance * 8);

                                if (volume < 0) volume = 0;

                                std::cout << "volume: " << volume << std::endl;

                                sound.setVolume(volume);

                                sound.play();
                            }
                            break;

                        case sf::Keyboard::V:
                            std::vector<int> visibleTileNumbers = game.getVisibleTileNumbers();
                            for (int i = 0; i < visibleTileNumbers.size(); i++)
                            {
                                if (visibleTileNumbers.at(i) < 1000)
                                    std::cout << " ";

                                std::cout << visibleTileNumbers.at(i) << " ";

                                if (i > 0 && (i + 1) % 13 == 0)
                                    std::cout << std::endl;
                            }
                            std::cout << std::endl;
                            std::cout << visibleTileNumbers.size() << std::endl;
                            break;
                    }
                }
            }

            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }
        }

        //view->setCenter(player->getTileX() + (tibia::TILE_SIZE / 2), player->getTileY() + (tibia::TILE_SIZE / 2));

        //sf::RectangleShape light_squares[3];

        //light_squares[0].setSize(sf::Vector2f(32, 32));
        //light_squares[0].setFillColor(sf::Color(255, 255, 255, 128));
        //light_squares[0].setPosition(player.tileX, player.tileY);

        //light_squares[1].setSize(sf::Vector2f(96, 96));
        //light_squares[1].setFillColor(sf::Color(255, 255, 255, 160));
        //light_squares[1].setPosition(player.tileX - tibia::TILE_SIZE, player.tileY - tibia::TILE_SIZE);

        //light_squares[2].setSize(sf::Vector2f(160, 160));
        //light_squares[2].setFillColor(sf::Color(255, 255, 255, 192));
        //light_squares[2].setPosition(player.tileX - (tibia::TILE_SIZE * 2), player.tileY - (tibia::TILE_SIZE * 2));

        //sf::CircleShape light_circle;
        //light_circle.setRadius(96);
        //light_circle.setPointCount(16);
        //light_circle.setFillColor(sf::Color(255, 255, 255, 128));
        //light_circle.setPosition(10 * tibia::TILE_SIZE, 10 * tibia::TILE_SIZE);

        //light_rt.clear(sf::Color(0, 0, 0, 192));

        //for (unsigned int ls = 0; ls < 3; ls++)
        //{
            //light_rt.draw(light_squares[ls],  sf::BlendMode::BlendMultiply);
        //}

        //light_rt.draw(light_circle,  sf::BlendMode::BlendMultiply);

        //light_rt.display();

        //light_rect.setTexture(&light_rt.getTexture());


        //gameWindow.setView(game.getWindowView());
        //gameWindow.clear(sf::Color(0, 0, 0));
        //gameWindow.draw(map.tileMapGroundTiles);
        //gameWindow.draw(map.tileMapGroundEdges);
        //gameWindow.draw(map.tileMapGroundWalls);
        //gameWindow.draw(map.tileMapGroundWallsEx);
        //gameWindow.draw(map.tileMapGroundWalls2);
        //gameWindow.draw(map.tileMapGroundObjects);
        //gameWindow.draw(map.tileMapGroundObjects2);
        //gameWindow.draw(mytext);
        //gameWindow.draw(mysprite);
        //gameWindow.draw(bft);
        //gameWindow.draw(player);

        //gameWindow.draw(map.tileMapGroundObjectsDrawLast);
        //gameWindow.draw(map.tileMapGroundObjectsDrawLast2);

        //std::vector<int> objectsDrawLastTiles = map.tileMapGroundObjectsDrawLast.getTiles();

        //int objectsDrawLastTileNumber = map.tileMapGroundObjectsDrawLast.getTileNumberByTileCoords(player.getTileX() - 1, player.getTileY() - 1);

        //if (objectsDrawLastTiles.at(objectsDrawLastTileNumber) != tibia::TILE_NULL)
        //{
            //std::cout << "Drawing player again" << std::endl;

            //gameWindow.draw(player);
        //}

        //std::vector<int> groundTiles = map.tileMapGroundTiles.getTiles();
        //int playerTileNumber = map.tileMapGroundTiles.getTileNumberByTileCoords(player.tileX, player.tileY);
        //std::cout << "player is in tile: " << groundTiles.at(playerTileNumber) << std::endl;

        //gameWindow.draw(light_rect);
        //gameWindow.display();
    }

    return EXIT_SUCCESS;
}

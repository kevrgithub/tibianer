#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>


#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/Sprite.hpp"
#include "tibia/Player.hpp"

int windowWidth  = 640;
int windowHeight = 480;

int main()
{
    std::srand(std::time(0));

    if (tibia::loadSprites() == false)
    {
        return EXIT_FAILURE;
    }

    tibia::loadAnimatedObjects();

    // 13x9 tiles

    sf::RenderWindow mywindow(sf::VideoMode(windowWidth, windowHeight), "Tibianer");

    sf::View myview(sf::FloatRect(0, 0, windowWidth, windowHeight));
    myview.setCenter(sf::Vector2f(0, 0));

    float zoomFactor = 0.8;

    tibia::Map map;
    map.load("maps/tibia.xml");

    //sf::Font myfont;
    //if (!myfont.loadFromFile("c:/windows/fonts/arial.ttf"))
        //return 1;

    //std::string temp_text = "Tibianer!";

    //sf::Text mytext(temp_text, myfont, 48);
    //mytext.setPosition(1024, 1024);

    //tibia::Sprite mysprite;
    //mysprite.setId(491);
    //mysprite.setPosition(0, 0);

    std::string test_text = "Hello, world!\nThe quick brown Fox jumps over the lazy Dog.\nWhat?";

    tibia::BitmapFontText bft;
    bft.load(sf::Color(0, 255, 0), test_text, "images/font.png", sf::Vector2u(8, 8), false);
    bft.setPosition(100, 100);

    tibia::Player player;
    player.setCoords(10, 10);
    //player.setPosition(64, 64);

    //sf::RenderTexture light_rt;
    //light_rt.create(tibia::MAP_SIZE * tibia::TILE_SIZE, tibia::MAP_SIZE * tibia::TILE_SIZE);

    //sf::RectangleShape light_rect;
    //light_rect.setPosition(0, 0);
    //light_rect.setSize(sf::Vector2f(tibia::MAP_SIZE * tibia::TILE_SIZE, tibia::MAP_SIZE * tibia::TILE_SIZE));

    sf::Clock myClock;

    sf::Clock clockOneSecond;
    clockOneSecond.restart();

    while (mywindow.isOpen())
    {
        sf::Time elapsedTime = myClock.restart();

        sf::Time timeOneSecond = clockOneSecond.getElapsedTime();

        if (timeOneSecond.asSeconds() >= 1.0)
        {
            map.tileMapGroundTiles.doAnimatedWater();
            std::cout << "doAnimatedWater()" << std::endl;

            map.doAnimatedObjects();
            std::cout << "doAnimatedObjects()" << std::endl;

            clockOneSecond.restart();
        }

        sf::Event myevent;
        while (mywindow.pollEvent(myevent))
        {
            if (myevent.type == sf::Event::MouseWheelMoved)
            {
                if (myevent.mouseWheel.delta > 0)
                {
                    myview.zoom(zoomFactor);
                }
                else if (myevent.mouseWheel.delta < 0)
                {
                    myview.zoom(1.0 + (1.0 - zoomFactor));
                }
            }

            switch(myevent.type)
            {
                case sf::Event::KeyPressed:
                {
                    switch(myevent.key.code)
                    {
                        case sf::Keyboard::Up:
                            //myview.move(0, -32);
                            if (myevent.key.control == false)
                                player.setY(player.getY() - 1);
                            player.setDirection(0);
                            break;

                        case sf::Keyboard::Down:
                            //myview.move(0, 32);
                            if (myevent.key.control == false)
                                player.setY(player.getY() + 1);
                            player.setDirection(2);
                            break;

                        case sf::Keyboard::Left:
                            //myview.move(-32, 0);
                            if (myevent.key.control == false)
                                player.setX(player.getX() - 1);
                            player.setDirection(3);
                            break;

                        case sf::Keyboard::Right:
                            //myview.move(32, 0);
                            if (myevent.key.control == false)
                                player.setX(player.getX() + 1);
                            player.setDirection(1);
                            break;

                        case sf::Keyboard::O:
                            player.setOutfitRandom();
                            break;
                    }
                }
            }

            if (myevent.type == sf::Event::Closed)
            {
                mywindow.close();
            }
        }

        player.update();

        myview.setCenter(player.getDrawX() + (tibia::TILE_SIZE / 2), player.getDrawY() + (tibia::TILE_SIZE / 2));

        //sf::RectangleShape light_squares[3];

        //light_squares[0].setSize(sf::Vector2f(32, 32));
        //light_squares[0].setFillColor(sf::Color(255, 255, 255, 128));
        //light_squares[0].setPosition(player.drawX, player.drawY);

        //light_squares[1].setSize(sf::Vector2f(96, 96));
        //light_squares[1].setFillColor(sf::Color(255, 255, 255, 160));
        //light_squares[1].setPosition(player.drawX - tibia::TILE_SIZE, player.drawY - tibia::TILE_SIZE);

        //light_squares[2].setSize(sf::Vector2f(160, 160));
        //light_squares[2].setFillColor(sf::Color(255, 255, 255, 192));
        //light_squares[2].setPosition(player.drawX - (tibia::TILE_SIZE * 2), player.drawY - (tibia::TILE_SIZE * 2));

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

        mywindow.setView(myview);
        mywindow.clear(sf::Color(0, 0, 0, 255));
        mywindow.draw(map.tileMapGroundTiles);
        mywindow.draw(map.tileMapGroundEdges);
        mywindow.draw(map.tileMapGroundWalls);
        mywindow.draw(map.tileMapGroundWallsEx);
        mywindow.draw(map.tileMapGroundWalls2);
        mywindow.draw(map.tileMapGroundObjects);
        mywindow.draw(map.tileMapGroundObjects2);
        //mywindow.draw(mytext);
        //mywindow.draw(mysprite);
        //mywindow.draw(bft);
        mywindow.draw(player);

        mywindow.draw(map.tileMapGroundObjectsDrawLast);
        mywindow.draw(map.tileMapGroundObjectsDrawLast2);

        std::vector<int> checkTiles = map.tileMapGroundObjectsDrawLast.getTiles();
        //for (int j = 0; j < checkTiles.size(); j++)
        //{
            //if (checkTiles.at(j) != 0)
            //{
                //std::cout << checkTiles.at(j) << " " << std::endl;
            //}
        //}

        int drawLastTileNumber = map.tileMapGroundObjectsDrawLast.getTileNumberByTileCoords(player.getDrawX() - 1, player.getDrawY() - 1);

        //std::cout << "top left draw last is: " << checkTiles.at(drawLastTileNumber) << std::endl;

        if (checkTiles.at(drawLastTileNumber) != 0)
        {
            std::cout << "drawing player again" << std::endl;
            mywindow.draw(player);
        }

        //std::vector<int> groundTiles = map.tileMapGroundTiles.getTiles();
        //int playerTileNumber = map.tileMapGroundTiles.getTileNumberByTileCoords(player.drawX, player.drawY);
        //std::cout << "player is in tile: " << groundTiles.at(playerTileNumber) << std::endl;

        //mywindow.draw(light_rect);
        mywindow.display();
    }

    return EXIT_SUCCESS;
}

#ifndef TIBIA_GAME_HPP
#define TIBIA_GAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <boost/algorithm/string.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Player.hpp"

namespace tibia
{

class Game
{

public:

    Game()
    :
        m_windowView(sf::FloatRect(0, 0, tibia::TILES_WIDTH, tibia::TILES_HEIGHT)),
        m_threadAnimatedWater(&tibia::TileMap::doAnimatedWater, &m_map.tileMapGroundTiles),
        m_threadAnimatedObjects(&tibia::Map::doAnimatedObjects, &m_map)
    {
        m_clock.restart();
        m_clockAnimatedWaterAndObjects.restart();

        m_clockText.restart();

        m_rtLight.create(tibia::MAP_XY_MAX, tibia::MAP_XY_MAX);
        m_rtLight.setSmooth(true);

        m_rectLight.setPosition(0, 0);
        m_rectLight.setSize(sf::Vector2f(tibia::MAP_XY_MAX, tibia::MAP_XY_MAX));
    }

    bool createGameWindow()
    {
        if (m_window.create(tibia::TILES_WIDTH, tibia::TILES_HEIGHT) == false)
        {
            return false;
        }

        m_windowBorder.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
        m_windowBorder.setOutlineColor(tibia::Colors::windowBorderColor);
        m_windowBorder.setOutlineThickness(1);
        m_windowBorder.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        return true;
    }

    bool loadFont(std::string filename)
    {
        return m_font.loadFromFile(filename);
    }

    bool loadMap(std::string filename)
    {
        return m_map.load(filename);
    }

    void loadCreatures()
    {
        //
    }

    void drawPlayer()
    {
        m_window.draw(m_player);
    }

    void drawCreatures()
    {
        if (m_creaturesList.size() == 0)
        {
            return;
        }

        for (m_creaturesList_it = m_creaturesList.begin(); m_creaturesList_it != m_creaturesList.end(); m_creaturesList_it++)
        {
            if (m_player.getZ() == tibia::ZAxis::underGround && (*m_creaturesList_it)->getZ() != tibia::ZAxis::underGround)
            {
                continue;
            }

            m_window.draw(**m_creaturesList_it);
        }
    }

    void updateCreatures()
    {
        for (m_creaturesList_it = m_creaturesList.begin(); m_creaturesList_it != m_creaturesList.end(); m_creaturesList_it++)
        {
            (*m_creaturesList_it)->update();
        }
    }

    void spawnCreature()
    {
        std::shared_ptr<tibia::Creature> c(new tibia::Creature);
        c->setName("Cipfried");
        c->setHasOutfit(true);
        c->setOutfitRandom();
        c->setCoords(11, 8);

        m_creaturesList.push_back(c);
    }

    std::vector<std::shared_ptr<tibia::Creature>> getCreaturesList()
    {
        return m_creaturesList;
    }

    void loadAnimatedObjects()
    {
        std::vector<int> animatedObject;

        // blue portal
        animatedObject.clear();
        animatedObject.push_back(197);
        animatedObject.push_back(198);
        animatedObject.push_back(199);
        animatedObjectsList.push_back(animatedObject);

        // fires
        animatedObject.clear();
        animatedObject.push_back(1489);
        animatedObject.push_back(1490);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(1491);
        animatedObject.push_back(1492);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(1493);
        animatedObject.push_back(1494);
        animatedObjectsList.push_back(animatedObject);

        // electricity
        animatedObject.clear();
        animatedObject.push_back(1497);
        animatedObject.push_back(1498);
        animatedObjectsList.push_back(animatedObject);

        // magic wall
        animatedObject.clear();
        animatedObject.push_back(3268);
        animatedObject.push_back(3272);
        animatedObject.push_back(3276);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(3269);
        animatedObject.push_back(3273);
        animatedObject.push_back(3277);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(3270);
        animatedObject.push_back(3274);
        animatedObject.push_back(3278);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(3271);
        animatedObject.push_back(3275);
        animatedObject.push_back(3279);
        animatedObjectsList.push_back(animatedObject);

        // fountain
        animatedObject.clear();
        animatedObject.push_back(202);
        animatedObject.push_back(367);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(203);
        animatedObject.push_back(368);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(204);
        animatedObject.push_back(369);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(205);
        animatedObject.push_back(370);
        animatedObjectsList.push_back(animatedObject);

        // wall torches
        animatedObject.clear();
        animatedObject.push_back(2968);
        animatedObject.push_back(2970);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2974);
        animatedObject.push_back(2975);
        animatedObjectsList.push_back(animatedObject);

        // candle
        animatedObject.clear();
        animatedObject.push_back(395);
        animatedObject.push_back(396);
        animatedObjectsList.push_back(animatedObject);

        // torches
        animatedObject.clear();
        animatedObject.push_back(443);
        animatedObject.push_back(444);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(446);
        animatedObject.push_back(447);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(449);
        animatedObject.push_back(450);
        animatedObjectsList.push_back(animatedObject);

        // water mill
        animatedObject.clear();
        animatedObject.push_back(890);
        animatedObject.push_back(896);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(891);
        animatedObject.push_back(897);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(892);
        animatedObject.push_back(898);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(893);
        animatedObject.push_back(899);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(894);
        animatedObject.push_back(900);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(895);
        animatedObject.push_back(901);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(902);
        animatedObject.push_back(911);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(903);
        animatedObject.push_back(912);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(904);
        animatedObject.push_back(913);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(905);
        animatedObject.push_back(914);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(906);
        animatedObject.push_back(915);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(907);
        animatedObject.push_back(916);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(908);
        animatedObject.push_back(917);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(909);
        animatedObject.push_back(918);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(910);
        animatedObject.push_back(919);
        animatedObjectsList.push_back(animatedObject);

        // purple wand
        animatedObject.clear();
        animatedObject.push_back(1123);
        animatedObject.push_back(1124);
        animatedObjectsList.push_back(animatedObject);

        // oven
        animatedObject.clear();
        animatedObject.push_back(1153);
        animatedObject.push_back(1155);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(1154);
        animatedObject.push_back(1156);
        animatedObjectsList.push_back(animatedObject);

        // slime bubble
        animatedObject.clear();
        animatedObject.push_back(1570);
        animatedObject.push_back(1571);
        animatedObject.push_back(1572);
        animatedObject.push_back(1573);
        animatedObject.push_back(1574);
        animatedObjectsList.push_back(animatedObject);

        // lava bubble
        animatedObject.clear();
        animatedObject.push_back(1582);
        animatedObject.push_back(1583);
        animatedObject.push_back(1584);
        animatedObject.push_back(1585);
        animatedObject.push_back(1586);
        animatedObjectsList.push_back(animatedObject);

        // magic tree
        animatedObject.clear();
        animatedObject.push_back(2124);
        animatedObject.push_back(2128);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2125);
        animatedObject.push_back(2129);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2126);
        animatedObject.push_back(2130);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2127);
        animatedObject.push_back(2131);
        animatedObjectsList.push_back(animatedObject);

        // basins
        animatedObject.clear();
        animatedObject.push_back(2664);
        animatedObject.push_back(2665);
        animatedObject.push_back(2666);
        animatedObject.push_back(2667);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2668);
        animatedObject.push_back(2669);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2670);
        animatedObject.push_back(2671);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2672);
        animatedObject.push_back(2673);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2675);
        animatedObject.push_back(2676);
        animatedObject.push_back(2677);
        animatedObject.push_back(2678);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2679);
        animatedObject.push_back(2680);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2681);
        animatedObject.push_back(2682);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2683);
        animatedObject.push_back(2684);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2686);
        animatedObject.push_back(2687);
        animatedObject.push_back(2688);
        animatedObject.push_back(2689);
        animatedObject.push_back(2690);
        animatedObject.push_back(2691);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2693);
        animatedObject.push_back(2694);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2695);
        animatedObject.push_back(2696);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(2697);
        animatedObject.push_back(2698);
        animatedObjectsList.push_back(animatedObject);

        // fire hole
        animatedObject.clear();
        animatedObject.push_back(3178);
        animatedObject.push_back(3179);
        animatedObject.push_back(3180);
        animatedObject.push_back(3181);
        animatedObject.push_back(3182);
        animatedObject.push_back(3183);
        animatedObjectsList.push_back(animatedObject);
    }

    void doAnimatedWater()
    {
        m_threadAnimatedWater.launch();
    }

    void doAnimatedObjects()
    {
        m_threadAnimatedObjects.launch();
    }

    void showGameText(std::string text, unsigned int textSize, sf::Color textColor)
    {
        m_textList.clear();

        std::vector<std::string> textTokens;

        boost::split(textTokens, text, boost::is_any_of("\n"));

        foreach (std::string textToken, textTokens)
        {
            sf::Text text;
            text.setString(textToken);
            text.setFont(m_font);
            text.setCharacterSize(textSize);
            text.setColor(textColor);

            //sf::FloatRect textRect = text.getLocalBounds();

            //text.setOrigin
            //(
                //textRect.left + textRect.width  / 2,
                //textRect.top  + textRect.height / 2
            //);

            text.setPosition
            (
                (m_player.getTileX() + tibia::TILE_DRAW_OFFSET) - (text.getLocalBounds().width / 2),
                (m_player.getTileY() - (tibia::TILE_SIZE * 2))
            );

            m_textList.push_back(text);
        }

        m_clockText.restart();
    }

    void drawGameText()
    {
        sf::Time timeText = m_clockText.getElapsedTime();

        if (timeText.asSeconds() < 5.0)
        {
            int textPositionOffsetY = 0;

            foreach (sf::Text text, m_textList)
            {
                //std::string textString = text.getString();
                //std::cout << textString << std::endl;

                text.setPosition(text.getPosition().x, text.getPosition().y + textPositionOffsetY);

                sf::Text textShadow = text;

                textShadow.setColor(tibia::Colors::gameTextShadowColor);

                textShadow.setPosition(text.getPosition().x + 1, text.getPosition().y + 1);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x - 1, text.getPosition().y - 1);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x + 1, text.getPosition().y);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x - 1, text.getPosition().y);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x, text.getPosition().y + 1);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x, text.getPosition().y - 1);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x + 1, text.getPosition().y - 1);
                m_window.draw(textShadow);

                textShadow.setPosition(text.getPosition().x - 1, text.getPosition().y + 1);
                m_window.draw(textShadow);

                m_window.draw(text);

                textPositionOffsetY += tibia::TILE_SIZE;
            }
        }
    }

    void drawGameWindow(sf::RenderWindow* mainWindow)
    {
        m_windowView.setCenter(m_player.getTileX() + (tibia::TILE_SIZE / 2), m_player.getTileY() + (tibia::TILE_SIZE / 2));

        m_window.setView(m_windowView);
        m_window.clear(tibia::Colors::black);

        int playerX = m_player.getTileX();
        int playerY = m_player.getTileY();
        int playerZ = m_player.getZ();

        if (playerZ == tibia::ZAxis::underGround)
        {
            m_window.draw(m_map.tileMapUnderGroundTiles);
            m_window.draw(m_map.tileMapUnderGroundEdges);
            m_window.draw(m_map.tileMapUnderGroundWalls);
            m_window.draw(m_map.tileMapUnderGroundWallsEx);
            m_window.draw(m_map.tileMapUnderGroundWalls2);
            m_window.draw(m_map.tileMapUnderGroundObjects);
            m_window.draw(m_map.tileMapUnderGroundObjects2);
            m_window.draw(m_map.tileMapUnderGroundObjectsOffset);
        }
        else
        {
            m_window.draw(m_map.tileMapGroundTiles);
            m_window.draw(m_map.tileMapGroundEdges);
            m_window.draw(m_map.tileMapGroundWalls);
            m_window.draw(m_map.tileMapGroundWallsEx);
            m_window.draw(m_map.tileMapGroundWalls2);
            m_window.draw(m_map.tileMapGroundObjects);
            m_window.draw(m_map.tileMapGroundObjects2);
            m_window.draw(m_map.tileMapGroundObjectsOffset);
        }

        drawPlayer();
        drawCreatures();

        if (playerZ == tibia::ZAxis::underGround)
        {
            m_window.draw(m_map.tileMapUnderGroundObjectsDrawLast);
            m_window.draw(m_map.tileMapUnderGroundObjectsDrawLast2);
        }
        else
        {
            m_window.draw(m_map.tileMapGroundObjectsDrawLast);
            m_window.draw(m_map.tileMapGroundObjectsDrawLast2);
        }

        if (playerZ != tibia::ZAxis::underGround)
        {
            bool drawAboveGround = true;

            if (playerZ == tibia::ZAxis::ground)
            {
                std::vector<int> aboveGroundTiles = m_map.tileMapAboveGroundTiles.getTiles();

                std::vector<int> aboveGroundTileNumbers;
                std::vector<int>::iterator aboveGroundTileNumbers_it;

                for (int i = -2; i < 3; i++)
                {
                    aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX - (2 * tibia::TILE_SIZE), playerY + (i * tibia::TILE_SIZE)));
                    aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX - (1 * tibia::TILE_SIZE), playerY + (i * tibia::TILE_SIZE)));
                    aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX                         , playerY + (i * tibia::TILE_SIZE)));
                    aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX + (1 * tibia::TILE_SIZE), playerY + (i * tibia::TILE_SIZE)));
                    aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX + (2 * tibia::TILE_SIZE), playerY + (i * tibia::TILE_SIZE)));
                }

                //aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX, playerY));
                //aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX - (2 * tibia::TILE_SIZE), playerY - (2 * tibia::TILE_SIZE)));
                //aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX - (1 * tibia::TILE_SIZE), playerY - (2 * tibia::TILE_SIZE)));
                //aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX                         , playerY - (2 * tibia::TILE_SIZE)));
                //aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX + (1 * tibia::TILE_SIZE), playerY - (2 * tibia::TILE_SIZE)));
                //aboveGroundTileNumbers.push_back(tibia::getTileNumberByTileCoords(playerX + (2 * tibia::TILE_SIZE), playerY - (2 * tibia::TILE_SIZE)));

                for (aboveGroundTileNumbers_it = aboveGroundTileNumbers.begin(); aboveGroundTileNumbers_it != aboveGroundTileNumbers.end(); aboveGroundTileNumbers_it++)
                {
                    if (*aboveGroundTileNumbers_it < 0 || *aboveGroundTileNumbers_it > tibia::TILE_NUMBER_MAX)
                    {
                        continue;
                    }

                    if (aboveGroundTiles.at(*aboveGroundTileNumbers_it) != tibia::TILE_NULL)
                    {
                        drawAboveGround = false;
                        break;
                    }
                }
            }

            if (drawAboveGround == true)
            {
                m_window.draw(m_map.tileMapAboveGroundTiles);
                m_window.draw(m_map.tileMapAboveGroundEdges);
                m_window.draw(m_map.tileMapAboveGroundWalls);
                m_window.draw(m_map.tileMapAboveGroundWallsEx);
                m_window.draw(m_map.tileMapAboveGroundWalls2);
                m_window.draw(m_map.tileMapAboveGroundObjects);
                m_window.draw(m_map.tileMapAboveGroundObjects2);
                m_window.draw(m_map.tileMapAboveGroundObjectsOffset);

                if (playerZ == tibia::ZAxis::aboveGround)
                {
                    m_window.draw(m_player);
                }

                m_window.draw(m_map.tileMapAboveGroundObjectsDrawLast);
                m_window.draw(m_map.tileMapAboveGroundObjectsDrawLast2);
            }
        }

        std::vector<int> objectsDrawLastTiles;

        switch (playerZ)
        {
            case tibia::ZAxis::underGround:
                objectsDrawLastTiles = m_map.tileMapUnderGroundObjectsDrawLast.getTiles();
                break;

            case tibia::ZAxis::ground:
                objectsDrawLastTiles = m_map.tileMapGroundObjectsDrawLast.getTiles();
                break;

            case tibia::ZAxis::aboveGround:
                objectsDrawLastTiles = m_map.tileMapAboveGroundObjectsDrawLast.getTiles();
                break;
        }

        if (objectsDrawLastTiles.size())
        {
            int objectsDrawLastTileNumberA = tibia::getTileNumberByTileCoords(playerX - 1, playerY - 1);
            int objectsDrawLastTileNumberB = tibia::getTileNumberByTileCoords(playerX,     playerY);

            if (objectsDrawLastTiles.at(objectsDrawLastTileNumberA) != tibia::TILE_NULL)
            {
                if (objectsDrawLastTiles.at(objectsDrawLastTileNumberB) == tibia::TILE_NULL)
                {
                    //std::cout << "Drawing player again" << std::endl;

                    drawPlayer();
                }
            }
        }

        //std::vector<int> groundTiles = m_map.tileMapGroundTiles.getTiles();
        //int playerTileNumber = m_map.tileMapGroundTiles.getTileNumberByTileCoords(m_player.getTileX(), m_player.getTileY());
        //std::cout << "player is in tile: " << groundTiles.at(playerTileNumber) << std::endl;

        if (playerZ == tibia::ZAxis::underGround)
        {
            sf::Sprite spritePlayerLight;
            spritePlayerLight.setTexture(tibia::Textures::light3);
            spritePlayerLight.setOrigin(spritePlayerLight.getLocalBounds().width / 2, spritePlayerLight.getLocalBounds().height / 2);
            spritePlayerLight.setPosition(playerX + (tibia::TILE_SIZE / 2), playerY + (tibia::TILE_SIZE / 2));
            spritePlayerLight.setColor(sf::Color(255, 255, 255, 192));

            m_rtLight.clear(sf::Color(0, 0, 0, 128));

            m_rtLight.draw(spritePlayerLight, sf::BlendMode::BlendAdd);

            m_rtLight.display();

            m_rectLight.setTexture(&m_rtLight.getTexture());

            m_window.draw(m_rectLight, sf::BlendMode::BlendMultiply);
        }

        drawGameText();

        m_window.display();

        m_windowSprite.setTexture(m_window.getTexture());
        m_windowSprite.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        mainWindow->draw(m_windowBorder);
        mainWindow->draw(m_windowSprite);
    }

    bool checkIsSitting(tibia::Creature* creature)
    {
        bool sitting = false;

        creature->updateTileCoords();

        for (m_map.tileMapList_it = m_map.tileMapList.begin(); m_map.tileMapList_it != m_map.tileMapList.end(); m_map.tileMapList_it++)
        {
            if ((*m_map.tileMapList_it)->getZ() != creature->getZ())
            {
                continue;
            }

            if ((*m_map.tileMapList_it)->getType() != "objects")
            {
                continue;
            }

            std::vector<int> tiles = (*m_map.tileMapList_it)->getTiles();

            if (tiles.size() == 0)
            {
                continue;
            }

            int tileNumber = tibia::getTileNumberByTileCoords(creature->getTileX(), creature->getTileY());

            int tileId = tiles.at(tileNumber);

            if (tileId == tibia::TILE_NULL)
            {
                continue;
            }

            for (int i = 0; i < (sizeof(tibia::SpriteData::chairs) / sizeof(int)); i++)
            {
                if (tiles.at(tileNumber) == tibia::SpriteData::chairs[i])
                {
                    sitting = true;
                    break;
                }
            }
        }

        return sitting;
    }

    bool checkCollision(tibia::Creature* creature, int direction)
    {
        bool collision = false;

        sf::Vector2u checkCoords;

        switch (direction)
        {
            case tibia::Directions::up:
                if (creature->getY() == 0)
                {
                    return true;
                }
                checkCoords.x = creature->getTileX();
                checkCoords.y = creature->getTileY() - tibia::TILE_SIZE;
                break;

            case tibia::Directions::right:
                if (creature->getX() == tibia::MAP_SIZE - 1)
                {
                    return true;
                }
                checkCoords.x = creature->getTileX() + tibia::TILE_SIZE;
                checkCoords.y = creature->getTileY();
                break;

            case tibia::Directions::down:
                if (creature->getY() == tibia::MAP_SIZE - 1)
                {
                    return true;
                }
                checkCoords.x = creature->getTileX();
                checkCoords.y = creature->getTileY() + tibia::TILE_SIZE;
                break;

            case tibia::Directions::left:
                if (creature->getX() == 0)
                {
                    return true;
                }
                checkCoords.x = creature->getTileX() - tibia::TILE_SIZE;
                checkCoords.y = creature->getTileY();
                break;
        }

        //std::cout << "checkCoords x,y: " << checkCoords.x << "," << checkCoords.y << std::endl;

        for (m_map.tileMapList_it = m_map.tileMapList.begin(); m_map.tileMapList_it != m_map.tileMapList.end(); m_map.tileMapList_it++)
        {
            if ((*m_map.tileMapList_it)->getZ() != creature->getZ())
            {
                continue;
            }

            if ((*m_map.tileMapList_it)->getType() == "edges")
            {
                continue;
            }

            if ((*m_map.tileMapList_it)->getType() == "objects draw last")
            {
                continue;
            }

            std::vector<int> tiles = (*m_map.tileMapList_it)->getTiles();

            if (tiles.size() == 0)
            {
                continue;
            }

            int tileNumber = tibia::getTileNumberByTileCoords(checkCoords.x, checkCoords.y);

            if (tileNumber > tibia::TILE_NUMBER_MAX)
            {
                continue;
            }

            int tileId = tiles.at(tileNumber);

            if (tileId == tibia::TILE_NULL)
            {
                if ((*m_map.tileMapList_it)->getType() == "tiles")
                {
                    if (m_player.getZ() == tibia::ZAxis::aboveGround)
                    {
                        return true;
                    }
                }

                continue;
            }

            //std::cout << "check for collision at tile number: " << tileNumber << std::endl;

            //std::cout << "check for collision at tile id:     " << tiles.at(tileNumber) << std::endl;

            for (int i = 0; i < (sizeof(tibia::SpriteData::solid) / sizeof(int)); i++)
            {
                //std::cout << "tibia::SpriteData::solid[i]: " << tibia::SpriteData::solid[i] << std::endl;

                if (tileId == tibia::SpriteData::solid[i])
                {
                    //std::cout << "collision occured at tile id: " << tiles.at(tileNumber) << std::endl;

                    collision = true;
                    break;
                }
            }
        }

        for (m_creaturesList_it = m_creaturesList.begin(); m_creaturesList_it != m_creaturesList.end(); m_creaturesList_it++)
        {
            if ((*m_creaturesList_it)->getZ() == m_player.getZ())
            {
                if ((*m_creaturesList_it)->getTileX() == checkCoords.x && (*m_creaturesList_it)->getTileY() == checkCoords.y)
                {
                    collision = true;
                    break;
                }
            }
        }

        return collision;
    }

    int getCreatureTileNumber(tibia::Creature* creature)
    {
        return tibia::getTileNumberByTileCoords(creature->getTileX(), creature->getTileY());
    }

    std::vector<int> getVisibleTileNumbers()
    {
        std::vector<int> visibleTileNumbers;

        int playerTileNumber = getCreatureTileNumber(&m_player);

        int playerTileOffset = (tibia::MAP_SIZE * 4) + 6;

        int firstTileNumber = playerTileNumber - playerTileOffset;
        int lastTileNumber  = playerTileNumber + playerTileOffset;

        // offscreen tiles
        //firstTileNumber -= (tibia::MAP_SIZE + 1);
        //lastTileNumber  += (tibia::MAP_SIZE + 1);

        std::cout << "firstTileNumber: " << firstTileNumber << std::endl;
        std::cout << "lastTileNumber: "  << lastTileNumber  << std::endl;

        int j = 0;

        for (int i = firstTileNumber; i < lastTileNumber + 1; i++)
        {
            if (j == tibia::NUM_TILES_X)
            {
                i += tibia::MAP_SIZE - j;
                j = 0;
            }

            int tileNumber = i;

            if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
            {
                j++;
                continue;
            }

            if (tileNumber < firstTileNumber || tileNumber > lastTileNumber)
            {
                j++;
                continue;
            }

            visibleTileNumbers.push_back(tileNumber);

            j++;
        }

        return visibleTileNumbers;
    }

    sf::Clock* getClock()
    {
        return &m_clock;
    }

    sf::Clock* getClockAnimatedWaterAndObjects()
    {
        return &m_clockAnimatedWaterAndObjects;
    }

    sf::Sprite getWindowSprite()
    {
        return m_windowSprite;
    }

    sf::RectangleShape getWindowBorder()
    {
        return m_windowBorder;
    }

    sf::View* getWindowView()
    {
        return &m_windowView;
    }

    sf::Font* getFont()
    {
        return &m_font;
    }

    tibia::Map* getMap()
    {
        return &m_map;
    }

    tibia::Player* getPlayer()
    {
        return &m_player;
    }

private:

    sf::Clock m_clock;
    sf::Clock m_clockAnimatedWaterAndObjects;

    sf::RenderTexture m_window;
    sf::Sprite m_windowSprite;
    sf::RectangleShape m_windowBorder;
    sf::View m_windowView;

    sf::RenderTexture m_rtLight;
    sf::RectangleShape m_rectLight;

    sf::Font m_font;

    std::vector<sf::Text> m_textList;

    sf::Clock m_clockText;

    tibia::Map m_map;

    tibia::Player m_player;

    std::vector<std::shared_ptr<tibia::Creature>> m_creaturesList;
    std::vector<std::shared_ptr<tibia::Creature>>::iterator m_creaturesList_it;

    sf::Thread m_threadAnimatedWater;
    sf::Thread m_threadAnimatedObjects;

};

}

#endif // TIBIA_GAME_HPP

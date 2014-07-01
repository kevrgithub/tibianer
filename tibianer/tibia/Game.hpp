#ifndef TIBIA_GAME_HPP
#define TIBIA_GAME_HPP

#include <iostream>
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Tile.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/BitmapFont.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Object.hpp"
#include "tibia/Creature.hpp"

namespace tibia
{

class Game
{

public:

    Game()
    :
        m_gameWindowView(sf::FloatRect(0, 0, tibia::GuiData::gameWindowWidth, tibia::GuiData::gameWindowHeight)),
        m_miniMapWindowView(sf::FloatRect(0, 0, tibia::GuiData::gameWindowWidth * 2, tibia::GuiData::gameWindowHeight * 2))
    {
        m_bitmapFontDefault.load(tibia::BitmapFonts::default, tibia::BitmapFonts::defaultGlyphSize, &tibia::BitmapFonts::defaultGlyphWidths);

        m_tileMapTileVertices.setPrimitiveType(sf::Quads);
    }

    bool createWindows()
    {
        if (m_gameWindow.create(tibia::GuiData::gameWindowWidth, tibia::GuiData::gameWindowHeight) == false)
        {
            return false;
        }

        m_gameWindowBorder.setSize(sf::Vector2f(m_gameWindow.getSize().x, m_gameWindow.getSize().y));
        m_gameWindowBorder.setOutlineColor(tibia::Colors::windowBorderColor);
        m_gameWindowBorder.setOutlineThickness(1);
        m_gameWindowBorder.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        if (m_miniMapWindow.create(tibia::GuiData::miniMapWindowWidth, tibia::GuiData::miniMapWindowHeight) == false)
        {
            return false;
        }

        m_miniMapWindowBorder.setSize(sf::Vector2f(m_miniMapWindow.getSize().x, m_miniMapWindow.getSize().y));
        m_miniMapWindowBorder.setOutlineColor(tibia::Colors::windowBorderColor);
        m_miniMapWindowBorder.setOutlineThickness(1);
        m_miniMapWindowBorder.setPosition(tibia::GuiData::miniMapWindowX, tibia::GuiData::miniMapWindowY);

        return true;
    }

    void createPlayer()
    {
        tibia::CreaturePtr player = std::make_shared<tibia::Creature>(10, 10, tibia::ZAxis::ground);

        player->setName("Player");
        player->setIsPlayer(true);
        player->setHasOutfit(true);
        player->setIsSitting(false);
        player->setTeam(tibia::Teams::good);
        player->setHpMax(10000);
        player->setHp(10000);
        player->setMovementSpeed(tibia::MovementSpeeds::player);

        m_player = std::move(player);

        tibia::Tile* tile = getCreatureTile(m_player);

        tile->addCreature(m_player);
    }

    bool loadTextures()
    {
        if (tibia::Textures::sprites.loadFromFile("images/sprites.png") == false)
        {
            return false;
        }

        if (tibia::Textures::lights.loadFromFile("images/lights.png") == false)
        {
            return false;
        }

        if (tibia::Textures::background.loadFromFile("images/background.png") == false)
        {
            return false;
        }

        if (tibia::Textures::cursor.loadFromFile("images/cursor.png") == false)
        {
            return false;
        }

        return true;
    }

    bool loadFonts()
    {
        if (m_fontDefault.loadFromFile(tibia::Fonts::default) == false)
        {
            return false;
        }

        return true;
    }

    bool loadMap(const std::string& filename)
    {
        return m_map.load(filename);
    }

    void loadSpriteFlags()
    {
        for (unsigned int i = 1; i < tibia::SPRITES_TOTAL + 1; i++)
        {
            tibia::spriteFlags[i] = tibia::Utility::getSpriteFlags(i);
        }
    }

    void handleKeyboardInput(sf::Event event)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::Up:
            case sf::Keyboard::Right:
            case sf::Keyboard::Down:
            case sf::Keyboard::Left:
                handleCreatureMovement(m_player, tibia::Utility::getDirectionByKey(event.key.code), event.key.control);
                break;

            case sf::Keyboard::O:
                m_player->setOutfitRandom();

                std::cout << "Player Head: " << m_player->getOutfitHead() << std::endl;
                std::cout << "Player Body: " << m_player->getOutfitBody() << std::endl;
                std::cout << "Player Legs: " << m_player->getOutfitLegs() << std::endl;
                std::cout << "Player Feet: " << m_player->getOutfitFeet() << std::endl;
                break;

            case sf::Keyboard::Z:
                m_player->setZ(getRandomNumber(tibia::ZAxis::floor, tibia::ZAxis::ceiling));

                std::cout << "Player Z: " << m_player->getZ() << std::endl;
                break;
        }
    }

    void handleMouseInput()
    {
        //
    }

    void handleCreatureMovement(tibia::CreaturePtr creature, int direction, bool turnOnly = false)
    {
        if (creature->isDead() == true)
        {
            return;
        }

        if (direction == tibia::Directions::up)
        {
            if (creature->getY() == 0)
            {
                return;
            }
        }
        else if (direction == tibia::Directions::right)
        {
            if (creature->getX() == MAP_XY_MAX - 1)
            {
                return;
            }
        }
        else if (direction == tibia::Directions::down)
        {
            if (creature->getY() == MAP_XY_MAX - 1)
            {
                return;
            }
        }
        else if (direction == tibia::Directions::left)
        {
            if (creature->getX() == 0)
            {
                return;
            }
        }

        else if (direction == tibia::Directions::upLeft)
        {
            if (creature->getY() == 0 || creature->getY() == 0)
            {
                return;
            }
        }
        else if (direction == tibia::Directions::upRight)
        {
            if (creature->getY() == 0 || creature->getX() == MAP_XY_MAX - 1)
            {
                return;
            }
        }
        else if (direction == tibia::Directions::downLeft)
        {
            if (creature->getX() == 0 || creature->getY() == MAP_XY_MAX - 1)
            {
                return;
            }
        }
        else if (direction == tibia::Directions::downRight)
        {
            if (creature->getX() == MAP_XY_MAX - 1 || creature->getY() == MAP_XY_MAX - 1)
            {
                return;
            }
        }

        sf::Time timeMovement = creature->getClockMovement()->getElapsedTime();

        if (timeMovement.asSeconds() >= creature->getMovementSpeed())
        {
            creature->setIsMovementReady(true);
        }

        if (creature->getIsMovementReady() == false)
        {
            return;
        }

        if (turnOnly == false)
        {
            tibia::Tile* currentTile = getCreatureTile(creature);

            tibia::CreatureList* creatureList = currentTile->getCreatureList();

            auto creatureIt = std::find(creatureList->begin(), creatureList->end(), creature);

            if (creatureIt != creatureList->end())
            {
                tibia::Tile* nextTile = getCreatureTileByMovement(creature, direction);

                if (nextTile != nullptr)
                {
                    bool tileIsSolid = false;

                    if (nextTile->getFlags() & tibia::SpriteFlags::solid)
                    {
                        tileIsSolid = true;
                    }

                    bool tileHasSolidObject = false;
                    bool tileHasChairObject = false;

                    tibia::ObjectList* objectList = nextTile->getObjectList();

                    for (auto object : *objectList)
                    {
                        if (object->getFlags() & tibia::SpriteFlags::solid)
                        {
                            tileHasSolidObject = true;
                        }

                        if (object->getFlags() & tibia::SpriteFlags::chair)
                        {
                            tileHasChairObject = true;
                        }
                    }

                    if (tileIsSolid == false && tileHasSolidObject == false)
                    {
                        nextTile->addCreature(creature);

                        creatureList->erase(creatureIt);

                        creature->doMove(direction);

                        creature->setIsSitting(tileHasChairObject);

                        updateStepTile(currentTile);
                        updateStepTile(nextTile);
                    }
                }
            }
        }

        creature->doTurn(direction);

        creature->setIsMovementReady(false);

        creature->getClockMovement()->restart();

        creature->update();
    }

    void updateStepTile(tibia::Tile* tile)
    {
        // wood
        if (tile->getId() == tibia::SpriteData::stepTileWood[0])
        {
            tile->setId(tibia::SpriteData::stepTileWood[1]);
        }
        else if (tile->getId() == tibia::SpriteData::stepTileWood[1])
        {
            tile->setId(tibia::SpriteData::stepTileWood[0]);
        }

        // stone
        else if (tile->getId() == tibia::SpriteData::stepTileStone[0])
        {
            tile->setId(tibia::SpriteData::stepTileStone[1]);
        }
        else if (tile->getId() == tibia::SpriteData::stepTileStone[1])
        {
            tile->setId(tibia::SpriteData::stepTileStone[0]);
        }

        // grass hole (one way)
        else if (tile->getId() == tibia::SpriteData::stepTileGrassHole[0])
        {
            tile->setId(tibia::SpriteData::stepTileGrassHole[1]);
        }
    }

    tibia::Tile* getCreatureTile(tibia::CreaturePtr creature)
    {
        tibia::TileList* tileList = m_map.tileMapTiles[creature->getZ()].getTileList();

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(creature->getTileX(), creature->getTileY()));

        return tileList->at(tileNumber).get();
    }

    tibia::Tile* getCreatureTileByMovement(tibia::CreaturePtr creature, int direction)
    {
        int addX = 0;
        int addY = 0;

        switch (direction)
        {
            case tibia::Directions::up:
                addY = -1;
                break;

            case tibia::Directions::right:
                addX = 1;
                break;

            case tibia::Directions::down:
                addY = 1;
                break;

            case tibia::Directions::left:
                addX = -1;
                break;

            case tibia::Directions::upLeft:
                addX = -1;
                addY = -1;
                break;

            case tibia::Directions::upRight:
                addX = 1;
                addY = -1;
                break;

            case tibia::Directions::downLeft:
                addX = -1;
                addY = 1;
                break;

            case tibia::Directions::downRight:
                addX = 1;
                addY = 1;
                break;
        }

        addX = addX * tibia::TILE_SIZE;
        addY = addY * tibia::TILE_SIZE;

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(creature->getTileX() + addX, creature->getTileY() + addY));

        if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
        {
            return nullptr;
        }
 
        return m_map.tileMapTiles[creature->getZ()].getTileList()->at(tileNumber).get();
    }

    void drawTileMap(tibia::TileMap* tileMap)
    {
        int x = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE) - NUM_TILES_FROM_CENTER_X;
        int y = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE) - NUM_TILES_FROM_CENTER_Y;

        if (x < 0) x = 0;
        if (y < 0) y = 0;

        std::vector<int>* tiles = tileMap->getTiles();

        if (tiles->size() == 0)
        {
            return;
        }

        tibia::TileList* tileList = tileMap->getTileList();

        m_tileMapTileVertices.clear();

        for (unsigned int i = x; i < x + tibia::NUM_TILES_X + 1; i++) // + 1 to draw tall objects that are offscreen
        {
            for (unsigned int j = y; j < y + tibia::NUM_TILES_Y + 1; j++) // + 1 to draw tall objects that are offscreen
            {
                if (i > tibia::MAP_SIZE - 1) continue;
                if (j > tibia::MAP_SIZE - 1) continue;

                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
                {
                    continue;
                }

                tibia::Tile* tile = tileList->at(tileNumber).get();

                int tileId = tile->getId(); //tiles->at(tileNumber);

                if (tileId == tibia::TILE_NULL || tileId == 1)
                {
                    continue;
                }

                int tu = (tileId - 1) % (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE);
                int tv = (tileId - 1) / (tibia::Textures::sprites.getSize().x / tibia::TILE_SIZE);

                sf::Vertex quad[4];

                quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                quad[0].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, tv       * tibia::TILE_SIZE);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);
                quad[3].texCoords = sf::Vector2f(tu       * tibia::TILE_SIZE, (tv + 1) * tibia::TILE_SIZE);

                m_tileMapTileVertices.append(quad[0]);
                m_tileMapTileVertices.append(quad[1]);
                m_tileMapTileVertices.append(quad[2]);
                m_tileMapTileVertices.append(quad[3]);

                if (tileMap->getType() == tibia::TileMapTypes::tiles)
                {
                    tibia::ObjectList* tileObjects = tile->getObjectList();

                    if (tileObjects->size())
                    {
                        tibia::ObjectList* tileMapObjects = &m_tileMapObjects[tileMap->getZ()];

                        std::copy(tileObjects->begin(), tileObjects->end(), std::back_inserter(*tileMapObjects));
                    }

                    tibia::CreatureList* tileCreatures = tile->getCreatureList();

                    if (tileCreatures->size())
                    {
                        tibia::CreatureList* tileMapCreatures = &m_tileMapCreatures[tileMap->getZ()];

                        std::copy(tileCreatures->begin(), tileCreatures->end(), std::back_inserter(*tileMapCreatures));
                    }
                }
            }
        }

        sf::RenderStates states;
        states.texture = &tibia::Textures::sprites;

        m_gameWindow.draw(m_tileMapTileVertices, states);
    }

    void addObjectListToThingList(tibia::ObjectList* objectList)
    {
        for (auto object : *objectList)
        {
            object->update();

            // fix drawing order of objects on walls
            for (auto spriteId : tibia::SpriteData::fixDrawOrderObjects)
            {
                if (object->getId() == spriteId)
                {
                    object->setTileX(object->getTileX() + tibia::TILE_SIZE);
                    object->setTileY(object->getTileY() + tibia::TILE_SIZE);
                    break;
                }
            }

            tibia::ThingList* tileMapThings = &m_tileMapThings[object->getZ()];

            tileMapThings->push_back(object);
        }
    }

    void addCreatureListToThingList(tibia::CreatureList* creatureList)
    {
        for (auto creature : *creatureList)
        {
            creature->update();

            tibia::ThingList* tileMapThings = &m_tileMapThings[creature->getZ()];

            tileMapThings->push_back(creature);
        }
    }

    void drawThingList(tibia::ThingList* thingList)
    {
        std::stable_sort(thingList->begin(), thingList->end(), tibia::ThingSort::sortByTileCoords());

        for (auto thing : *thingList)
        {
            m_gameWindow.draw(*thing);
        }
    }

    void drawGameLayer(int z)
    {
        drawTileMap(&m_map.tileMapTiles[z]);
        drawTileMap(&m_map.tileMapTileEdges[z]);

        addObjectListToThingList(&m_tileMapObjects[z]);
        addCreatureListToThingList(&m_tileMapCreatures[z]);

        drawThingList(&m_tileMapThings[z]);
    }

    void drawGameWindow(sf::RenderWindow* mainWindow)
    {
        m_gameWindowView.setCenter
        (
            m_player->getTileX() + (tibia::TILE_SIZE / 2),
            m_player->getTileY() + (tibia::TILE_SIZE / 2)
        );

        m_gameWindow.setView(m_gameWindowView);
        m_gameWindow.clear(tibia::Colors::black);

        for (unsigned int i = tibia::ZAxis::floor; i < tibia::ZAxis::ceiling + 1; i++)
        {
            m_tileMapObjects[i].clear();
            m_tileMapCreatures[i].clear();
            m_tileMapThings[i].clear();
        }

        if (m_player->getZ() < tibia::ZAxis::ground)
        {
            drawGameLayer(tibia::ZAxis::floor);

            if (m_player->getZ() > tibia::ZAxis::floor)
            {
                drawGameLayer(tibia::ZAxis::underGround);
            }
        }
        else
        {
            drawGameLayer(tibia::ZAxis::ground);

            if (findTilesAbovePlayer(&m_map.tileMapTiles[tibia::ZAxis::aboveGround]) == false)
            {
                drawGameLayer(tibia::ZAxis::aboveGround);

                if (findTilesAbovePlayer(&m_map.tileMapTiles[tibia::ZAxis::ceiling]) == false)
                {
                    drawGameLayer(tibia::ZAxis::ceiling);
                }
            }
        }

        m_gameWindow.display();

        m_gameWindowSprite.setTexture(m_gameWindow.getTexture());
        m_gameWindowSprite.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        mainWindow->draw(m_gameWindowBorder);
        mainWindow->draw(m_gameWindowSprite);

        tibia::BitmapFontText bft;
        bft.setText(&m_bitmapFontDefault, "The quick brown fox jumps over the lazy dog!", tibia::Colors::textYellow);

        bft.setPosition
        (
            (mainWindow->getSize().x / 2) - (bft.getVertexArray()->getBounds().width / 2),
            0
        );

        sf::RenderStates bftstates;
        bftstates.texture = m_bitmapFontDefault.getTexture();

        mainWindow->draw(bft, bftstates);
    }

    bool findTilesAbovePlayer(tibia::TileMap* tileMap)
    {
        tibia::TileList* tileList = tileMap->getTileList();

        std::vector<int> nearbyTileNumbers;

        for (int i = -2; i < 3; i++)
        {
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() - (2 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() - (1 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX()                         , m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() + (1 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() + (2 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
        }

        for (auto tileNumber : nearbyTileNumbers)
        {
            if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
            {
                continue;
            }

            tibia::Tile* tile = tileList->at(tileNumber).get();

            if (tile->getId() != tibia::TILE_NULL && tile->getZ() > m_player->getZ())
            {
                return true;
            }
        }

        return false;
    }

    bool isWaterVisible()
    {
        int x = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE) - NUM_TILES_FROM_CENTER_X;
        int y = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE) - NUM_TILES_FROM_CENTER_Y;

        if (x < 0) x = 0;
        if (y < 0) y = 0;

        for (int i = x; i < x + tibia::NUM_TILES_X; i++)
        {
            for (int j = y; j < y + tibia::NUM_TILES_Y; j++)
            {
                if (i > tibia::MAP_SIZE - 1) continue;
                if (j > tibia::MAP_SIZE - 1) continue;

                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
                {
                    continue;
                }

                if (m_map.tileMapTiles[tibia::ZAxis::ground].getTileList()->at(tileNumber)->getFlags() & tibia::SpriteFlags::water)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void doAnimatedWater()
    {
        if (isWaterVisible() == false)
        {
            return;
        }

        m_map.tileMapTiles[tibia::ZAxis::ground].doAnimatedWater();
    }

    void doAnimatedObjects()
    {
        if (m_player->getZ() < tibia::ZAxis::ground)
        {
            doAnimateObjectList(&m_tileMapObjects[tibia::ZAxis::floor]);
            doAnimateObjectList(&m_tileMapObjects[tibia::ZAxis::underGround]);
        }
        else
        {
            doAnimateObjectList(&m_tileMapObjects[tibia::ZAxis::ground]);
            doAnimateObjectList(&m_tileMapObjects[tibia::ZAxis::aboveGround]);
            doAnimateObjectList(&m_tileMapObjects[tibia::ZAxis::ceiling]);
        }
    }

    void doAnimateObjectList(tibia::ObjectList* objectList)
    {
        if (objectList->size() == 0)
        {
            return;
        }

        for (auto object : *objectList)
        {
            if (object->isAnimated() == true)
            {
                object->doAnimation();
            }
        }
    }

    sf::Clock* getClock()
    {
        return &m_clock;
    }

    sf::Clock* getClockAnimatedWaterAndObjects()
    {
        return &m_clockAnimatedWaterAndObjects;
    }

    sf::Font* getFontDefault()
    {
        return &m_fontDefault;
    }

    sf::RenderTexture* getGameWindow()
    {
        return &m_gameWindow;
    }

    sf::View* getGameWindowView()
    {
        return &m_gameWindowView;
    }

    tibia::Map* getMap()
    {
        return &m_map;
    }

private:

    sf::Clock m_clock;
    sf::Clock m_clockAnimatedWaterAndObjects;

    sf::Font m_fontDefault;

    tibia::BitmapFont m_bitmapFontDefault;

    sf::RenderTexture m_gameWindow;
    sf::View m_gameWindowView;
    sf::Sprite m_gameWindowSprite;
    sf::RectangleShape m_gameWindowBorder;

    sf::RenderTexture m_miniMapWindow;
    sf::View m_miniMapWindowView;
    sf::Sprite m_miniMapWindowSprite;
    sf::RectangleShape m_miniMapWindowBorder;

    tibia::Map m_map;

    sf::VertexArray m_tileMapTileVertices;

    tibia::ObjectList m_tileMapObjects[5];

    tibia::CreatureList m_tileMapCreatures[5];

    tibia::ThingList m_tileMapThings[5];

    tibia::CreaturePtr m_player;
};

} // tibia

#endif // TIBIA_GAME_HPP

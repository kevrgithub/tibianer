#ifndef TIBIA_GAME_HPP
#define TIBIA_GAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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
#include "tibia/GameText.hpp"
#include "tibia/FloatingText.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Object.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Animation.hpp"
#include "tibia/Light.hpp"

namespace tibia
{

class Game
{

public:

    Game::Game()
    :
        m_gameWindowView(sf::FloatRect(0, 0, tibia::GuiData::gameWindowWidth, tibia::GuiData::gameWindowHeight)),
        m_miniMapWindowView(sf::FloatRect(0, 0, tibia::GuiData::gameWindowWidth * tibia::MiniMapWindow::zoomDefault, tibia::GuiData::gameWindowHeight * tibia::MiniMapWindow::zoomDefault))
    {
        m_miniMapWindowZoom = tibia::MiniMapWindow::zoomDefault;

        m_tileMapTileVertices.setPrimitiveType(sf::Quads);

        m_statusBarText.setPosition(tibia::StatusBarTextData::position);

        m_gameWindowLayer.create(tibia::MAP_TILE_XY_MAX, tibia::MAP_TILE_XY_MAX);

        m_gameWindowLayerSprite.setPosition(0, 0);
        m_gameWindowLayerSprite.setTextureRect(sf::IntRect(0, 0, tibia::MAP_TILE_XY_MAX, tibia::MAP_TILE_XY_MAX));

        m_lightLayer.create(tibia::MAP_TILE_XY_MAX, tibia::MAP_TILE_XY_MAX);

        m_lightLayerSprite.setPosition(0, 0);
        m_lightLayerSprite.setTextureRect(sf::IntRect(0, 0, tibia::MAP_TILE_XY_MAX, tibia::MAP_TILE_XY_MAX));

        m_timeOfDay = tibia::TimeOfDay::day;

        m_lightBrightness = tibia::LightBrightnesses::day;
    }

    bool createWindows()
    {
        if (m_gameWindow.create(tibia::GuiData::gameWindowWidth, tibia::GuiData::gameWindowHeight) == false)
        {
            return false;
        }

        if (m_miniMapWindow.create(tibia::GuiData::miniMapWindowWidth, tibia::GuiData::miniMapWindowHeight) == false)
        {
            return false;
        }

        return true;
    }

    void createPlayer()
    {
        tibia::CreaturePtr player = std::make_shared<tibia::Creature>
        (
            m_map.properties.playerStartX * tibia::TILE_SIZE,
            m_map.properties.playerStartY * tibia::TILE_SIZE,
            m_map.properties.playerStartZ
        );

        player->setIsPlayer(true);
        player->setName(tibia::CREATURE_NAME_PLAYER);
        player->setHasCustomName(true);
        player->setHasOutfit(true);
        player->setTeam(tibia::Teams::good);
        player->setHpMax(10000);
        player->setHp(10000);
        player->setMovementSpeed(tibia::MovementSpeeds::player);

        m_player = std::move(player);

        tibia::TilePtr tile = getThingTile(m_player);

        tile->addCreature(m_player);
    }

    void showMapNameAndAuthor()
    {
        std::stringstream ss;
        ss << m_map.properties.name << "\nby " << m_map.properties.author;

        spawnGameText(m_player->getTilePosition(), m_player->getZ(), ss.str(), tibia::Colors::textWhite);
    }

    void doEnterGame()
    {
        setTimeOfDay(m_map.properties.timeOfDay);

        spawnAnimation(m_player->getTilePosition(), m_player->getZ(), tibia::Animations::spellBlue);

        showMapNameAndAuthor();
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

        m_mouseCursor.setTexture(tibia::Textures::cursor);

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

    bool loadBitmapFonts()
    {
        if (m_bitmapFontDefault.load(tibia::BitmapFonts::default, tibia::BitmapFonts::defaultGlyphSize, &tibia::BitmapFonts::defaultGlyphWidths) == false)
        {
            return false;
        }

        if (m_bitmapFontTiny.load(tibia::BitmapFonts::tiny, tibia::BitmapFonts::tinyGlyphSize, &tibia::BitmapFonts::tinyGlyphWidths, -1) == false)
        {
            return false;
        }

        if (m_bitmapFontModern.load(tibia::BitmapFonts::modern, tibia::BitmapFonts::modernGlyphSize, &tibia::BitmapFonts::modernGlyphWidths, -1) == false)
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
            tibia::umapSpriteFlags[i] = tibia::Utility::getSpriteFlags(i);
        }
    }

    void handleKeyboardEvent(sf::Event event)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::Space:
                doPlayerInteractWithPlayerTileObjects();
                break;

            case sf::Keyboard::B:
                spawnAnimation(m_player->getTilePosition(), m_player->getZ(), tibia::Animations::spellBlue);
                break;

            case sf::Keyboard::F:
                spawnFloatingText(m_player->getTilePosition(), m_player->getZ(), "Floating Text", tibia::Colors::textWhite);
                break;

            case sf::Keyboard::H:
                spawnGameText(m_player->getTilePosition(), m_player->getZ(), "You say:\nHello!\nWelcome to\nthe world of\nTibianer!", tibia::Colors::textYellow);
                break;

            case sf::Keyboard::M:
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::magicWall);
                break;

            case sf::Keyboard::P:
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::poolRed);
                break;

            case sf::Keyboard::T:
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::torchBig);
                break;

            case sf::Keyboard::N:
                toggleTimeOfDay();
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

    void handleKeyboardInput()
    {
        bool isKeyControlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            handleCreatureMovement(m_player, tibia::Directions::up, isKeyControlPressed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            handleCreatureMovement(m_player, tibia::Directions::right, isKeyControlPressed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            handleCreatureMovement(m_player, tibia::Directions::down, isKeyControlPressed);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            handleCreatureMovement(m_player, tibia::Directions::left, isKeyControlPressed);
        }
    }

    void handleMouseEvent(sf::Event event)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (isMouseInsideMiniMapWindow() == true)
            {
                if (m_miniMapWindowZoom == tibia::MiniMapWindow::zoomDefault)
                {
                    m_miniMapWindowZoom = tibia::MiniMapWindow::zoomMax;
                }
                else if (m_miniMapWindowZoom == tibia::MiniMapWindow::zoomMax)
                {
                    m_miniMapWindowZoom = tibia::MiniMapWindow::zoomMin;
                }
                else if (m_miniMapWindowZoom == tibia::MiniMapWindow::zoomMin)
                {
                    m_miniMapWindowZoom = tibia::MiniMapWindow::zoomDefault;
                }

                m_miniMapWindowView.setSize(sf::Vector2f(tibia::GuiData::gameWindowWidth * m_miniMapWindowZoom, tibia::GuiData::gameWindowHeight * m_miniMapWindowZoom));
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (isMouseInsideGameWindow() == true)
            {
                doCreatureInteractWithTileObjects(m_player, m_mouseTile);
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Middle)
        {
            if (isMouseInsideGameWindow() == true)
            {
                tibia::ObjectList* objectList = m_mouseTile->getObjectList();

                if (objectList->size())
                {
                    tibia::ObjectPtr object = objectList->back();

                    std::cout << "Push Object ID: " << object->getId() << std::endl;

                    doCreaturePushObject(m_player, object);
                }
            }
        }
    }

    void handleMouseInput()
    {
        if (isMouseInsideGameWindow() == false)
        {
            return;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            doPlayerClickToMove();
        }
    }

    void setMouseCursorType(unsigned int type)
    {
        m_mouseCursor.setTextureRect(sf::IntRect(0, type * tibia::MouseCursor::height, tibia::MouseCursor::width, tibia::MouseCursor::height));
    }

    void doCreatureLogic(int z)
    {
        for (auto& creature : m_tileMapCreatures[z])
        {
            if (creature->isPlayer() == true || creature->isDead() == true || creature->isSleeping() == true)
            {
                continue;
            }

            sf::Time timeLogic = creature->getClockLogic()->getElapsedTime();

            float randomTime = getRandomNumberFloat(1.0, 2.0);

            if (timeLogic.asSeconds() >= randomTime)
            {
                creature->setIsLogicReady(true);
            }

            if (creature->getIsLogicReady() == false)
            {
                continue;
            }

            int direction = getRandomNumber(tibia::Directions::up, tibia::Directions::left);

            handleCreatureMovement(creature, direction);

            creature->setIsLogicReady(false);

            creature->getClockLogic()->restart();
        }
    }

    void doPlayerInteractWithPlayerTileObjects()
    {
        tibia::TilePtr playerTile = getPlayerTile();

        if (playerTile == nullptr)
        {
            return;
        }

        if (playerTile->getObjectList()->size())
        {
            doCreatureInteractWithTileObjects(m_player, playerTile);
        }
        else
        {
            doCreatureInteractWithTileObjects(m_player, getTileByCreatureDirection(m_player));
        }
    }

    void doPlayerClickToMove()
    {
        if (tibia::Utility::isTilePositionOutOfBounds(getMouseTilePosition()) == true)
        {
            return;
        }

        sf::Vector2f playerTilePosition = static_cast<sf::Vector2f>(m_player->getTilePosition());

        sf::Vector2f mouseTilePosition = static_cast<sf::Vector2f>(getMouseTilePosition());

        if (playerTilePosition == mouseTilePosition)
        {
            return;
        }

        sf::Vector2f playerMovementNormal = tibia::Utility::getNormalByVectors
        (
            playerTilePosition,
            mouseTilePosition
        );

        int playerMovementDirection = tibia::Utility::getDirectionByVector(playerMovementNormal);

        handleCreatureMovement(m_player, playerMovementDirection);
    }

    tibia::Tile::TileProperties_t getTileProperties(tibia::TilePtr tile)
    {
        tibia::Tile::TileProperties_t tileProperties;

        if (tile->getFlags() & tibia::SpriteFlags::solid)
        {
            tileProperties.isSolid = true;
        }

        if (tile->getFlags() & tibia::SpriteFlags::moveAbove)
        {
            tileProperties.isMoveAbove = true;
        }

        if (tile->getFlags() & tibia::SpriteFlags::moveBelow)
        {
            tileProperties.isMoveBelow = true;

            if (tile->getId() == tibia::SpriteData::mountainRampLeftMoveBelow)
            {
                tileProperties.isMountainRampLeftMoveBelow = true;
            }
            else if (tile->getId() == tibia::SpriteData::mountainRampRightMoveBelow)
            {
                tileProperties.isMountainRampRightMoveBelow = true;
            }
        }

        tibia::ObjectList* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getFlags() & tibia::SpriteFlags::solid)
            {
                tileProperties.hasSolidObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::hasHeight)
            {
                tileProperties.hasHasHeightObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::moveAbove)
            {
                tileProperties.hasMoveAboveObject = true;

                if (object->getId() == tibia::SpriteData::mountainRampLeft)
                {
                    tileProperties.hasMountainRampLeftObject = true;
                }
                else if (object->getId() == tibia::SpriteData::mountainRampRight)
                {
                    tileProperties.hasMountainRampRightObject = true;
                }
            }

            if (object->getFlags() & tibia::SpriteFlags::moveBelow)
            {
                tileProperties.hasMoveBelowObject = true;
            }

            if (object->getType() == tibia::ObjectTypes::teleporter)
            {
                tileProperties.hasTeleporterObject = true;
            }
        }

        tibia::CreatureList* creatureList = tile->getCreatureList();

        for (auto& creature : *creatureList)
        {
            if (creature->isDead() == false && creature->isSleeping() == false)
            {
                tileProperties.hasSolidCreature = true;
            }
        }

        return tileProperties;
    }

    tibia::TilePtr getTile(sf::Vector2u tileCoords, int z)
    {
        int tileNumber = tibia::Utility::getTileNumberByTileCoords(tileCoords);

        tibia::TileList* tileList = m_map.tileMapTiles[z].getTileList();

        return tileList->at(tileNumber);
    }

    tibia::TilePtr getPlayerTile()
    {
        return getTile(m_player->getTilePosition(), m_player->getZ());
    }

    tibia::TilePtr getTileByTileDirection(tibia::TilePtr tile, int direction)
    {
        sf::Vector2u position = tile->getPosition();

        sf::Vector2i normal = tibia::Utility::getVectorByDirection(direction);

        position.x += normal.x * tibia::TILE_SIZE;
        position.y += normal.y * tibia::TILE_SIZE;

        return getTile(position, tile->getZ());
    }

    tibia::TilePtr getTileByThingDirection(tibia::ThingPtr thing, int direction)
    {
        sf::Vector2u position = thing->getTilePosition();

        sf::Vector2i normal = tibia::Utility::getVectorByDirection(direction);

        position.x += normal.x * tibia::TILE_SIZE;
        position.y += normal.y * tibia::TILE_SIZE;

        return getTile(position, thing->getZ());
    }

    tibia::TilePtr getTileByCreatureDirection(tibia::CreaturePtr creature)
    {
        return getTileByThingDirection(creature, creature->getDirection());
    }

    tibia::TilePtr getTileForMoveAbove(sf::Vector2u toTileCoords, int toZ)
    {
        tibia::TilePtr tile = getTile(sf::Vector2u(toTileCoords.x - tibia::TILE_SIZE, toTileCoords.y - (tibia::TILE_SIZE * 2)), toZ);

        return tile;
    }

    tibia::TilePtr getTileForMoveBelow(sf::Vector2u toTileCoords, int toZ)
    {
        tibia::TilePtr tile = getTile(sf::Vector2u(toTileCoords.x + tibia::TILE_SIZE, toTileCoords.y + tibia::TILE_SIZE), toZ);

        tibia::ObjectList* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getFlags() & tibia::SpriteFlags::moveAbove)
            {
                tile = getTile(sf::Vector2u(tile->getPosition().x, tile->getPosition().y + tibia::TILE_SIZE), toZ);
                break;
            }
        }

        return tile;
    }

    tibia::ObjectPtr findTileObject(sf::Vector2u tileCoords, int z, int id)
    {
        tibia::TilePtr tile = getTile(tileCoords, z);

        if (tile == nullptr)
        {
            return nullptr;
        }

        tibia::ObjectList* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getId() == id)
            {
                return object;
            }
        }

        return nullptr;
    }

/*
    bool doMoveObjectToTile(tibia::ObjectPtr object, tibia::TilePtr toTile)
    {
        tibia::TilePtr fromTile = getThingTile(object);

        if (fromTile == nullptr || toTile == nullptr || toTile->getId() == tibia::TILE_NULL)
        {
            return false;
        }

        tibia::ObjectList* objectList = fromTile->getObjectList();

        auto objectIt = std::find(objectList->begin(), objectList->end(), object);

        if (objectIt == objectList->end())
        {
            return false;
        }

        toTile->addObject(object);

        objectList->erase(objectIt);

        object->setTileCoords(toTile->getPosition().x, toTile->getPosition().y);

        object->setZ(toTile->getZ());

        object->update();

        return true;
    }
*/

    bool doMoveThingToTile(tibia::ThingPtr thing, tibia::TilePtr toTile, int direction = tibia::Directions::up, bool ignoreAllCollision = false, bool ignoreCreatureCollision = false)
    {
        tibia::TilePtr fromTile = getThingTile(thing);

        if (fromTile == nullptr || toTile == nullptr || toTile->getId() == tibia::TILE_NULL)
        {
            return false;
        }

        tibia::CreaturePtr thingCreature = std::dynamic_pointer_cast<tibia::Creature>(thing);

        tibia::ObjectPtr thingObject = std::dynamic_pointer_cast<tibia::Object>(thing);

        tibia::CreatureList::iterator thingCreatureIt;

        tibia::ObjectList::iterator thingObjectIt;

        if (thingCreature != nullptr)
        {
            tibia::CreatureList* creatureList = fromTile->getCreatureList();

            thingCreatureIt = std::find(creatureList->begin(), creatureList->end(), thingCreature);

            if (thingCreatureIt == creatureList->end())
            {
                return false;
            }
        }

        if (thingObject != nullptr)
        {
            tibia::ObjectList* objectList = fromTile->getObjectList();

            thingObjectIt = std::find(objectList->begin(), objectList->end(), thingObject);

            if (thingObjectIt == objectList->end())
            {
                return false;
            }
        }

        tibia::Tile::TileProperties_t toTileProperties = getTileProperties(toTile);

        tibia::ObjectList* objectList = toTile->getObjectList();

        tibia::ObjectPtr teleporterObject = nullptr;

        for (auto& object : *objectList)
        {
            if (object->getType() == tibia::ObjectTypes::teleporter)
            {
                teleporterObject = object;
                break;
            }
        }

        int toZ = toTile->getZ();

        if (fromTile->getZ() >= tibia::ZAxis::ground && fromTile->getHeight() >= tibia::TILE_CLIMB_HEIGHT)
        {
            int climbToZ = toZ + 1;

            tibia::TilePtr climbToTile;

            if (direction == tibia::Directions::up || direction == tibia::Directions::left)
            {
                climbToTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y - tibia::TILE_SIZE), climbToZ);
            }
            else if (direction == tibia::Directions::right)
            {
                climbToTile = getTile(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y - tibia::TILE_SIZE), climbToZ);
            }
            else if (direction == tibia::Directions::down)
            {
                climbToTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y), climbToZ);
            }

            if (climbToTile != nullptr)
            {
                if (climbToTile->getId() != tibia::TILE_NULL)
                {
                    toZ = climbToZ;

                    toTile = climbToTile;
                }
            }
        }
        else if (toTileProperties.isMoveAbove == true || toTileProperties.hasMoveAboveObject == true)
        {
            toZ = toZ + 1;

            if (toTileProperties.hasMountainRampLeftObject == true)
            {
                toTile = getTile(sf::Vector2u(toTile->getPosition().x + tibia::TILE_SIZE, toTile->getPosition().y - tibia::TILE_SIZE), toZ);

                direction = tibia::Directions::right;
            }
            else if (toTileProperties.hasMountainRampRightObject == true)
            {
                toTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y - tibia::TILE_SIZE), toZ);

                direction = tibia::Directions::left;
            }
            else
            {
                toTile = getTileForMoveAbove(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y), toZ);

                direction = tibia::Directions::up;
            }
        }
        else if (toTileProperties.isMoveBelow == true || toTileProperties.hasMoveBelowObject == true)
        {
            toZ = toZ - 1;

            // update tile for falling in grass hole
            if (toTile->getId() == tibia::SpriteData::stepTileGrassHole[0])
            {
                toTile->setId(tibia::SpriteData::stepTileGrassHole[1]);

                toTile->setFlags(tibia::umapSpriteFlags[tibia::SpriteData::stepTileGrassHole[1]]);
            }

            if (toTileProperties.isMountainRampLeftMoveBelow == true)
            {
                toTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y + tibia::TILE_SIZE), toZ);
            }
            else
            {
                toTile = getTileForMoveBelow(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y), toZ);
            }

            if (toTileProperties.isMountainRampLeftMoveBelow == true)
            {
                direction = tibia::Directions::left;
            }
            else if (toTileProperties.isMountainRampRightMoveBelow == true)
            {
                direction = tibia::Directions::right;
            }
            else
            {
                direction = tibia::Directions::down;
            }
        }
        else if (toTileProperties.hasTeleporterObject == true)
        {
            toZ = teleporterObject->properties.teleporterZ;

            sf::Vector2u teleporterPosition
            (
                teleporterObject->properties.teleporterX * tibia::TILE_SIZE,
                teleporterObject->properties.teleporterY * tibia::TILE_SIZE
            );

            toTile = getTile(teleporterPosition, toZ);

            direction = tibia::Directions::down;
        }

        if (toTile == nullptr)
        {
            return false;
        }

        if (toTile->getId() == tibia::TILE_NULL)
        {
            return false;
        }

        if (ignoreAllCollision == false)
        {
            if (toZ == thing->getZ())
            {
                toTileProperties = getTileProperties(toTile);

                if (toTileProperties.isSolid == true || toTileProperties.hasSolidObject == true)
                {
                    return false;
                }

                if (ignoreCreatureCollision == false && toTileProperties.hasSolidCreature == true)
                {
                    if (teleporterObject == nullptr)
                    {
                        return false;
                    }
                }

                if (thingCreature != nullptr)
                {
                    if (toTile->getHeight() - fromTile->getHeight() >= tibia::TILE_HEIGHT_MOVEMENT_DIFFERENCE)
                    {
                        if (teleporterObject == nullptr)
                        {
                            if (thingCreature->isPlayer() == true)
                            {
                                showStatusBarText("You cannot move there, it is too high up.");
                            }

                            return false;
                        }
                    }
                }
            }
        }

        if (teleporterObject != nullptr)
        {
            spawnAnimation(thing->getTilePosition(), thing->getZ(), tibia::Animations::spellBlue);

            spawnAnimation(toTile->getPosition(), toZ, tibia::Animations::spellBlue);
        }

        if (thingCreature != nullptr)
        {
            toTile->addCreature(thingCreature);

            fromTile->getCreatureList()->erase(thingCreatureIt);

            thingCreature->doTurn(direction);

            thingCreature->update();
        }

        if (thingObject != nullptr)
        {
            toTile->addObject(thingObject);

            fromTile->getObjectList()->erase(thingObjectIt);

            thingObject->update();
        }

        thing->setTileCoords(toTile->getPosition().x, toTile->getPosition().y);

        thing->setZ(toZ);

        updateStepTile(fromTile);
        updateStepTile(toTile);

        return true;
    }

    void doCreatureInteractWithTileObjects(tibia::CreaturePtr creature, tibia::TilePtr tile)
    {
        if (creature == nullptr || tile == nullptr)
        {
            return;
        }

        tibia::ObjectList* objectList = tile->getObjectList();

        if (objectList->size() == 0)
        {
            return;
        }

        if (doCreatureInteractWithObject(creature, objectList->back()) == false)
        {
            for (auto& object : *objectList)
            {
                if (doCreatureInteractWithObject(creature, object) == true)
                {
                    break;
                }
            }
        }
    }

    bool doCreaturePushObject(tibia::CreaturePtr creature, tibia::ObjectPtr object)
    {
        if (creature == nullptr || object == nullptr)
        {
            return false;
        }

        // object is too far away
        if
        (
            tibia::Utility::calculateTileDistance
            (
                creature->getTileX(),
                creature->getTileY(),
                object->getTileX(),
                object->getTileY()
            )
            > 1
        )
        {
            showStatusBarText("Object is too far away.");
            return false;
        }

        if ((object->getFlags() & tibia::SpriteFlags::moveable) == false)
        {
            return false;
        }

        tibia::TilePtr fromTile = getThingTile(object);

        tibia::TilePtr toTile = getTileByTileDirection(fromTile, m_player->getDirection());

        bool ignoreCreatureCollision = false;

        if ((object->getFlags() & tibia::SpriteFlags::solid) == false)
        {
            ignoreCreatureCollision = true;
        }

        doMoveThingToTile(object, toTile, creature->getDirection(), false, ignoreCreatureCollision);

        return true;
    }

    bool doCreatureInteractWithObject(tibia::CreaturePtr creature, tibia::ObjectPtr object)
    {
        if (creature == nullptr || object == nullptr)
        {
            return false;
        }

        // object is too far away
        if
        (
            tibia::Utility::calculateTileDistance
            (
                creature->getTileX(),
                creature->getTileY(),
                object->getTileX(),
                object->getTileY()
            )
            > 1
        )
        {
            showStatusBarText("Object is too far away.");
            return false;
        }

        // street lamp
        if (object->getId() == tibia::SpriteData::streetLamp[0])
        {
            object->setId(tibia::SpriteData::streetLamp[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::streetLamp[1])
        {
            object->setId(tibia::SpriteData::streetLamp[0]);

            return true;
        }

        // signs
        if (object->getType() == tibia::ObjectTypes::sign)
        {
            std::stringstream signText;
            signText << "You see a " << object->properties.signName << ". It reads:\n" << object->properties.signText;

            std::cout << signText.str() << std::endl;

            spawnGameText(object->getTilePosition(), creature->getZ(), signText.str(), tibia::Colors::textGreen);

            return true;
        }

        // doors
        if (object->getType() == tibia::ObjectTypes::door)
        {
            // do not let creature close the door on itself
            if (creature->getTilePosition() == object->getTilePosition())
            {
                return false;
            }

            // door vertical
            if (object->getId() == tibia::SpriteData::doorVertical[0])
            {
                object->setId(tibia::SpriteData::doorVertical[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorVertical[1])
            {
                object->setId(tibia::SpriteData::doorVertical[0]);
            }

            // door horizontal
            if (object->getId() == tibia::SpriteData::doorHorizontal[0])
            {
                object->setId(tibia::SpriteData::doorHorizontal[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorHorizontal[1])
            {
                object->setId(tibia::SpriteData::doorHorizontal[0]);
            }

            // door locked vertical
            // TODO: make locked doors require keys
            if (object->getId() == tibia::SpriteData::doorLockedVertical[0])
            {
                object->setId(tibia::SpriteData::doorLockedVertical[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorLockedVertical[1])
            {
                object->setId(tibia::SpriteData::doorLockedVertical[0]);
            }

            // door locked horizontal
            // TODO: make locked doors require keys
            if (object->getId() == tibia::SpriteData::doorLockedHorizontal[0])
            {
                object->setId(tibia::SpriteData::doorLockedHorizontal[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorLockedHorizontal[1])
            {
                object->setId(tibia::SpriteData::doorLockedHorizontal[0]);
            }

            return true;
        }

        // ladder and rope up
        // TODO: make rope up require a rope
        if (object->getId() == tibia::SpriteData::ladder || object->getId() == tibia::SpriteData::ropeUp)
        {
            tibia::TilePtr toTile = getTileForMoveAbove(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() + 1);

            doMoveThingToTile(creature, toTile, tibia::Directions::up);

            return true;
        }

        // sewer grate
        if (object->getId() == tibia::SpriteData::sewerGrate)
        {
            tibia::TilePtr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() - 1);

            doMoveThingToTile(creature, toTile, tibia::Directions::down);

            return true;
        }

        // dig holes
        // TODO: require shovel or pickaxe to dig
        if (object->getId() == tibia::SpriteData::digHole[0] || object->getId() == tibia::SpriteData::digHoleIce[0])
        {
            if (object->getId() == tibia::SpriteData::digHole[0])
            {
                object->setId(tibia::SpriteData::digHole[1]);
            }
            else if (object->getId() == tibia::SpriteData::digHoleIce[0])
            {
                object->setId(tibia::SpriteData::digHoleIce[1]);
            }

            // things fall down the hole if it is dug while things are on top of it
            tibia::TilePtr fromTile = getThingTile(object);

            tibia::TilePtr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), object->getZ() - 1);

            if (fromTile != nullptr && toTile != nullptr)
            {
                tibia::CreatureList* fromCreatureList = fromTile->getCreatureList();

                for (auto fromCreature : *fromCreatureList)
                {
                    doMoveThingToTile(fromCreature, toTile, tibia::Directions::down);
                }

                tibia::ObjectList* fromObjectList = fromTile->getObjectList();

                for (auto fromObject : *fromObjectList)
                {
                    // do not let the dig hole fall into itself
                    if (fromObject == object)
                    {
                        continue;
                    }

                    doMoveThingToTile(fromObject, toTile, tibia::Directions::null, true);
                }
            }

            return true;
        }

        // beds
        if (object->getType() == tibia::ObjectTypes::bed)
        {
            // creature is already sleeping
            if
            (
                object->getId() == tibia::SpriteData::bedVertical[0]   ||
                object->getId() == tibia::SpriteData::bedVertical[1]   ||
                object->getId() == tibia::SpriteData::bedHorizontal[0] ||
                object->getId() == tibia::SpriteData::bedHorizontal[1] ||

                object->getId() == tibia::SpriteData::stretcherVertical[0]   ||
                object->getId() == tibia::SpriteData::stretcherVertical[1]   ||
                object->getId() == tibia::SpriteData::stretcherHorizontal[0] ||
                object->getId() == tibia::SpriteData::stretcherHorizontal[1]
            )
            {
                if (creature->isSleeping() == true)
                {
                    showStatusBarText("You are already sleeping in another bed.");
                    return false;
                }
            }

            // beds vertical
            if (object->getId() == tibia::SpriteData::bedVertical[0])
            {
                object->setId(tibia::SpriteData::bedVertical[2]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() + tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::bedVertical[1]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedVertical[3]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::bedVertical[1])
            {
                object->setId(tibia::SpriteData::bedVertical[3]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() - tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::bedVertical[0]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedVertical[2]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::bedVertical[2])
            {
                object->setId(tibia::SpriteData::bedVertical[0]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() + tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::bedVertical[3]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedVertical[1]);
                }

                creature->setIsSleeping(false);
            }
            else if (object->getId() == tibia::SpriteData::bedVertical[3])
            {
                object->setId(tibia::SpriteData::bedVertical[1]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() - tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::bedVertical[2]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedVertical[0]);
                }

                creature->setIsSleeping(false);
            }

            // beds horizontal
            if (object->getId() == tibia::SpriteData::bedHorizontal[0])
            {
                object->setId(tibia::SpriteData::bedHorizontal[2]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() + tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::bedHorizontal[1]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedHorizontal[3]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::bedHorizontal[1])
            {
                object->setId(tibia::SpriteData::bedHorizontal[3]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() - tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::bedHorizontal[0]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedHorizontal[2]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::bedHorizontal[2])
            {
                object->setId(tibia::SpriteData::bedHorizontal[0]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() + tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::bedHorizontal[3]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedHorizontal[1]);
                }

                creature->setIsSleeping(false);
            }
            else if (object->getId() == tibia::SpriteData::bedHorizontal[3])
            {
                object->setId(tibia::SpriteData::bedHorizontal[1]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() - tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::bedHorizontal[2]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::bedHorizontal[0]);
                }

                creature->setIsSleeping(false);
            }

            // stretchers vertical
            if (object->getId() == tibia::SpriteData::stretcherVertical[0])
            {
                object->setId(tibia::SpriteData::stretcherVertical[2]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() + tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::stretcherVertical[1]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherVertical[3]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::stretcherVertical[1])
            {
                object->setId(tibia::SpriteData::stretcherVertical[3]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() - tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::stretcherVertical[0]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherVertical[2]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::stretcherVertical[2])
            {
                object->setId(tibia::SpriteData::stretcherVertical[0]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() + tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::stretcherVertical[3]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherVertical[1]);
                }

                creature->setIsSleeping(false);
            }
            else if (object->getId() == tibia::SpriteData::stretcherVertical[3])
            {
                object->setId(tibia::SpriteData::stretcherVertical[1]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX(), object->getTileY() - tibia::TILE_SIZE),
                    creature->getZ(),
                    tibia::SpriteData::stretcherVertical[2]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherVertical[0]);
                }

                creature->setIsSleeping(false);
            }

            // stretchers horizontal
            if (object->getId() == tibia::SpriteData::stretcherHorizontal[0])
            {
                object->setId(tibia::SpriteData::stretcherHorizontal[2]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() + tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::stretcherHorizontal[1]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherHorizontal[3]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::stretcherHorizontal[1])
            {
                object->setId(tibia::SpriteData::stretcherHorizontal[3]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() - tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::stretcherHorizontal[0]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherHorizontal[2]);
                }

                creature->setIsSleeping(true);
            }
            else if (object->getId() == tibia::SpriteData::stretcherHorizontal[2])
            {
                object->setId(tibia::SpriteData::stretcherHorizontal[0]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() + tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::stretcherHorizontal[3]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherHorizontal[1]);
                }

                creature->setIsSleeping(false);
            }
            else if (object->getId() == tibia::SpriteData::stretcherHorizontal[3])
            {
                object->setId(tibia::SpriteData::stretcherHorizontal[1]);

                tibia::ObjectPtr nextObject = findTileObject
                (
                    sf::Vector2u(object->getTileX() - tibia::TILE_SIZE, object->getTileY()),
                    creature->getZ(),
                    tibia::SpriteData::stretcherHorizontal[2]
                );

                if (nextObject != nullptr)
                {
                    nextObject->setId(tibia::SpriteData::stretcherHorizontal[0]);
                }

                creature->setIsSleeping(false);
            }

            return true;
        }

        // blueberry bush // TODO: make blueberries restore health or hunger
        if (object->getId() == tibia::SpriteData::bushBlueberry)
        {
            object->setId(tibia::SpriteData::bush);

            return true;
        }

        // levers
        if (object->getType() == tibia::ObjectTypes::lever)
        {
            if (object->getId() == tibia::SpriteData::lever[0])
            {
                object->setId(tibia::SpriteData::lever[1]);
            }
            else if (object->getId() == tibia::SpriteData::lever[1])
            {
                object->setId(tibia::SpriteData::lever[0]);
            }

            return true;
        }

        // torches
        if (object->getId() == tibia::SpriteData::torchBig[0] || object->getId() == tibia::SpriteData::torchBig[1])
        {
            object->setId(tibia::SpriteData::torchBig[2]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchBig[2])
        {
            // TODO: pick up the torch

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchMedium[0] || object->getId() == tibia::SpriteData::torchMedium[1])
        {
            object->setId(tibia::SpriteData::torchMedium[2]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchMedium[2])
        {
            object->setId(tibia::SpriteData::torchMedium[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchSmall[0] || object->getId() == tibia::SpriteData::torchSmall[1])
        {
            object->setId(tibia::SpriteData::torchSmall[2]);

            return true;
        }

        // torch inset
        if (object->getId() == tibia::SpriteData::torchInsetHorizontal[0])
        {
            object->setId(tibia::SpriteData::torchInsetHorizontal[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchInsetHorizontal[1] || object->getId() == tibia::SpriteData::torchInsetHorizontal[2])
        {
            object->setId(tibia::SpriteData::torchInsetHorizontal[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchInsetVertical[0])
        {
            object->setId(tibia::SpriteData::torchInsetVertical[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchInsetVertical[1] || object->getId() == tibia::SpriteData::torchInsetVertical[2])
        {
            object->setId(tibia::SpriteData::torchInsetVertical[0]);

            return true;
        }

        // torch hanging
        if (object->getId() == tibia::SpriteData::torchHangingHorizontal[0])
        {
            object->setId(tibia::SpriteData::torchHangingHorizontal[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchHangingHorizontal[1] || object->getId() == tibia::SpriteData::torchHangingHorizontal[2])
        {
            object->setId(tibia::SpriteData::torchHangingHorizontal[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchHangingVertical[0])
        {
            object->setId(tibia::SpriteData::torchHangingVertical[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::torchHangingVertical[1] || object->getId() == tibia::SpriteData::torchHangingVertical[2])
        {
            object->setId(tibia::SpriteData::torchHangingVertical[0]);

            return true;
        }

        // candle stick
        if (object->getId() == tibia::SpriteData::candleStick[0])
        {
            object->setId(tibia::SpriteData::candleStick[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::candleStick[1] || object->getId() == tibia::SpriteData::candleStick[2])
        {
            object->setId(tibia::SpriteData::candleStick[0]);

            return true;
        }

        // candle holder
        if (object->getId() == tibia::SpriteData::candleHolder[0])
        {
            object->setId(tibia::SpriteData::candleHolder[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::candleHolder[1] || object->getId() == tibia::SpriteData::candleHolder[2])
        {
            object->setId(tibia::SpriteData::candleHolder[0]);

            return true;
        }

        // lantern
        if (object->getId() == tibia::SpriteData::lantern[0])
        {
            object->setId(tibia::SpriteData::lantern[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::lantern[1] || object->getId() == tibia::SpriteData::lantern[2])
        {
            object->setId(tibia::SpriteData::lantern[0]);

            return true;
        }

        // ovens
        if (object->getId() == tibia::SpriteData::ovenDown[0])
        {
            object->setId(tibia::SpriteData::ovenDown[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenDown[1] || object->getId() == tibia::SpriteData::ovenDown[2])
        {
            object->setId(tibia::SpriteData::ovenDown[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenLeft[0])
        {
            object->setId(tibia::SpriteData::ovenLeft[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenLeft[1] || object->getId() == tibia::SpriteData::ovenLeft[2])
        {
            object->setId(tibia::SpriteData::ovenLeft[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenUp[0])
        {
            object->setId(tibia::SpriteData::ovenUp[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenUp[1] || object->getId() == tibia::SpriteData::ovenUp[2])
        {
            object->setId(tibia::SpriteData::ovenUp[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenRight[0])
        {
            object->setId(tibia::SpriteData::ovenRight[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::ovenRight[1] || object->getId() == tibia::SpriteData::ovenRight[2])
        {
            object->setId(tibia::SpriteData::ovenRight[0]);

            return true;
        }

        // counter tops
        if (object->getId() == tibia::SpriteData::counterVertical[0])
        {
            tibia::TilePtr toTile = getTile(sf::Vector2u(object->getTilePosition().x, object->getTilePosition().y - tibia::TILE_SIZE), object->getZ());

            doMoveThingToTile(object, toTile, tibia::Directions::null, true);

            object->setId(tibia::SpriteData::counterVertical[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::counterVertical[1])
        {
            tibia::TilePtr toTile = getTile(sf::Vector2u(object->getTilePosition().x, object->getTilePosition().y + tibia::TILE_SIZE), object->getZ());

            // do not close the counter on solid objects or creatures
            tibia::Tile::TileProperties_t tileProperties = getTileProperties(toTile);
            if (tileProperties.hasSolidObject == true || tileProperties.hasSolidCreature == true)
            {
                return false;
            }

            doMoveThingToTile(object, toTile);

            object->setId(tibia::SpriteData::counterVertical[0]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::counterHorizontal[0])
        {
            tibia::TilePtr toTile = getTile(sf::Vector2u(object->getTilePosition().x - tibia::TILE_SIZE, object->getTilePosition().y), object->getZ());

            doMoveThingToTile(object, toTile, tibia::Directions::null, true);

            object->setId(tibia::SpriteData::counterHorizontal[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::counterHorizontal[1])
        {
            tibia::TilePtr toTile = getTile(sf::Vector2u(object->getTilePosition().x + tibia::TILE_SIZE, object->getTilePosition().y), object->getZ());

            // do not close the counter on solid objects or creatures
            tibia::Tile::TileProperties_t tileProperties = getTileProperties(toTile);
            if (tileProperties.hasSolidObject == true || tileProperties.hasSolidCreature == true)
            {
                return false;
            }

            doMoveThingToTile(object, toTile);

            object->setId(tibia::SpriteData::counterHorizontal[0]);

            return true;
        }

        return false;
    }

    void drawMouseCursor(sf::RenderWindow* mainWindow)
    {
        m_mouseCursor.setPosition(static_cast<sf::Vector2f>(getMouseWindowPosition()));
        m_mouseCursor.setColor(tibia::Colors::white);
        setMouseCursorType(tibia::MouseCursorTypes::point);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
        {
            setMouseCursorType(tibia::MouseCursorTypes::click);
        }

        if (m_mouseTile != nullptr)
        {
            tibia::ObjectList* objectList = m_mouseTile->getObjectList();

            for (auto& object : *objectList)
            {
                if (object->getFlags() & tibia::SpriteFlags::interactive)
                {
                    m_mouseCursor.setColor(tibia::Colors::mouseCursorPink);
                    setMouseCursorType(tibia::MouseCursorTypes::open);
                    break;
                }
            }
        }

        mainWindow->draw(m_mouseCursor);
    }

    bool isCreatureMovementOutOfBounds(tibia::CreaturePtr creature, int direction)
    {
        if (direction == tibia::Directions::up)
        {
            if (creature->getY() == 0)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::right)
        {
            if (creature->getX() == MAP_XY_MAX - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::down)
        {
            if (creature->getY() == MAP_XY_MAX - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::left)
        {
            if (creature->getX() == 0)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::upLeft)
        {
            if (creature->getY() == 0 || creature->getY() == 0)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::upRight)
        {
            if (creature->getY() == 0 || creature->getX() == MAP_XY_MAX - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::downLeft)
        {
            if (creature->getX() == 0 || creature->getY() == MAP_XY_MAX - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::downRight)
        {
            if (creature->getX() == MAP_XY_MAX - 1 || creature->getY() == MAP_XY_MAX - 1)
            {
                return true;
            }
        }

        return false;
    }

    void handleCreatureMovement(tibia::CreaturePtr creature, int direction, bool turnOnly = false)
    {
        if (direction == tibia::Directions::null)
        {
            return;
        }

        if (creature->isDead() == true || creature->isSleeping() == true)
        {
            return;
        }

        if (isCreatureMovementOutOfBounds(creature, direction) == true)
        {
            creature->doTurn(direction);
            return;
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

        if (turnOnly == true)
        {
            creature->doTurn(direction);
            return;
        }

        creature->doTurn(direction);

        tibia::TilePtr toTile = getThingTileByMovement(creature, direction);

        doMoveThingToTile(creature, toTile, direction);

        creature->setIsMovementReady(false);

        creature->getClockMovement()->restart();

        creature->update();
    }

    void updateStepTile(tibia::TilePtr tile)
    {
        tibia::ObjectList* objectList = tile->getObjectList();

        tibia::CreatureList* creatureList = tile->getCreatureList();

        bool tileHasThing = false;

        if (objectList->size() || creatureList->size())
        {
            tileHasThing = true;
        }

        if (tileHasThing == true)
        {
            // wood
            if (tile->getId() == tibia::SpriteData::stepTileWood[0])
            {
                tile->setId(tibia::SpriteData::stepTileWood[1]);
            }

            // stone
            else if (tile->getId() == tibia::SpriteData::stepTileStone[0])
            {
                tile->setId(tibia::SpriteData::stepTileStone[1]);
            }
        }
        else
        {
            // wood
            if (tile->getId() == tibia::SpriteData::stepTileWood[1])
            {
                tile->setId(tibia::SpriteData::stepTileWood[0]);
            }

            // stone
            else if (tile->getId() == tibia::SpriteData::stepTileStone[1])
            {
                tile->setId(tibia::SpriteData::stepTileStone[0]);
            }
        }
    }

    void updateTileHeight(tibia::TilePtr tile)
    {
        int height = 0;

        int numOffsetObjects = 0;

        tibia::ObjectList* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getFlags() & tibia::SpriteFlags::ignoreHeight)
            {
                object->setDrawOffset(0);
            }
            else
            {
                object->setDrawOffset(height + numOffsetObjects);
            }

            object->update();

            if (object->getFlags() & tibia::SpriteFlags::hasHeight)
            {
                height++;

                if (object->getFlags() & tibia::SpriteFlags::offset)
                {
                    numOffsetObjects++;
                }
            }
        }

        tibia::CreatureList* creatureList = tile->getCreatureList();

        for (auto& creature : *creatureList)
        {
            creature->setDrawOffset(height);
        }

        height = height - numOffsetObjects;

        tile->setHeight(height);
    }

    void updateMouseWindowPosition(sf::RenderWindow* mainWindow)
    {
        sf::Vector2i mouseWindowPosition = sf::Mouse::getPosition(*mainWindow);

        m_mouseWindowPosition = static_cast<sf::Vector2i>(mainWindow->mapPixelToCoords(mouseWindowPosition));
    }

    sf::Vector2i getMouseWindowPosition()
    {
        return m_mouseWindowPosition;
    }

    void updateMouseTile()
    {
        m_mouseTile = std::move(getTileUnderMouseCursor());
    }

    bool isMouseInsideGameWindow()
    {
        return tibia::GuiData::gameWindowRect.contains(getMouseWindowPosition());
    }

    bool isMouseInsideMiniMapWindow()
    {
        return tibia::GuiData::miniMapWindowRect.contains(getMouseWindowPosition());
    }

    sf::Vector2u getMouseTilePosition()
    {
        sf::Vector2u mouseTilePosition = static_cast<sf::Vector2u>(m_gameWindow.mapPixelToCoords(getMouseWindowPosition(), m_gameWindowView));

        // game window is offset
        mouseTilePosition.x -= tibia::GuiData::gameWindowX;
        mouseTilePosition.y -= tibia::GuiData::gameWindowY;

        // convert mouse tile position to actual tile position
        // round down to nearest multiple of tile size
        mouseTilePosition.x = mouseTilePosition.x - (mouseTilePosition.x % tibia::TILE_SIZE);
        mouseTilePosition.y = mouseTilePosition.y - (mouseTilePosition.y % tibia::TILE_SIZE);

        return mouseTilePosition;
    }

    tibia::TilePtr getTileUnderMouseCursor()
    {
        if (isMouseInsideGameWindow() == false)
        {
            return nullptr;
        }

        sf::Vector2u mouseTilePosition = getMouseTilePosition();

        if (tibia::Utility::isTilePositionOutOfBounds(mouseTilePosition) == true)
        {
            return nullptr;
        }

        return getTile(mouseTilePosition, m_player->getZ());
    }

    tibia::TilePtr getThingTile(tibia::ThingPtr thing)
    {
        tibia::TileList* tileList = m_map.tileMapTiles[thing->getZ()].getTileList();

        if (tileList->size() == 0)
        {
            return nullptr;
        }

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(thing->getTilePosition());

        return tileList->at(tileNumber);
    }

    tibia::TilePtr getThingTileByMovement(tibia::ThingPtr thing, int direction)
    {
        sf::Vector2i vecMovement = tibia::Utility::getVectorByDirection(direction);

        vecMovement.x = vecMovement.x * tibia::TILE_SIZE;
        vecMovement.y = vecMovement.y * tibia::TILE_SIZE;

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() + vecMovement.x, thing->getTileY() + vecMovement.y));
 
        return m_map.tileMapTiles[thing->getZ()].getTileList()->at(tileNumber);
    }

    void spawnObject(sf::Vector2u tileCoords, int z, int id)
    {
        tibia::ObjectPtr object = std::make_shared<tibia::Object>(tileCoords.x, tileCoords.y, z, id);

        tibia::TilePtr tile = getTile(tileCoords, z);

        tile->addObject(object);
    }

    void spawnDecayObject(sf::Vector2u tileCoords, int z, std::vector<int> id)
    {
        tibia::ObjectPtr object = std::make_shared<tibia::Object>(tileCoords.x, tileCoords.y, z, id[0]);

        object->setIsDecay(true);

        tibia::TilePtr tile = getTile(tileCoords, z);

        tibia::ObjectList* objectList = tile->getObjectList();

        if (objectList->size())
        {
            tibia::ObjectPtr topObject = objectList->back();

            if (topObject->isDecay() == true)
            {
                for (auto spriteId : id)
                {
                    if (spriteId == topObject->getId())
                    {
                        topObject->setIsReadyForErase(true);
                        break;
                    }
                }
            }
        }

        tile->addObject(object);
    }

    void spawnAnimation(sf::Vector2u tileCoords, int z, std::vector<int> animationId, float frameTime = tibia::AnimationTimes::default)
    {
        tibia::AnimationPtr animation = std::make_shared<tibia::Animation>(tileCoords.x, tileCoords.y, z, animationId[0], animationId[1]);

        animation->setFrameTime(frameTime);

        tibia::TilePtr tile = getTile(tileCoords, z);

        tile->addAnimation(animation);
    }

    void drawTileMap(tibia::TileMap* tileMap)
    {
        tibia::TileList* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return;
        }

        m_tileMapTileVertices.clear();

        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        x1 = x1 - NUM_TILES_FROM_CENTER_X - 1;
        y1 = y1 - NUM_TILES_FROM_CENTER_Y - 1;

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;

        int x2 = tibia::NUM_TILES_X + 2;
        int y2 = tibia::NUM_TILES_Y + 2;

        for (unsigned int i = x1; i < x1 + x2; i++)
        {
            for (unsigned int j = y1; j < y1 + y2; j++)
            {
                if (i > tibia::MAP_SIZE - 1) continue;
                if (j > tibia::MAP_SIZE - 1) continue;

                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tibia::Utility::isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::TilePtr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

                if (tileId == tibia::TILE_NULL)
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

                unsigned int tileFlags = tile->getFlags();

                if (tileFlags & tibia::SpriteFlags::transparent)
                {
                    quad[0].color = tibia::Colors::transparent;
                    quad[1].color = tibia::Colors::transparent;
                    quad[2].color = tibia::Colors::transparent;
                    quad[3].color = tibia::Colors::transparent;
                }

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

                        tileMapObjects->reserve(tileObjects->size());

                        std::copy(tileObjects->begin(), tileObjects->end(), std::back_inserter(*tileMapObjects));
                    }

                    tibia::CreatureList* tileCreatures = tile->getCreatureList();

                    if (tileCreatures->size())
                    {
                        tibia::CreatureList* tileMapCreatures = &m_tileMapCreatures[tileMap->getZ()];

                        tileMapCreatures->reserve(tileCreatures->size());

                        std::copy(tileCreatures->begin(), tileCreatures->end(), std::back_inserter(*tileMapCreatures));
                    }

                    tibia::AnimationList* tileAnimations = tile->getAnimationList();

                    if (tileAnimations->size())
                    {
                        tibia::AnimationList* tileMapAnimations = &m_tileMapAnimations[tileMap->getZ()];

                        tileMapAnimations->reserve(tileAnimations->size());

                        std::copy(tileAnimations->begin(), tileAnimations->end(), std::back_inserter(*tileMapAnimations));
                    }
                }
            }
        }

        sf::RenderStates states;
        states.texture = &tibia::Textures::sprites;

        m_gameWindowLayer.draw(m_tileMapTileVertices, states);
    }

    void updateTileThings(tibia::TileMap* tileMap)
    {
        tibia::TileList* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return;
        }

        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        x1 = x1 - NUM_TILES_FROM_CENTER_X - 1;
        y1 = y1 - NUM_TILES_FROM_CENTER_Y - 1;

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;

        int x2 = tibia::NUM_TILES_X + 2;
        int y2 = tibia::NUM_TILES_Y + 2;

        for (unsigned int i = x1; i < x1 + x2; i++)
        {
            for (unsigned int j = y1; j < y1 + y2; j++)
            {
                if (i > tibia::MAP_SIZE - 1) continue;
                if (j > tibia::MAP_SIZE - 1) continue;

                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tibia::Utility::isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::TilePtr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

                if (tileId == tibia::TILE_NULL)
                {
                    continue;
                }

                updateStepTile(tile);
                updateTileHeight(tile);

                tibia::CreatureList* tileCreatures = tile->getCreatureList();

                if (tileCreatures->size())
                {
                    for (auto& creatureIt = tileCreatures->begin(); creatureIt != tileCreatures->end(); creatureIt++)
                    {
                        tibia::Creature* creature = creatureIt->get();

                        creature->update();

                        if (creature->isReadyForErase() == true)
                        {
                            creatureIt = tileCreatures->erase(creatureIt);
                            creatureIt--;
                            continue;
                        }
                    }
                }

                //

                tibia::ObjectList* tileObjects = tile->getObjectList();

                if (tileObjects->size())
                {
                    for (auto& objectIt = tileObjects->begin(); objectIt != tileObjects->end(); objectIt++)
                    {
                        tibia::Object* object = objectIt->get();

                        if (object->isDecay() == true)
                        {
                            sf::Time timeDecayObject = object->getClockDecay()->getElapsedTime();

                            if (timeDecayObject.asSeconds() >= tibia::DecayObjects::time)
                            {
                                object->doDecay();
                            }
                        }

                        object->update();

                        if (object->isReadyForErase() == true)
                        {
                            objectIt = tileObjects->erase(objectIt);
                            objectIt--;
                            continue;
                        }

                        // fix drawing order of objects on walls
                        if (object->getFlags() & tibia::SpriteFlags::fixDrawOrder)
                        {
                            object->setTileX(object->getTileX() + tibia::TILE_SIZE);
                            object->setTileY(object->getTileY() + tibia::TILE_SIZE);
                            break;
                        }
                    }
                }

                //

                tibia::AnimationList* tileAnimations = tile->getAnimationList();

                if (tileAnimations->size())
                {
                    for (auto& animationIt = tileAnimations->begin(); animationIt != tileAnimations->end(); animationIt++)
                    {
                        tibia::Animation* animation = animationIt->get();

                        animation->update();

                        if (animation->isReadyForErase() == true)
                        {
                            animationIt = tileAnimations->erase(animationIt);
                            animationIt--;
                            continue;
                        }
                    }
                }
            }
        }
    }

    void drawThingList(tibia::ThingList* thingList)
    {
        std::stable_sort(thingList->begin(), thingList->end(), tibia::ThingSort::sortByTileCoords());

        for (auto& thing : *thingList)
        {
            //tibia::CreaturePtr creature = std::dynamic_pointer_cast<tibia::Creature>(thing);
            //if (creature != nullptr)
            //{
                //std::cout << "Creature Name: " << creature->getName() << std::endl;
                //break;
            //}

            m_gameWindowLayer.draw(*thing);
        }
    }

    void drawLightAtTilePosition(tibia::Light& light, sf::Vector2u tilePosition)
    {
        light.setPosition(tilePosition.x + (tibia::TILE_SIZE / 2), tilePosition.y + (tibia::TILE_SIZE / 2));

        m_lightLayer.draw(light, sf::BlendMode::BlendAdd);
    }

    void drawTileMapLights(int z)
    {
        if (z < tibia::ZAxis::ground)
        {
            m_lightLayer.clear(sf::Color(tibia::LightBrightnesses::underground, tibia::LightBrightnesses::underground, tibia::LightBrightnesses::underground));
        }
        else
        {
            m_lightLayer.clear(sf::Color(m_lightBrightness, m_lightBrightness, m_lightBrightness));
        }

        tibia::Light light;

        light.setSize(tibia::LightSizes::small);

        for (auto& creature : m_tileMapCreatures[z])
        {
            drawLightAtTilePosition(light, creature->getTilePosition());
        }

        light.setSize(tibia::LightSizes::medium);

        for (auto& object : m_tileMapObjects[z])
        {
            if (object->getFlags() & tibia::SpriteFlags::lightSource)
            {
                //light.setColorbyId(object->getId());

                drawLightAtTilePosition(light, object->getTilePosition());
            }

            if (z == tibia::ZAxis::ground - 1)
            {
                if (object->getId() == tibia::SpriteData::stairs || object->getId() == tibia::SpriteData::ladder || object->getId() == tibia::SpriteData::ropeUp)
                {
                    light.setColor(tibia::Colors::light);

                    drawLightAtTilePosition(light, object->getTilePosition());
                }
            }
        }

        light.setSize(tibia::LightSizes::tiny);

        for (auto& animation : m_tileMapAnimations[z])
        {
            if (animation->getFlags() & tibia::SpriteFlags::lightSource)
            {
                drawLightAtTilePosition(light, animation->getTilePosition());
            }
        }

        m_lightLayer.display();

        m_lightLayerSprite.setTexture(m_lightLayer.getTexture());

        m_gameWindowLayer.draw(m_lightLayerSprite, sf::BlendMode::BlendMultiply);
    }

    void drawGameLayer(int z)
    {
        m_gameWindowLayer.clear(tibia::Colors::transparent);

        drawTileMap(&m_map.tileMapTiles[z]);
        drawTileMap(&m_map.tileMapTileEdges[z]);

        // this only updates things being drawn
        updateTileThings(&m_map.tileMapTiles[z]);

        tibia::ThingList* tileMapThings = &m_tileMapThings[z];

        unsigned int reserveThingsSize = m_tileMapObjects[z].size() + m_tileMapCreatures[z].size() + m_tileMapAnimations[z].size();

        tileMapThings->reserve(reserveThingsSize);

        std::copy(m_tileMapObjects[z].begin(),    m_tileMapObjects[z].end(),    std::back_inserter(*tileMapThings));
        std::copy(m_tileMapCreatures[z].begin(),  m_tileMapCreatures[z].end(),  std::back_inserter(*tileMapThings));
        std::copy(m_tileMapAnimations[z].begin(), m_tileMapAnimations[z].end(), std::back_inserter(*tileMapThings));

        drawThingList(&m_tileMapThings[z]);

        if (z < tibia::ZAxis::ground || m_lightBrightness != tibia::LightBrightnesses::max) // && z == m_player->getZ())
        {
            drawTileMapLights(z);
        }

        m_gameWindowLayer.display();

        m_gameWindowLayerSprite.setTexture(m_gameWindowLayer.getTexture());

        m_gameWindow.draw(m_gameWindowLayerSprite);

        doCreatureLogic(z);
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
            m_tileMapAnimations[i].clear();
            m_tileMapThings[i].clear();

            //updateTileThings(&m_map.tileMapTiles[i]);
        }

        if (m_player->getZ() < tibia::ZAxis::ground)
        {
            drawGameLayer(tibia::ZAxis::floor);

            if (findTilesAbovePlayer(&m_map.tileMapTiles[tibia::ZAxis::underGround]) == false)
            {
                if (isTileMapVisible(&m_map.tileMapTiles[tibia::ZAxis::underGround]) == true)
                {
                    drawGameLayer(tibia::ZAxis::underGround);
                }
            }
        }
        else
        {
            drawGameLayer(tibia::ZAxis::ground);

            if (findTilesAbovePlayer(&m_map.tileMapTiles[tibia::ZAxis::aboveGround]) == false)
            {
                if (isTileMapVisible(&m_map.tileMapTiles[tibia::ZAxis::aboveGround]) == true)
                {
                    drawGameLayer(tibia::ZAxis::aboveGround);
                }

                if (findTilesAbovePlayer(&m_map.tileMapTiles[tibia::ZAxis::ceiling]) == false)
                {
                    if (isTileMapVisible(&m_map.tileMapTiles[tibia::ZAxis::ceiling]) == true)
                    {
                        drawGameLayer(tibia::ZAxis::ceiling);
                    }
                }
            }
        }

        drawFloatingTextList();
        drawGameTextList();

        m_gameWindow.display();

        m_gameWindowSprite.setTexture(m_gameWindow.getTexture());
        m_gameWindowSprite.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        mainWindow->draw(m_gameWindowSprite);

        drawStatusBarText(mainWindow);
    }

    void updateMiniMapWindow()
    {
        m_miniMapWindow.clear(tibia::Colors::black);

        m_miniMapVertices.clear();

        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        x1 = x1 - (NUM_TILES_FROM_CENTER_X * m_miniMapWindowZoom) - 1;
        y1 = y1 - (NUM_TILES_FROM_CENTER_Y * m_miniMapWindowZoom) - 1;

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;

        int x2 = (tibia::NUM_TILES_X * m_miniMapWindowZoom) + 1;
        int y2 = (tibia::NUM_TILES_Y * m_miniMapWindowZoom) + 1;

        m_map.tileMapTiles[m_player->getZ()].addMiniMapTiles(m_miniMapVertices, x1, y1, x2, y2);

        sf::Vertex quadPlayer[4];

        quadPlayer[0].position = sf::Vector2f(m_player->getTileX(),                    m_player->getTileY());
        quadPlayer[1].position = sf::Vector2f(m_player->getTileX() + tibia::TILE_SIZE, m_player->getTileY());
        quadPlayer[2].position = sf::Vector2f(m_player->getTileX() + tibia::TILE_SIZE, m_player->getTileY() + tibia::TILE_SIZE);
        quadPlayer[3].position = sf::Vector2f(m_player->getTileX(),                    m_player->getTileY() + tibia::TILE_SIZE);

        quadPlayer[0].color = tibia::Colors::white;
        quadPlayer[1].color = tibia::Colors::white;
        quadPlayer[2].color = tibia::Colors::white;
        quadPlayer[3].color = tibia::Colors::white;

        m_miniMapVertices.push_back(quadPlayer[0]);
        m_miniMapVertices.push_back(quadPlayer[1]);
        m_miniMapVertices.push_back(quadPlayer[2]);
        m_miniMapVertices.push_back(quadPlayer[3]);

        m_miniMapWindow.draw(&m_miniMapVertices[0], m_miniMapVertices.size(), sf::Quads);
    }

    void drawMiniMapWindow(sf::RenderWindow* mainWindow)
    {
        m_miniMapWindowView.setCenter
        (
            m_player->getTileX() + (tibia::TILE_SIZE / 2),
            m_player->getTileY() + (tibia::TILE_SIZE / 2)
        );

        m_miniMapWindow.setView(m_miniMapWindowView);

        m_miniMapWindow.display();

        m_miniMapWindowSprite.setTexture(m_miniMapWindow.getTexture());
        m_miniMapWindowSprite.setPosition(tibia::GuiData::miniMapWindowX, tibia::GuiData::miniMapWindowY);

        mainWindow->draw(m_miniMapWindowSprite);
    }

    void updateLightBrightness()
    {
        sf::Time elapsedTime = clockLightBrightness.getElapsedTime();

        if (elapsedTime.asMilliseconds() > tibia::LightBrightnesses::updateTime * m_timeDelta.asMilliseconds())
        {
            clockLightBrightness.restart();
        }
        else
        {
            return;
        }

        if (m_timeOfDay == tibia::TimeOfDay::day)
        {
            if (m_lightBrightness < tibia::LightBrightnesses::day)
            {
                m_lightBrightness++;
            }
        }
        else if (m_timeOfDay == tibia::TimeOfDay::night)
        {
            if (m_lightBrightness > tibia::LightBrightnesses::night)
            {
                m_lightBrightness--;
            }
        }
    }

    void spawnGameText(const sf::Vector2u& tilePosition, int z, const std::string& text, sf::Color textColor)
    {
        tibia::GameText gt;
        gt.setText(m_bitmapFontDefault, tilePosition, z, text, textColor);

        if (z != m_player->getZ())
        {
            return;
        }

        m_gameTextList.push_back(gt);
    }

    void drawGameTextList()
    {
        for (auto& gameTextIt = m_gameTextList.begin(); gameTextIt != m_gameTextList.end(); gameTextIt++)
        {
            if (gameTextIt->getZ() != m_player->getZ())
            {
                continue;
            }

            sf::Clock* clock = gameTextIt->getClock();

            sf::Time elapsedTime = clock->getElapsedTime();

            float displayTime = tibia::GameTextData::time;

            if (gameTextIt->getNumTextLines() > 3)
            {
                displayTime *= 2;
            }

            float textTileDistance = calculateDistance
            (
                gameTextIt->getTilePosition().x,
                gameTextIt->getTilePosition().y,
                m_player->getTilePosition().x,
                m_player->getTilePosition().y
            );

            bool textIsOffScreen = false;

            if (textTileDistance > (tibia::NUM_TILES_X * tibia::TILE_SIZE))
            {
                textIsOffScreen = true;
            }

            if (elapsedTime.asSeconds() > displayTime || textIsOffScreen == true)
            {
                gameTextIt = m_gameTextList.erase(gameTextIt);
                gameTextIt--;
                continue;
            }

            m_gameWindow.draw(*gameTextIt);
        }
    }

    void spawnFloatingText(const sf::Vector2u& tilePosition, int z, const std::string& text, sf::Color textColor)
    {
        tibia::FloatingText ft;
        ft.setText(m_bitmapFontModern, tilePosition, z, text, textColor);

        if (z != m_player->getZ())
        {
            return;
        }

        m_floatingTextList.push_back(ft);
    }

    void drawFloatingTextList()
    {
        for (auto& floatingTextIt = m_floatingTextList.begin(); floatingTextIt != m_floatingTextList.end(); floatingTextIt++)
        {
            if (floatingTextIt->getZ() != m_player->getZ())
            {
                continue;
            }

            sf::Clock* clock = floatingTextIt->getClock();

            sf::Time elapsedTime = clock->getElapsedTime();

            if (elapsedTime.asSeconds() > tibia::FloatingTextData::time)
            {
                floatingTextIt = m_floatingTextList.erase(floatingTextIt);
                floatingTextIt--;
                continue;
            }

            floatingTextIt->update(m_timeDelta);

            m_gameWindow.draw(*floatingTextIt);
        }
    }

    void showStatusBarText(const std::string& text)
    {
        m_statusBarText.setText(&m_bitmapFontModern, text, tibia::Colors::textWhite, true);

        m_clockStatusBar.restart();
    }

    void drawStatusBarText(sf::RenderWindow* mainWindow)
    {
        sf::Time elapsedTime = m_clockStatusBar.getElapsedTime();

        if (elapsedTime.asSeconds() < tibia::StatusBarTextData::time)
        {
            mainWindow->draw(m_statusBarText);
        }
    }

    bool isTileMapVisible(tibia::TileMap* tileMap)
    {
        tibia::TileList* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return false;
        }

        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        x1 = x1 - NUM_TILES_FROM_CENTER_X - 1;
        y1 = y1 - NUM_TILES_FROM_CENTER_Y - 1;

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;

        int x2 = tibia::NUM_TILES_X + 2;
        int y2 = tibia::NUM_TILES_Y + 2;

        for (unsigned int i = x1; i < x1 + x2; i++)
        {
            for (unsigned int j = y1; j < y1 + y2; j++)
            {
                if (i > tibia::MAP_SIZE - 1) continue;
                if (j > tibia::MAP_SIZE - 1) continue;

                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tibia::Utility::isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::TilePtr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

                if (tileId != tibia::TILE_NULL)
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool findTilesAbovePlayer(tibia::TileMap* tileMap)
    {
        tibia::TileList* tileList = tileMap->getTileList();

        std::vector<int> nearbyTileNumbers;

        //for (int i = -2; i < 3; i++)
        for (int i = -2; i < 2; i++)
        {
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() - (2 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() - (1 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX()                         , m_player->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() + (1 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
            //nearbyTileNumbers.push_back(tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(m_player->getTileX() + (2 * tibia::TILE_SIZE), m_player->getTileY() + (i * tibia::TILE_SIZE))));
        }

        for (auto tileNumber : nearbyTileNumbers)
        {
            if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
            {
                continue;
            }

            tibia::TilePtr tile = tileList->at(tileNumber);

            if (tile->getId() != tibia::TILE_NULL)
            {
                if (tile->getZ() > m_player->getZ())
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool isWaterVisible()
    {
        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        x1 = x1 - NUM_TILES_FROM_CENTER_X;
        y1 = y1 - NUM_TILES_FROM_CENTER_Y;

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;

        int x2 = tibia::NUM_TILES_X;
        int y2 = tibia::NUM_TILES_Y;

        for (unsigned int i = x1; i < x1 + x2; i++)
        {
            for (unsigned int j = y1; j < y1 + y2; j++)
            {
                if (i > tibia::MAP_SIZE - 1) continue;
                if (j > tibia::MAP_SIZE - 1) continue;

                int tileNumber = i + j * tibia::MAP_SIZE;

                if (tibia::Utility::isTileNumberOutOfBounds(tileNumber) == true)
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

        for (auto& object : *objectList)
        {
            if (object->isAnimated() == true)
            {
                object->doAnimation();
            }
        }
    }

    void toggleTimeOfDay()
    {
        if (m_timeOfDay == tibia::TimeOfDay::night)
        {
            setTimeOfDay(tibia::TimeOfDay::day);
        }
        else if (m_timeOfDay == tibia::TimeOfDay::day)
        {
            setTimeOfDay(tibia::TimeOfDay::night);
        }
    }

    sf::Clock* getClockDelta()
    {
        return &m_clockDelta;
    }

    void setTimeDelta(sf::Time time)
    {
        m_timeDelta = time;
    }

    sf::Clock* getClockGame()
    {
        return &m_clockGame;
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

    tibia::CreaturePtr getPlayer()
    {
        return m_player;
    }

    tibia::BitmapFont* getBitmapFontDefault()
    {
        return &m_bitmapFontDefault;
    }

    void setTimeOfDay(int timeOfDay)
    {
        m_timeOfDay = timeOfDay;
    }

    int getTimeOfDay()
    {
        return m_timeOfDay;
    }

private:

    sf::Clock m_clockDelta;
    sf::Time m_timeDelta;

    sf::Clock m_clockGame;
    sf::Clock m_clockAnimatedWaterAndObjects;

    sf::Sprite m_mouseCursor;

    sf::Vector2i m_mouseWindowPosition;

    tibia::TilePtr m_mouseTile;

    sf::Font m_fontDefault;

    tibia::BitmapFont m_bitmapFontDefault;
    tibia::BitmapFont m_bitmapFontTiny;
    tibia::BitmapFont m_bitmapFontModern;

    std::vector<tibia::GameText> m_gameTextList;

    std::vector<tibia::FloatingText> m_floatingTextList;

    sf::Clock m_clockStatusBar;
    tibia::BitmapFontText m_statusBarText;

    sf::View m_gameWindowView;
    sf::RenderTexture m_gameWindow;
    sf::Sprite m_gameWindowSprite;
    sf::RenderTexture m_gameWindowLayer;
    sf::Sprite m_gameWindowLayerSprite;

    sf::View m_miniMapWindowView;
    sf::RenderTexture m_miniMapWindow;
    sf::Sprite m_miniMapWindowSprite;

    int m_miniMapWindowZoom;

    sf::RenderTexture m_lightLayer;
    sf::Sprite m_lightLayerSprite;

    unsigned int m_lightBrightness;

    int m_timeOfDay;

    sf::Clock clockLightBrightness;

    tibia::Map m_map;

    std::vector<sf::Vertex> m_miniMapVertices;

    sf::VertexArray m_tileMapTileVertices;

    tibia::ObjectList    m_tileMapObjects[tibia::NUM_Z_LEVELS];
    tibia::CreatureList  m_tileMapCreatures[tibia::NUM_Z_LEVELS];
    tibia::AnimationList m_tileMapAnimations[tibia::NUM_Z_LEVELS];
    tibia::ThingList     m_tileMapThings[tibia::NUM_Z_LEVELS];

    tibia::CreaturePtr m_player;
};

} // tibia

#endif // TIBIA_GAME_HPP

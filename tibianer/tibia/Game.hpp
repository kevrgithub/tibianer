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

        m_lightLayer.create(MAP_TILE_XY_MAX, MAP_TILE_XY_MAX);

        m_lightLayerRect.setPosition(0, 0);
        m_lightLayerRect.setSize(sf::Vector2f(MAP_TILE_XY_MAX, MAP_TILE_XY_MAX));
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
        tibia::CreaturePtr player = std::make_shared<tibia::Creature>(10, 10, tibia::ZAxis::ground);

        player->setName("Player");
        player->setIsPlayer(true);
        player->setHasOutfit(true);
        player->setTeam(tibia::Teams::good);
        player->setHpMax(10000);
        player->setHp(10000);
        player->setMovementSpeed(tibia::MovementSpeeds::player);

        m_player = std::move(player);

        tibia::TilePtr tile = getCreatureTile(m_player);

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
            tibia::spriteFlags[i] = tibia::Utility::getSpriteFlags(i);
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
                spawnObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::magicWall[0], true);
                break;

            case sf::Keyboard::P:
                spawnObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::poolRed[0], true);
                break;

            case sf::Keyboard::T:
                spawnObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::torchBig[0], true);
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

    void doPlayerInteractWithPlayerTileObjects()
    {
        tibia::TilePtr playerTile = getPlayerTile();

        if (playerTile != nullptr)
        {
        if (playerTile->getObjectList()->size())
        {
            doCreatureInteractWithTileObjects(m_player, playerTile);
        }
        else
        {
            doCreatureInteractWithTileObjects(m_player, getTileByCreatureDirection(m_player));
        }
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

    tibia::TilePtr getTileByCreatureDirection(tibia::CreaturePtr creature)
    {
        sf::Vector2u position = creature->getTilePosition();

        sf::Vector2i normal = tibia::Utility::getVectorByDirection(creature->getDirection());

        position.x += normal.x * tibia::TILE_SIZE;
        position.y += normal.y * tibia::TILE_SIZE;

        return getTile(position, creature->getZ());
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

        for (auto object : *objectList)
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

        for (auto object : *objectList)
        {
            if (object->getId() == id)
            {
                return object;
            }
        }

        return nullptr;
    }

    bool doCreatureMoveToTile(tibia::CreaturePtr creature, tibia::TilePtr toTile, int direction)
    {
        tibia::TilePtr fromTile = getCreatureTile(creature);

        if (fromTile == nullptr || toTile == nullptr || toTile->getId() == tibia::TILE_NULL)
        {
            return false;
        }

        tibia::CreatureList* creatureList = fromTile->getCreatureList();

        auto creatureIt = std::find(creatureList->begin(), creatureList->end(), creature);

        if (creatureIt != creatureList->end())
        {
            bool tileIsSolid     = false;
            bool tileIsMoveAbove = false;
            bool tileIsMoveBelow = false;

            if (toTile->getFlags() & tibia::SpriteFlags::solid)
            {
                tileIsSolid = true;
            }

            if (toTile->getFlags() & tibia::SpriteFlags::moveAbove)
            {
                tileIsMoveAbove = true;
            }

            if (toTile->getFlags() & tibia::SpriteFlags::moveBelow)
            {
                tileIsMoveBelow = true;
            }

            bool tileHasSolidObject      = false;
            bool tileHasChairObject      = false;
            bool tileHasMoveAboveObject  = false;
            bool tileHasMoveBelowObject  = false;
            bool tileHasTeleporterObject = false;

            tibia::ObjectPtr teleporterObject;

            tibia::ObjectList* objectList = toTile->getObjectList();

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

                if (object->getFlags() & tibia::SpriteFlags::moveAbove)
                {
                    tileHasMoveAboveObject = true;
                }

                if (object->getFlags() & tibia::SpriteFlags::moveBelow)
                {
                    tileHasMoveBelowObject = true;
                }

                if (object->getType() == tibia::ObjectTypes::teleporter)
                {
                    tileHasTeleporterObject = true;

                    teleporterObject = object;
                }
            }

            if (tileIsSolid == false && tileHasSolidObject == false)
            {
                int toZ = toTile->getZ();

                if (tileIsMoveAbove == true || tileHasMoveAboveObject == true)
                {
                    toZ = toZ + 1;

                    toTile = getTileForMoveAbove(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y), toZ);

                    direction = tibia::Directions::up;
                }
                else if (tileIsMoveBelow == true || tileHasMoveBelowObject == true)
                {
                    toZ = toZ - 1;

                    updateStepTile(toTile); // update tile for falling in grass hole

                    toTile = getTileForMoveBelow(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y), toZ);

                    direction = tibia::Directions::down;
                }
                else if (tileHasTeleporterObject == true)
                {
                    spawnAnimation(creature->getTilePosition(), creature->getZ(), tibia::Animations::spellBlue);

                    toZ = teleporterObject->getTeleporterZ();

                    sf::Vector2u teleporterPosition
                    (
                        teleporterObject->getTeleporterX() * tibia::TILE_SIZE,
                        teleporterObject->getTeleporterY() * tibia::TILE_SIZE
                    );

                    toTile = getTile(teleporterPosition, toZ);

                    direction = tibia::Directions::down;

                    spawnAnimation(toTile->getPosition(), toZ, tibia::Animations::spellBlue);
                }

                toTile->addCreature(creature);

                creatureList->erase(creatureIt);

                updateStepTile(fromTile);
                updateStepTile(toTile);

                creature->setIsSitting(tileHasChairObject);

                creature->setTileCoords(toTile->getPosition().x, toTile->getPosition().y);

                creature->setZ(toZ);

                creature->doTurn(direction);

                return true;
            }
        }

        return false;
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
            for (auto object : *objectList)
            {
                if (doCreatureInteractWithObject(creature, object) == true)
                {
                    break;
                }
            }
        }
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

        // signs
        if (object->getType() == tibia::ObjectTypes::sign)
        {
            std::stringstream signText;
            signText << "You see a sign. It reads:\n" << object->getSignText();

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

            // door locked vertical // TODO: make locked doors require keys
            if (object->getId() == tibia::SpriteData::doorLockedVertical[0])
            {
                object->setId(tibia::SpriteData::doorLockedVertical[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorLockedVertical[1])
            {
                object->setId(tibia::SpriteData::doorLockedVertical[0]);
            }

            // door locked horizontal // TODO: make locked doors require keys
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

        // ladder and rope up // TODO: make rope up require a rope
        if (object->getId() == tibia::SpriteData::ladder || object->getId() == tibia::SpriteData::ropeUp)
        {
            tibia::TilePtr toTile = getTileForMoveAbove(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() + 1);

            doCreatureMoveToTile(creature, toTile, tibia::Directions::up);

            return true;
        }

        // sewer grate
        if (object->getId() == tibia::SpriteData::sewerGrate)
        {
            tibia::TilePtr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() - 1);

            doCreatureMoveToTile(creature, toTile, tibia::Directions::down);

            return true;
        }

        // dig holes // TODO: require shovel or pickaxe to dig
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

            // fall down the hole if creature digs while standing on top of it
            if (creature->getTilePosition() == object->getTilePosition())
            {
                tibia::TilePtr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() - 1);

                doCreatureMoveToTile(creature, toTile, tibia::Directions::down);
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

        return false;
    }

    void drawMouseCursor(sf::RenderWindow* mainWindow)
    {
        //
        //static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mainWindow))

        m_mouseCursor.setPosition(static_cast<sf::Vector2f>(getMouseWindowPosition()));
        m_mouseCursor.setColor(tibia::Colors::white);

        if (m_mouseTile != nullptr)
        {
            tibia::ObjectList* objectList = m_mouseTile->getObjectList();

            for (auto object : *objectList)
            {
                if (object->getFlags() & tibia::SpriteFlags::interactive)
                {
                    m_mouseCursor.setColor(tibia::Colors::mousePink);
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

        tibia::TilePtr toTile = getCreatureTileByMovement(creature, direction);

        doCreatureMoveToTile(creature, toTile, direction);

        creature->setIsMovementReady(false);

        creature->getClockMovement()->restart();

        creature->update();
    }

    void updateStepTile(tibia::TilePtr tile)
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

        // grass hole
        else if (tile->getId() == tibia::SpriteData::stepTileGrassHole[0])
        {
            tile->setId(tibia::SpriteData::stepTileGrassHole[1]);

            tile->setFlags(tibia::spriteFlags[tibia::SpriteData::stepTileGrassHole[1]]);
        }
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

        if (mouseTilePosition.x < 0) mouseTilePosition.x = 0;
        if (mouseTilePosition.y < 0) mouseTilePosition.y = 0;

        // game window is offset
        mouseTilePosition.x -= tibia::GuiData::gameWindowX;
        mouseTilePosition.y -= tibia::GuiData::gameWindowY;

        // convert mouse tile position to actual tile position
        while (mouseTilePosition.x > 0 && mouseTilePosition.x % tibia::TILE_SIZE != 0) mouseTilePosition.x--;
        while (mouseTilePosition.y > 0 && mouseTilePosition.y % tibia::TILE_SIZE != 0) mouseTilePosition.y--;

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

    tibia::TilePtr getCreatureTile(tibia::CreaturePtr creature)
    {
        tibia::TileList* tileList = m_map.tileMapTiles[creature->getZ()].getTileList();

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(creature->getTilePosition());

        return tileList->at(tileNumber);
    }

    tibia::TilePtr getCreatureTileByMovement(tibia::CreaturePtr creature, int direction)
    {
        sf::Vector2i vecMovement = tibia::Utility::getVectorByDirection(direction);

        vecMovement.x = vecMovement.x * tibia::TILE_SIZE;
        vecMovement.y = vecMovement.y * tibia::TILE_SIZE;

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(creature->getTileX() + vecMovement.x, creature->getTileY() + vecMovement.y));
 
        return m_map.tileMapTiles[creature->getZ()].getTileList()->at(tileNumber);
    }

    void spawnObject(sf::Vector2u tileCoords, int z, int id, bool isDecay = false)
    {
        tibia::ObjectPtr object = std::make_shared<tibia::Object>(tileCoords.x, tileCoords.y, z, id);

        object->setIsDecay(isDecay);

        tibia::TilePtr tile = getTile(tileCoords, z);

        tile->addObject(object);
    }

    void spawnAnimation(sf::Vector2u tileCoords, int z, int animationId[], float frameTime = tibia::AnimationTimes::default)
    {
        tibia::AnimationPtr animation = std::make_shared<tibia::Animation>(tileCoords.x, tileCoords.y, z, animationId[0], animationId[1]);

        animation->setFrameTime(frameTime);

        if
        (
            animationId[0] == tibia::Animations::fire[0]        ||
            animationId[0] == tibia::Animations::electricity[0]
        )
        {
            animation->setNumRepeat(1);
        }

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

        int x = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE) - NUM_TILES_FROM_CENTER_X - 1; // - 1 to draw one tile offscreen
        int y = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE) - NUM_TILES_FROM_CENTER_Y - 1; // - 1 to draw one tile offscreen

        if (x < 0) x = 0;
        if (y < 0) y = 0;

        m_tileMapTileVertices.clear();

        for (unsigned int i = x; i < x + tibia::NUM_TILES_X + 2; i++) // + 2 to draw one tile offscreen
        {
            for (unsigned int j = y; j < y + tibia::NUM_TILES_Y + 2; j++) // + 2 to draw one tile offscreen
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

                if (tileId == tibia::TILE_NULL)// || tileId == 1)
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

                if (tileId == 1)
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

                        std::copy(tileObjects->begin(), tileObjects->end(), std::back_inserter(*tileMapObjects));
                    }

                    tibia::CreatureList* tileCreatures = tile->getCreatureList();

                    if (tileCreatures->size())
                    {
                        tibia::CreatureList* tileMapCreatures = &m_tileMapCreatures[tileMap->getZ()];

                        std::copy(tileCreatures->begin(), tileCreatures->end(), std::back_inserter(*tileMapCreatures));
                    }

                    tibia::AnimationList* tileAnimations = tile->getAnimationList();

                    if (tileAnimations->size())
                    {
                        tibia::AnimationList* tileMapAnimations = &m_tileMapAnimations[tileMap->getZ()];

                        std::copy(tileAnimations->begin(), tileAnimations->end(), std::back_inserter(*tileMapAnimations));
                    }
                }
            }
        }

        sf::RenderStates states;
        states.texture = &tibia::Textures::sprites;

        m_gameWindow.draw(m_tileMapTileVertices, states);
    }

    void updateTileThings(tibia::TileMap* tileMap)
    {
        tibia::TileList* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return;
        }

        for (auto tile : *tileList)
        {
            tibia::CreatureList* tileCreatures = tile->getCreatureList();

            if (tileCreatures->size())
            {
                for (auto creatureIt = tileCreatures->begin(); creatureIt != tileCreatures->end(); creatureIt++)
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

            tibia::ObjectList* tileObjects = tile->getObjectList();

            if (tileObjects->size())
            {
                for (auto objectIt = tileObjects->begin(); objectIt != tileObjects->end(); objectIt++)
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
                    for (auto spriteId : tibia::SpriteData::fixDrawOrderObjects)
                    {
                        if (object->getId() == spriteId)
                        {
                            object->setTileX(object->getTileX() + tibia::TILE_SIZE);
                            object->setTileY(object->getTileY() + tibia::TILE_SIZE);
                            break;
                        }
                    }
                }
            }

            tibia::AnimationList* tileAnimations = tile->getAnimationList();

            if (tileAnimations->size())
            {
                for (auto animationIt = tileAnimations->begin(); animationIt != tileAnimations->end(); animationIt++)
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

    void addObjectListToThingList(tibia::ObjectList* objectList)
    {
        for (auto object : *objectList)
        {
            tibia::ThingList* tileMapThings = &m_tileMapThings[object->getZ()];

            tileMapThings->push_back(object);
        }
    }

    void addCreatureListToThingList(tibia::CreatureList* creatureList)
    {
        for (auto creature : *creatureList)
        {
            tibia::ThingList* tileMapThings = &m_tileMapThings[creature->getZ()];

            tileMapThings->push_back(creature);
        }
    }

    void addAnimationListToThingList(tibia::AnimationList* animationList)
    {
        for (auto animation : *animationList)
        {
            tibia::ThingList* tileMapThings = &m_tileMapThings[animation->getZ()];

            tileMapThings->push_back(animation);
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

    void drawLightAtTilePosition(tibia::Light& light, sf::Vector2u tilePosition)
    {
        light.setPosition(tilePosition.x + (tibia::TILE_SIZE / 2), tilePosition.y + (tibia::TILE_SIZE / 2));

        m_lightLayer.draw(light, sf::BlendMode::BlendAdd);
    }

    void drawTileMapLights(int z)
    {
        m_lightLayer.clear(tibia::Colors::black);

        tibia::Light light;

        light.setSize(3);

        drawLightAtTilePosition(light, m_player->getTilePosition());

        light.setSize(2);

        for (auto object : m_tileMapObjects[z])
        {
            if (object->getFlags() & tibia::SpriteFlags::light)
            {
                drawLightAtTilePosition(light, object->getTilePosition());
            }
        }

        m_lightLayer.display();

        m_lightLayerRect.setTexture(&m_lightLayer.getTexture());

        m_gameWindow.draw(m_lightLayerRect, sf::BlendMode::BlendMultiply);
    }

    void drawGameLayer(int z)
    {
        drawTileMap(&m_map.tileMapTiles[z]);
        drawTileMap(&m_map.tileMapTileEdges[z]);

        // this only updated things being drawn, moved to draw everything
        //updateTileThings(&m_map.tileMapTiles[z]);

        addObjectListToThingList(&m_tileMapObjects[z]);
        addCreatureListToThingList(&m_tileMapCreatures[z]);
        addAnimationListToThingList(&m_tileMapAnimations[z]);

        drawThingList(&m_tileMapThings[z]);

        if (z < tibia::ZAxis::ground && z == m_player->getZ())
        {
            drawTileMapLights(z);
        }
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

            updateTileThings(&m_map.tileMapTiles[i]);
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

        m_map.tileMapTiles[m_player->getZ()].addMiniMapTiles(m_miniMapVertices);

        sf::Vertex quadPlayer[4];

        quadPlayer[0].position = sf::Vector2f(m_player->getTileX(),                    m_player->getTileY());
        quadPlayer[1].position = sf::Vector2f(m_player->getTileX() + tibia::TILE_SIZE, m_player->getTileY());
        quadPlayer[2].position = sf::Vector2f(m_player->getTileX() + tibia::TILE_SIZE, m_player->getTileY() + tibia::TILE_SIZE);
        quadPlayer[3].position = sf::Vector2f(m_player->getTileX(),                    m_player->getTileY() + tibia::TILE_SIZE);

        quadPlayer[0].color = tibia::Colors::pink;
        quadPlayer[1].color = tibia::Colors::pink;
        quadPlayer[2].color = tibia::Colors::pink;
        quadPlayer[3].color = tibia::Colors::pink;

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
        for (auto gameTextIt = m_gameTextList.begin(); gameTextIt != m_gameTextList.end(); gameTextIt++)
        {
            if (gameTextIt->getZ() != m_player->getZ())
            {
                continue;
            }

            sf::Clock* clock = gameTextIt->getClock();

            sf::Time elapsedTime = clock->getElapsedTime();

            if (elapsedTime.asSeconds() > tibia::GameTextData::time)
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
        for (auto floatingTextIt = m_floatingTextList.begin(); floatingTextIt != m_floatingTextList.end(); floatingTextIt++)
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

            floatingTextIt->update();

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

    tibia::CreaturePtr getPlayer()
    {
        return m_player;
    }

    tibia::BitmapFont* getBitmapFontDefault()
    {
        return &m_bitmapFontDefault;
    }

private:

    sf::Clock m_clock;
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

    sf::RenderTexture m_gameWindow;
    sf::View m_gameWindowView;
    sf::Sprite m_gameWindowSprite;

    sf::RenderTexture m_miniMapWindow;
    sf::View m_miniMapWindowView;
    sf::Sprite m_miniMapWindowSprite;

    int m_miniMapWindowZoom;

    sf::RenderTexture m_lightLayer;
    sf::RectangleShape m_lightLayerRect;

    tibia::Map m_map;

    std::vector<sf::Vertex> m_miniMapVertices;

    sf::VertexArray m_tileMapTileVertices;

    tibia::ObjectList    m_tileMapObjects[5];
    tibia::CreatureList  m_tileMapCreatures[5];
    tibia::AnimationList m_tileMapAnimations[5];
    tibia::ThingList     m_tileMapThings[5];

    tibia::CreaturePtr m_player;
};

} // tibia

#endif // TIBIA_GAME_HPP

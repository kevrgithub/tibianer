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

    void handleMouseInput(sf::Event event)
    {
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (m_mouseTile != nullptr)
            {
                tibia::ObjectList* objectList = m_mouseTile->getObjectList();

                for (auto object : *objectList)
                {
                    doCreatureInteractWithObject(m_player, object);
                }
            }
        }
    }

    tibia::TilePtr getTile(sf::Vector2u tileCoords, int z)
    {
        int tileNumber = tibia::Utility::getTileNumberByTileCoords(tileCoords);

        tibia::TileList* tileList = m_map.tileMapTiles[z].getTileList();

        return tileList->at(tileNumber);
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

            bool tileHasSolidObject     = false;
            bool tileHasChairObject     = false;
            bool tileHasMoveAboveObject = false;
            bool tileHasMoveBelowObject = false;

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

    void doCreatureInteractWithObject(tibia::CreaturePtr creature, tibia::ObjectPtr object)
    {
        if (creature == nullptr || object == nullptr)
        {
            return;
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
            return;
        }

        // ladder and rope up
        if (object->getId() == tibia::SpriteData::ladder || object->getId() == tibia::SpriteData::ropeUp)
        {
            // TODO: make rope up require a rope

            tibia::TilePtr toTile = getTileForMoveAbove(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() + 1);

            doCreatureMoveToTile(creature, toTile, tibia::Directions::up);
        }

        // sewer grate
        if (object->getId() == tibia::SpriteData::sewerGrate)
        {
            tibia::TilePtr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() - 1);

            doCreatureMoveToTile(creature, toTile, tibia::Directions::down);
        }

        // dig holes // TODO: require shovel or pickaxe to dig
        if (object->getId() == tibia::SpriteData::digHole[0])
        {
            object->setId(tibia::SpriteData::digHole[1]);
        }
        else if (object->getId() == tibia::SpriteData::digHoleIce[0])
        {
            object->setId(tibia::SpriteData::digHoleIce[1]);
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

        // blueberry bush
        if (object->getId() == tibia::SpriteData::bushBlueberry)
        {
            object->setId(tibia::SpriteData::bush);
        }

        // lever
        if (object->getId() == tibia::SpriteData::lever[0])
        {
            object->setId(tibia::SpriteData::lever[1]);
        }
        else if (object->getId() == tibia::SpriteData::lever[1])
        {
            object->setId(tibia::SpriteData::lever[0]);
        }
    }

    void drawMouseCursor(sf::RenderWindow* mainWindow)
    {
        m_mouseCursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mainWindow)));
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

    void updateMouseTile(sf::RenderWindow* mainWindow)
    {
        m_mouseTile = std::move(getTileUnderMouseCursor(mainWindow));
    }

    tibia::TilePtr getTileUnderMouseCursor(sf::RenderWindow* mainWindow)
    {
        sf::Vector2i mouseWindowPosition = sf::Mouse::getPosition(*mainWindow);

        sf::Vector2u mouseTilePosition = static_cast<sf::Vector2u>(m_gameWindow.mapPixelToCoords(mouseWindowPosition, m_gameWindowView));

        if (mouseTilePosition.x < 0) mouseTilePosition.x = 0;
        if (mouseTilePosition.y < 0) mouseTilePosition.y = 0;

        mouseTilePosition.x -= tibia::GuiData::gameWindowX;
        mouseTilePosition.y -= tibia::GuiData::gameWindowY;

        // out of bounds of map
        if
        (
            mouseTilePosition.x < 0 ||
            mouseTilePosition.y < 0 ||
            mouseTilePosition.x > tibia::MAP_TILE_XY_MAX ||
            mouseTilePosition.y > tibia::MAP_TILE_XY_MAX
        )
        {
            return nullptr;
        }

        // convert mouse position to tile position
        while (mouseTilePosition.x > 0 && mouseTilePosition.x % tibia::TILE_SIZE != 0) mouseTilePosition.x--;
        while (mouseTilePosition.y > 0 && mouseTilePosition.y % tibia::TILE_SIZE != 0) mouseTilePosition.y--;

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
        sf::Vector2u vecMovement = tibia::Utility::getVectorByDirection(direction);

        vecMovement.x = vecMovement.x * tibia::TILE_SIZE;
        vecMovement.y = vecMovement.y * tibia::TILE_SIZE;

        int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(creature->getTileX() + vecMovement.x, creature->getTileY() + vecMovement.y));
 
        return m_map.tileMapTiles[creature->getZ()].getTileList()->at(tileNumber);
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

                tibia::TilePtr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

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

        tibia::BitmapFontText bitmapFontTextDefault;
        bitmapFontTextDefault.setText(&m_bitmapFontDefault, "The quick brown fox jumps over the lazy dog", tibia::Colors::textYellow);
        bitmapFontTextDefault.setPosition
        (
            (mainWindow->getSize().x / 2) - (bitmapFontTextDefault.getVertexArray()->getBounds().width / 2),
            0
        );

        sf::RenderStates states;
        states.texture = m_bitmapFontDefault.getTexture();

        mainWindow->draw(bitmapFontTextDefault, states);
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

            tibia::TilePtr tile = tileList->at(tileNumber);

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

    sf::Sprite m_mouseCursor;

    tibia::TilePtr m_mouseTile;

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

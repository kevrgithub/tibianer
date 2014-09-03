#ifndef TIBIA_GAME_HPP
#define TIBIA_GAME_HPP

#include <cmath>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include <boost/range/algorithm/remove_if.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "iup.h"

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Tile.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/BitmapFont.hpp"
#include "tibia/BitmapFontText.hpp"
#include "tibia/Sound.hpp"
#include "tibia/GameText.hpp"
#include "tibia/FloatingText.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Object.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Animation.hpp"
#include "tibia/Light.hpp"
#include "tibia/Projectile.hpp"
#include "tibia/Bar.hpp"

namespace tibia
{

class Game
{

public:

    struct Options_t
    {
        bool isMouseUseDefaultMouseCursorEnabled = false;

        bool isGameShowFloatingTextEnabled      = true;
        bool isGameShowNamesEnabled             = false;
        bool isGameShowCreatureBarsEnabled      = false;
        bool isGameSmoothLightingEnabled        = false;
        bool isGameFriendlyFireEnabled          = false;

        bool isAudioSoundEnabled = true;
        bool isAudioMusicEnabled = true;

        bool isCheatInfiniteHealthEnabled = false;
        bool isCheatInfiniteManaEnabled   = false;
    };

    Options_t options;

    struct Gui_t
    {
        int tabButtonsState = tibia::GuiData::TabButtons::State::miniMap;

        int combatButtonsState = tibia::GuiData::CombatButtons::State::normal;

        bool topMiniMap   = true;
        bool topStatus    = false;
        bool topEquipment = false;

        bool bottomInventory  = true;
        bool bottomSkills     = false;
        bool bottomCombat     = false;

        bool isInventorySplit = false;

        bool chatLogWindowButtonScrollUp   = false;
        bool chatLogWindowButtonScrollDown = false;

        bool inventoryWindowButtonScrollUp   = false;
        bool inventoryWindowButtonScrollDown = false;

        bool combatWindowButtonScrollUp   = false;
        bool combatWindowButtonScrollDown = false;
    };

    Gui_t gui;

    Game::Game()
    {
        m_gameWindow.create(tibia::GuiData::GameWindow::width, tibia::GuiData::GameWindow::height);
        m_gameWindowView.reset(sf::FloatRect(0, 0, tibia::GuiData::GameWindow::width, tibia::GuiData::GameWindow::height));

        m_gameWindowLayer.create(tibia::GuiData::GameWindow::width, tibia::GuiData::GameWindow::height);

        m_lightLayer.create(tibia::LIGHT_WIDTH, tibia::LIGHT_HEIGHT);

        m_lightBrightness = tibia::LightBrightness::day;

        m_timeOfDay = tibia::TimeOfDay::day;

        m_miniMapWindow.create(tibia::GuiData::MiniMapWindow::width, tibia::GuiData::MiniMapWindow::height);
        m_miniMapWindowView.reset(sf::FloatRect(0, 0, tibia::TILES_WIDTH * tibia::GuiData::MiniMapWindow::zoomDefault, tibia::TILES_WIDTH * tibia::GuiData::MiniMapWindow::zoomDefault));
        m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomDefault;

        m_chatLogWindow.create(tibia::GuiData::ChatLogWindow::width, tibia::GuiData::ChatLogWindow::height);
        m_chatLogWindowView.reset(sf::FloatRect(0, 0, tibia::GuiData::ChatLogWindow::width, tibia::GuiData::ChatLogWindow::height));

        m_inventorySlotsWindow.create(tibia::GuiData::InventoryWindow::Slots::Window::width, tibia::GuiData::InventoryWindow::Slots::Window::height);
        m_inventorySlotsWindowView.reset(sf::FloatRect(0, 0, tibia::GuiData::InventoryWindow::Slots::Window::width, tibia::GuiData::InventoryWindow::Slots::Window::height));

        m_combatCreaturesWindow.create(tibia::GuiData::CombatWindow::Creatures::Window::width, tibia::GuiData::CombatWindow::Creatures::Window::height);
        m_combatCreaturesWindowView.reset(sf::FloatRect(0, 0, tibia::GuiData::CombatWindow::Creatures::Window::width, tibia::GuiData::CombatWindow::Creatures::Window::height));

        m_statusBarText.setPosition(tibia::GuiData::StatusBarText::position);

        m_tileMapTileVertices.setPrimitiveType(sf::Quads);
    }

    void setMouseCursorVisible(sf::RenderWindow* mainWindow, bool isVisible)
    {
        mainWindow->setMouseCursorVisible(isVisible);
    }

    void showOptionsWindow()
    {
        setMouseCursorVisible(m_mainWindow, true);

        int option_isGameShowFloatingTextEnabled = this->options.isGameShowFloatingTextEnabled;
        int option_isGameShowNamesEnabled        = this->options.isGameShowNamesEnabled;
        int option_isGameShowCreatureBarsEnabled = this->options.isGameShowCreatureBarsEnabled;
        int option_isGameSmoothLightingEnabled   = this->options.isGameSmoothLightingEnabled;
        int option_isGameFriendlyFireEnabled     = this->options.isGameFriendlyFireEnabled;

        int option_isAudioSoundEnabled           = this->options.isAudioSoundEnabled;
        int option_isAudioMusicEnabled           = this->options.isAudioMusicEnabled;

        int option_isCheatInfiniteHealthEnabled  = this->options.isCheatInfiniteHealthEnabled;
        int option_isCheatInfiniteManaEnabled    = this->options.isCheatInfiniteManaEnabled;

        IupGetParam
        (
            "Options", NULL, 0,

            "Game %t\n"
                "Show Floating Text: %b[Disabled,Enabled]\n"
                "Show Names: %b[Disabled,Enabled]\n"
                "Show Creature Bars: %b[Disabled,Enabled]\n"
                "Smooth Lighting: %b[Disabled,Enabled]\n"
                "Friendly Fire: %b[Disabled,Enabled]\n"

            "Audio %t\n"
                "Sound: %b[Disabled,Enabled]\n"
                "Music: %b[Disabled,Enabled]\n"

            "Cheats %t\n"
                "Infinite Health: %b[Disabled,Enabled]\n"
                "Infinite Mana: %b[Disabled,Enabled]\n"

            ,

            &option_isGameShowFloatingTextEnabled,
            &option_isGameShowNamesEnabled,
            &option_isGameShowCreatureBarsEnabled,
            &option_isGameSmoothLightingEnabled,
            &option_isGameFriendlyFireEnabled,

            &option_isAudioSoundEnabled,
            &option_isAudioMusicEnabled,

            &option_isCheatInfiniteHealthEnabled,
            &option_isCheatInfiniteManaEnabled,

            NULL
        );

        this->options.isGameShowFloatingTextEnabled = option_isGameShowFloatingTextEnabled;
        this->options.isGameShowNamesEnabled        = option_isGameShowNamesEnabled;
        this->options.isGameShowCreatureBarsEnabled = option_isGameShowCreatureBarsEnabled;
        this->options.isGameSmoothLightingEnabled   = option_isGameSmoothLightingEnabled;
        this->options.isGameFriendlyFireEnabled     = option_isGameFriendlyFireEnabled;

        this->options.isAudioSoundEnabled           = option_isAudioSoundEnabled;
        this->options.isAudioMusicEnabled           = option_isAudioMusicEnabled;

        this->options.isCheatInfiniteHealthEnabled  = option_isCheatInfiniteHealthEnabled;
        this->options.isCheatInfiniteManaEnabled    = option_isCheatInfiniteManaEnabled;

        if (this->options.isMouseUseDefaultMouseCursorEnabled == false)
        {
            setMouseCursorVisible(m_mainWindow, false);
        }

        updateLightingStyle();
    }

    void createPlayer()
    {
        tibia::Creature::Ptr player = std::make_shared<tibia::Creature>
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
        player->setHpMax(1000);
        player->setHp(1000);
        player->setMovementSpeed(tibia::MovementSpeeds::player);

        m_player = player;

        tibia::Tile::Ptr tile = getThingTile(m_player);

        tile->addCreature(m_player);
    }

    void respawnPlayer(tibia::Creature::Ptr creature)
    {
        creature->setIsPlayer(false);

        tibia::Creature::Ptr player = std::make_shared<tibia::Creature>(*creature);

        player->getInventoryItemList()->clear();
        player->setIsDead(false);
        player->setIsPlayer(true);
        player->setHp(player->getHpMax());
        player->setMp(player->getMpMax());

        m_player = player;

        tibia::Tile::Ptr toTile = getTile
        (
            sf::Vector2u
            (
                m_map.properties.playerStartX * tibia::TILE_SIZE,
                m_map.properties.playerStartY * tibia::TILE_SIZE
            ),
            m_map.properties.playerStartZ
        );

        m_player->setTileCoords(toTile->getPosition().x, toTile->getPosition().y);
        m_player->setZ(toTile->getZ());
        m_player->setDirection(tibia::Directions::down);

        toTile->addCreature(m_player);

        spawnAnimation(m_player->getTilePosition(), m_player->getZ(), tibia::Animations::spellBlue);
    }

    void showMapNameAndAuthor()
    {
        std::stringstream ss;
        ss << m_map.properties.name << "\nby " << m_map.properties.author;

        showGameWindowText(ss.str(), tibia::Colors::Text::white);

        ss.str("");

        ss << m_map.properties.name << " by " << m_map.properties.author;

        doChatLogWindowAddText(ss.str(), sf::Color::Red);

        if (m_map.properties.description.size() != 0)
        {
            doChatLogWindowAddText(m_map.properties.description, sf::Color::Red);
        }
    }

    void doEnterGame()
    {
        setTimeOfDay(m_map.properties.timeOfDay);

        spawnAnimation(m_player->getTilePosition(), m_player->getZ(), tibia::Animations::spellBlue);

        showMapNameAndAuthor();
    }

    void updateLightingStyle()
    {
        if (this->options.isGameSmoothLightingEnabled == true)
        {
            m_light.setTexture(tibia::Textures::lightsSmooth);
        }
        else
        {
            m_light.setTexture(tibia::Textures::lights);
        }
    }

    bool loadTextures()
    {
        for (auto& texture : tibia::UMaps::textureFiles)
        {
            if (texture.second.loadFromFile(texture.first) == false)
            {
                std::cout << "Error: Failed to load texture file: " << texture.first << std::endl;
                return false;
            }
        }

        m_mouseCursor.setTexture(tibia::Textures::cursor);

        updateLightingStyle();

        return true;
    }

    bool loadFonts()
    {
        if (m_fontDefault.loadFromFile(tibia::Fonts::Default::filename) == false)
        {
            return false;
        }

        if (m_fontConsole.loadFromFile(tibia::Fonts::Console::filename) == false)
        {
            return false;
        }

        if (m_fontSystem.loadFromFile(tibia::Fonts::System::filename) == false)
        {
            return false;
        }

        return true;
    }

    bool loadBitmapFonts()
    {
        if (m_bitmapFontDefault.load(tibia::BitmapFonts::Default::filename, tibia::BitmapFonts::Default::glyphSize, &tibia::BitmapFonts::Default::glyphWidths) == false)
        {
            return false;
        }

        if (m_bitmapFontTiny.load(tibia::BitmapFonts::Tiny::filename, tibia::BitmapFonts::Tiny::glyphSize, &tibia::BitmapFonts::Tiny::glyphWidths, -1) == false)
        {
            return false;
        }

        if (m_bitmapFontModern.load(tibia::BitmapFonts::Modern::filename, tibia::BitmapFonts::Modern::glyphSize, &tibia::BitmapFonts::Modern::glyphWidths, -1) == false)
        {
            return false;
        }

        return true;
    }

    bool loadSounds()
    {
        for (auto& sound : tibia::UMaps::soundFiles)
        {
            if (sound.second.loadFromFile(sound.first) == false)
            {
                std::cout << "Error: Failed to load sound file: " << sound.first << std::endl;
                return false;
            }
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
            tibia::UMaps::spriteFlags[i] = tibia::Utility::getSpriteFlags(i);
        }
    }

    void loadWindows()
    {
        //
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
                spawnFloatingText(m_player->getTilePosition(), m_player->getZ(), "Floating Text", tibia::Colors::Text::white);
                break;

            case sf::Keyboard::H:
                doCreatureSpeech(m_player, "Hello!");
                break;

            case sf::Keyboard::M:
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::magicWall);
                break;

            case sf::Keyboard::P:
            {
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::poolRed, true);
                break;
            }

            case sf::Keyboard::R:
            {
                tibia::Tile::Ptr toTile = getTile
                (
                    sf::Vector2u
                    (
                        m_map.properties.playerStartX * tibia::TILE_SIZE,
                        m_map.properties.playerStartY * tibia::TILE_SIZE
                    ),
                    m_map.properties.playerStartZ
                );

                doMoveThingToTile(m_player, toTile, tibia::Directions::down, true);

                std::cout << "To Tile X: " << toTile->getPosition().x << std::endl;
                std::cout << "To Tile Y: " << toTile->getPosition().y << std::endl;
                std::cout << "To Tile Z: " << toTile->getZ()          << std::endl;

                std::cout << "Player Start X: " << m_map.properties.playerStartX << std::endl;
                std::cout << "Player Start Y: " << m_map.properties.playerStartY << std::endl;
                std::cout << "Player Start Z: " << m_map.properties.playerStartZ << std::endl;

                break;
            }

            case sf::Keyboard::T:
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::torchBig);
                break;

            case sf::Keyboard::U:
            {
                spawnDecayObject(m_player->getTilePosition(), m_player->getZ(), tibia::SpriteData::fieldFire);
                break;
            }

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

            case sf::Keyboard::I:
            {
                std::cout << "Player HP,MP: " << m_player->getHp() << "," << m_player->getMp() << std::endl;
                std::cout << "Player X,Y,Z: " << m_player->getX()  << "," << m_player->getY()  << "," << m_player->getZ() <<std::endl;

                tibia::Tile::Ptr playerTile = getThingTile(m_player);

                std::cout << "Tile X,Y: "    << playerTile->getPosition().x << "," << playerTile->getPosition().y << std::endl;
                std::cout << "Tile Height: " << playerTile->getHeight() << std::endl;
                break;
            }

            case sf::Keyboard::Z:
                m_player->setZ(utility::getRandomNumber(tibia::ZAxis::min, tibia::ZAxis::max));

                std::cout << "Player Z: " << m_player->getZ() << std::endl;
                break;

            case sf::Keyboard::G:
            {
                handleCreatureModifyHp(nullptr, m_player, -(m_player->getHp()), tibia::ModifyHpTypes::blood);
                break;
            }

            case sf::Keyboard::X:
            {
                spawnProjectile
                (
                    m_player,
                    tibia::ProjectileTypes::spellFire,
                    m_player->getDirection(),
                    sf::Vector2f(m_player->getTileX(), m_player->getTileY()),
                    sf::Vector2f
                    (
                        m_player->getTileX() + (tibia::Utility::getVectorByDirection(m_player->getDirection()).x * tibia::TILE_SIZE),
                        m_player->getTileY() + (tibia::Utility::getVectorByDirection(m_player->getDirection()).y * tibia::TILE_SIZE)
                    ),
                    m_player->getZ()
                );
                break;
            }

            case sf::Keyboard::C:
            {
                spawnProjectile
                (
                    m_player,
                    tibia::ProjectileTypes::spear,
                    m_player->getDirection(),
                    sf::Vector2f(m_player->getTileX(), m_player->getTileY()),
                    sf::Vector2f
                    (
                        m_player->getTileX() + (tibia::Utility::getVectorByDirection(m_player->getDirection()).x * tibia::TILE_SIZE),
                        m_player->getTileY() + (tibia::Utility::getVectorByDirection(m_player->getDirection()).y * tibia::TILE_SIZE)
                    ),
                    m_player->getZ()
                );
                break;
            }
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
        if (event.mouseButton.button == sf::Mouse::Button::Left)
        {
            if (isMouseInsideMiniMapWindow() == true)
            {
                if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomDefault)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomMax;
                }
                else if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomMax)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomMin;
                }
                else if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomMin)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomDefault;
                }

                updateMiniMapWindowZoom();
            }
            else if (tibia::GuiData::OptionsButton::rect.contains(getMouseWindowPosition()) == true)
            {
                showOptionsWindow();
            }
            else if (tibia::GuiData::TabButtons::Inventory::rect.contains(getMouseWindowPosition()) == true)
            {
                this->gui.tabButtonsState = tibia::GuiData::TabButtons::State::inventory;

                this->gui.topMiniMap   = false;
                this->gui.topStatus    = false;
                this->gui.topEquipment = true;

                this->gui.bottomInventory = true;
                this->gui.bottomSkills    = false;
                this->gui.bottomCombat    = false;
            }
            else if (tibia::GuiData::TabButtons::Status::rect.contains(getMouseWindowPosition()) == true)
            {
                this->gui.tabButtonsState = tibia::GuiData::TabButtons::State::status;

                this->gui.topMiniMap   = false;
                this->gui.topStatus    = true;
                this->gui.topEquipment = false;

                this->gui.bottomInventory = false;
                this->gui.bottomSkills    = true;
                this->gui.bottomCombat    = false;
            }
            else if (tibia::GuiData::TabButtons::Combat::rect.contains(getMouseWindowPosition()) == true)
            {
                this->gui.tabButtonsState = tibia::GuiData::TabButtons::State::combat;

                this->gui.bottomInventory = false;
                this->gui.bottomSkills    = false;
                this->gui.bottomCombat    = true;
            }
            else if (tibia::GuiData::TabButtons::MiniMap::rect.contains(getMouseWindowPosition()) == true)
            {
                this->gui.tabButtonsState = tibia::GuiData::TabButtons::State::miniMap;

                this->gui.topMiniMap   = true;
                this->gui.topStatus    = false;
                this->gui.topEquipment = false;
            }
            else if (tibia::GuiData::ChatLogWindow::Buttons::ScrollDown::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollDown == true)
                {
                    doChatLogWindowViewScrollDown();
                }
            }
            else if (tibia::GuiData::ChatLogWindow::Buttons::ScrollUp::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollUp == true)
                {
                    doChatLogWindowViewScrollUp();
                }
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::Buttons::ScrollDown::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollDown == true)
                    {
                        doInventoryWindowSlotsViewScrollDown();
                    }
                }
                else if (tibia::GuiData::InventoryWindow::Buttons::ScrollUp::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollUp == true)
                    {
                        doInventoryWindowSlotsViewScrollUp();
                    }
                }
                else if (tibia::GuiData::InventoryWindow::Icons::Container::rect.contains(getMouseWindowPosition()) == true)
                {
                    m_player->sortInventoryItemsById();
                }
                else if (tibia::GuiData::InventoryWindow::Slots::Window::rect.contains(getMouseWindowPosition()) == true)
                {
                    // move inventory item to front of inventory list

                    int inventoryWindowSlotIndex = getMouseInventoryWindowSlotIndex();

                    //std::cout << "inventoryWindowSlotIndex: " << inventoryWindowSlotIndex << std::endl;

                    tibia::Creature::InventoryItemList* inventoryItemList = m_player->getInventoryItemList();

                    if (inventoryItemList->size() != 0 && (inventoryWindowSlotIndex < inventoryItemList->size()))
                    {
                        auto inventoryItemIt = inventoryItemList->begin() + inventoryWindowSlotIndex;

                        inventoryItemList->insert(inventoryItemList->begin(), *inventoryItemIt);

                        m_player->removeInventoryItem(inventoryWindowSlotIndex + 1);
                    }
                }
            }
            else if (this->gui.bottomCombat == true)
            {
                if (tibia::GuiData::CombatWindow::Buttons::ScrollDown::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollDown == true)
                    {
                        doCombatWindowCreaturesViewScrollDown();
                    }
                }
                else if (tibia::GuiData::CombatWindow::Buttons::ScrollUp::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollUp == true)
                    {
                        doCombatWindowCreaturesViewScrollUp();
                    }
                }

                if (tibia::GuiData::CombatButtons::FullOffense::rect.contains(getMouseWindowPosition()) == true)
                {
                    this->gui.combatButtonsState = tibia::GuiData::CombatButtons::State::offense;
                }
                else if (tibia::GuiData::CombatButtons::HalfOffenseDefense::rect.contains(getMouseWindowPosition()) == true)
                {
                    this->gui.combatButtonsState = tibia::GuiData::CombatButtons::State::normal;
                }
                else if (tibia::GuiData::CombatButtons::FullDefense::rect.contains(getMouseWindowPosition()) == true)
                {
                    this->gui.combatButtonsState = tibia::GuiData::CombatButtons::State::defense;
                }
            }
            else if (this->gui.bottomSkills == true)
            {
                if (tibia::GuiData::OutfitButtons::Head::rect.contains(getMouseWindowPosition()) == true)
                {
                    int head = m_player->getOutfitHead();

                    head++;

                    if (head > (tibia::Outfits::head.size() / 4) - 1)
                    {
                        head = 0;
                    }

                    m_player->setOutfitHead(head);
                }
                else if (tibia::GuiData::OutfitButtons::Body::rect.contains(getMouseWindowPosition()) == true)
                {
                    int body = m_player->getOutfitBody();

                    body++;

                    if (body > (tibia::Outfits::body.size() / 4) - 1)
                    {
                        body = 0;
                    }

                    m_player->setOutfitBody(body);
                }
                else if (tibia::GuiData::OutfitButtons::Legs::rect.contains(getMouseWindowPosition()) == true)
                {
                    int legs = m_player->getOutfitLegs();

                    legs++;

                    if (legs > (tibia::Outfits::legs.size() / 4) - 1)
                    {
                        legs = 0;
                    }

                    m_player->setOutfitLegs(legs);
                }
                else if (tibia::GuiData::OutfitButtons::Feet::rect.contains(getMouseWindowPosition()) == true)
                {
                    int feet = m_player->getOutfitFeet();

                    feet++;

                    if (feet > (tibia::Outfits::feet.size() / 4) - 1)
                    {
                        feet = 0;
                    }

                    m_player->setOutfitFeet(feet);
                }
                else if (tibia::GuiData::OutfitButtons::All::rect.contains(getMouseWindowPosition()) == true)
                {
                    m_player->setOutfitRandom();
                }
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Button::Right)
        {
            if (isMouseInsideGameWindow() == true)
            {
                if (m_mouseTile != nullptr)
                {
                    // rope up things
                    if (m_player->getZ() > tibia::ZAxis::min && m_player->hasInventoryItem(tibia::SpriteData::rope))
                    {
                        if (m_mouseTile->getFlags() & tibia::SpriteFlags::moveBelow)
                        {
                            //std::cout << "right clicked moveBelow" << std::endl;

                            tibia::Tile::Ptr belowTile = getTile
                            (
                                sf::Vector2u
                                (
                                    m_mouseTile->getPosition().x + tibia::TILE_SIZE,
                                    m_mouseTile->getPosition().y + tibia::TILE_SIZE
                                ),
                                m_mouseTile->getZ() - 1
                            );

                            tibia::Tile::Ptr toTile = getTileForMoveAbove(belowTile->getPosition(), belowTile->getZ() + 1);

                            tibia::Creature::List* creatureList = belowTile->getCreatureList();

                            tibia::Object::List* objectList = belowTile->getObjectList();

                            tibia::Thing::Ptr ropeUpThing = nullptr;

                            if (creatureList->size())
                            {
                                ropeUpThing = creatureList->back();
                            }
                            else
                            {
                                if (objectList->size())
                                {
                                    tibia::Object::Ptr ropeUpObject = objectList->back();

                                    if (ropeUpObject->getFlags() & tibia::SpriteFlags::moveable)
                                    {
                                        ropeUpThing = ropeUpObject;
                                    }
                                }
                            }

                            if (ropeUpThing != nullptr)
                            {
                                doMoveThingToTile(ropeUpThing, toTile, tibia::Directions::up, true);
                            }
                        }
                    }

                    if (m_mouseTile->getCreatureList()->size())
                    {
                        // melee attack creature

                        tibia::Creature::Ptr creature = m_mouseTile->getCreatureList()->back();

                        if (creature != m_player)
                        {
                            int creatureTileDistance = tibia::Utility::calculateTileDistance
                            (
                                m_player->getTileX(),
                                m_player->getTileY(),
                                creature->getTileX(),
                                creature->getTileY()
                            );

                            if (creatureTileDistance == 1)
                            {
                                handleCreatureModifyHp(m_player, creature, -10, tibia::ModifyHpTypes::blood);
                            }
                        }
                    }

                    if (m_mouseTile->getObjectList()->size())
                    {
                        // pickup object or interact with object

                        tibia::Object::Ptr object = m_mouseTile->getObjectList()->back();

                        if (object->getFlags() & tibia::SpriteFlags::pickupable)
                        {
                            int objectTileDistance = tibia::Utility::calculateTileDistance
                            (
                                m_player->getTileX(),
                                m_player->getTileY(),
                                object->getTileX(),
                                object->getTileY()
                            );

                            if (objectTileDistance > 1)
                            {
                                showStatusBarText("Object is too far away.");
                            }
                            else
                            {
                                int creatureAddInventoryItemResult = m_player->addInventoryItem(object->getId(), object->getCount(), object->getFlags());

                                if (creatureAddInventoryItemResult == tibia::CreatureAddInventoryItemResult::success)
                                {
                                    object->setIsReadyForErase(true);
                                }
                                else if (creatureAddInventoryItemResult == tibia::CreatureAddInventoryItemResult::inventoryItemCountMax)
                                {
                                    showStatusBarText("You cannot pick up any more of that object.");
                                }
                                else if (creatureAddInventoryItemResult == tibia::CreatureAddInventoryItemResult::inventoryItemsMax)
                                {
                                    showStatusBarText("Your inventory is full.");
                                }
                            }
                        }
                        else
                        {
                            doCreatureInteractWithTileObjects(m_player, m_mouseTile);
                        }
                    }
                }
            }
            else if (tibia::GuiData::ChatLogWindow::Buttons::ScrollDown::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollDown == true)
                {
                    doChatLogWindowViewScrollToBottom();
                }
            }
            else if (tibia::GuiData::ChatLogWindow::Buttons::ScrollUp::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollUp == true)
                {
                    doChatLogWindowViewScrollToTop();
                }
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::Buttons::ScrollDown::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollDown == true)
                    {
                        doInventoryWindowSlotsViewScrollToBottom();
                    }
                }
                else if (tibia::GuiData::InventoryWindow::Buttons::ScrollUp::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollUp == true)
                    {
                        doInventoryWindowSlotsViewScrollToTop();
                    }
                }
                else if (tibia::GuiData::InventoryWindow::Icons::Container::rect.contains(getMouseWindowPosition()) == true)
                {
                    m_player->sortInventoryItemsByCount();
                }
                else if (tibia::GuiData::InventoryWindow::Slots::Window::rect.contains(getMouseWindowPosition()) == true)
                {
                    // interact with inventory item

                    int inventoryWindowSlotIndex = getMouseInventoryWindowSlotIndex();

                    tibia::Creature::InventoryItemList* inventoryItemList = m_player->getInventoryItemList();

                    if (inventoryItemList->size() != 0 && inventoryWindowSlotIndex < inventoryItemList->size())
                    {
                        auto inventoryItemIt = inventoryItemList->begin() + inventoryWindowSlotIndex;

                        //
                    }
                }
            }
            else if (this->gui.bottomCombat == true)
            {
                if (tibia::GuiData::CombatWindow::Buttons::ScrollDown::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollDown == true)
                    {
                        doCombatWindowCreaturesViewScrollToBottom();
                    }
                }
                else if (tibia::GuiData::CombatWindow::Buttons::ScrollUp::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollUp == true)
                    {
                        doCombatWindowCreaturesViewScrollToTop();
                    }
                }
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Button::Middle)
        {
            if (isMouseInsideGameWindow() == true)
            {
                // push object

                tibia::Object::List* objectList = m_mouseTile->getObjectList();

                if (objectList->size())
                {
                    tibia::Object::Ptr object = objectList->back();

                    //std::cout << "Push Object ID: " << object->getId() << std::endl;

                    doCreaturePushObject(m_player, object);
                }
            }
            else if (isMouseInsideMiniMapWindow() == true)
            {
                // reset minimap zoom to default

                m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomDefault;

                updateMiniMapWindowZoom();
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::Icons::Container::rect.contains(getMouseWindowPosition()) == true)
                {
                    m_player->sortInventoryItemsReverse();
                }
                else if (tibia::GuiData::InventoryWindow::Slots::Window::rect.contains(getMouseWindowPosition()) == true)
                {
                    // drop inventory item on ground

                    int inventoryWindowSlotIndex = getMouseInventoryWindowSlotIndex();

                    //std::cout << "inventoryWindowSlotIndex: " << inventoryWindowSlotIndex << std::endl;

                    tibia::Creature::InventoryItemList* inventoryItemList = m_player->getInventoryItemList();

                    if (inventoryItemList->size() != 0 && inventoryWindowSlotIndex < inventoryItemList->size())
                    {
                        auto inventoryItemIt = inventoryItemList->begin() + inventoryWindowSlotIndex;

                        doCreatureDropInventoryItem(m_player, inventoryItemIt);
                    }
                }
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Button::XButton1)
        {
            if (tibia::GuiData::ChatLogWindow::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollDown == true)
                {
                    doChatLogWindowViewScrollToBottom();
                }
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollUp == true)
                    {
                        doInventoryWindowSlotsViewScrollToTop();
                    }
                }
            }
            else if (this->gui.bottomCombat == true)
            {
                if (tibia::GuiData::CombatWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollUp == true)
                    {
                        doCombatWindowCreaturesViewScrollToTop();
                    }
                }
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Button::XButton2)
        {
            if (tibia::GuiData::ChatLogWindow::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollUp == true)
                {
                    doChatLogWindowViewScrollToTop();
                }
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollDown == true)
                    {
                        doInventoryWindowSlotsViewScrollToBottom();
                    }
                }
            }
            else if (this->gui.bottomCombat == true)
            {
                if (tibia::GuiData::CombatWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollDown == true)
                    {
                        doCombatWindowCreaturesViewScrollToBottom();
                    }
                }
            }
        }
    }

    void handleMouseWheelEvent(sf::Event event)
    {
        // scroll up
        if (event.mouseWheel.delta > 0)
        {
            if (isMouseInsideGameWindow() == true)
            {
                // zoom in

                m_gameWindowZoomLevel -= m_gameWindowZoomFactor;

                if (m_gameWindowZoomLevel < 1.0f) m_gameWindowZoomLevel = 1.0f;

                m_gameWindowView.setSize(sf::Vector2f(tibia::TILES_WIDTH * m_gameWindowZoomLevel, tibia::TILES_HEIGHT * m_gameWindowZoomLevel));
            }
            else if (isMouseInsideMiniMapWindow() == true)
            {
                // decrease minimap zoom
                if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomMax)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomDefault;
                }
                else if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomDefault)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomMin;
                }

                updateMiniMapWindowZoom();
            }
            else if (tibia::GuiData::ChatLogWindow::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollUp == true)
                {
                    doChatLogWindowViewScrollUp();
                }
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollUp == true)
                    {
                        doInventoryWindowSlotsViewScrollUp();
                    }
                }
            }
            else if (this->gui.bottomCombat == true)
            {
                if (tibia::GuiData::CombatWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollUp == true)
                    {
                        doCombatWindowCreaturesViewScrollUp();
                    }
                }
            }
        }

        // scroll down
        else if (event.mouseWheel.delta < 0)
        {
            if (isMouseInsideGameWindow() == true)
            {
                // zoom out

                m_gameWindowZoomLevel += m_gameWindowZoomFactor;

                m_gameWindowView.setSize(sf::Vector2f(tibia::TILES_WIDTH * m_gameWindowZoomLevel, tibia::TILES_HEIGHT * m_gameWindowZoomLevel));
            }
            else if (isMouseInsideMiniMapWindow() == true)
            {
                // increase minimap zoom
                if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomMin)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomDefault;
                }
                else if (m_miniMapWindowZoom == tibia::GuiData::MiniMapWindow::zoomDefault)
                {
                    m_miniMapWindowZoom = tibia::GuiData::MiniMapWindow::zoomMax;
                }

                updateMiniMapWindowZoom();
            }
            else if (tibia::GuiData::ChatLogWindow::rect.contains(getMouseWindowPosition()) == true)
            {
                if (this->gui.chatLogWindowButtonScrollDown == true)
                {
                    doChatLogWindowViewScrollDown();
                }
            }

            if (this->gui.bottomInventory == true)
            {
                if (tibia::GuiData::InventoryWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.inventoryWindowButtonScrollDown == true)
                    {
                        doInventoryWindowSlotsViewScrollDown();
                    }
                }
            }
            else if (this->gui.bottomCombat == true)
            {
                if (tibia::GuiData::CombatWindow::rect.contains(getMouseWindowPosition()) == true)
                {
                    if (this->gui.combatWindowButtonScrollDown == true)
                    {
                        doCombatWindowCreaturesViewScrollDown();
                    }
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

    void updateMiniMapWindowZoom()
    {
        m_miniMapWindowView.setSize(sf::Vector2f(tibia::TILES_WIDTH * m_miniMapWindowZoom, tibia::TILES_WIDTH * m_miniMapWindowZoom));
    }

    void doChatLogWindowAddText(const std::string& text, sf::Color color = sf::Color::Black)
    {
        sf::Text chatLogWindowText;
        chatLogWindowText.setFont(m_fontSystem);
        chatLogWindowText.setCharacterSize(tibia::Fonts::System::characterSize);
        chatLogWindowText.setString(text);
        chatLogWindowText.setColor(color);

        m_chatLogWindowTextList.push_back(chatLogWindowText);
    }

    void doChatLogWindowViewScrollDown()
    {
        m_chatLogWindowView.move(0, (tibia::GuiData::ChatLogWindow::textHeight + tibia::GuiData::ChatLogWindow::textOffset));
    }

    void doChatLogWindowViewScrollUp()
    {
        m_chatLogWindowView.move(0, -(tibia::GuiData::ChatLogWindow::textHeight + tibia::GuiData::ChatLogWindow::textOffset));
    }

    int getChatLogWindowViewMaxScrollY()
    {
        return
            -(
                (m_chatLogWindowTextList.size() - tibia::GuiData::ChatLogWindow::numLinesVisible)
                *
                (tibia::GuiData::ChatLogWindow::textHeight + tibia::GuiData::ChatLogWindow::textOffset)
            );
    }

    void doChatLogWindowViewScrollToTop()
    {
        m_chatLogWindowView.reset
        (
            sf::FloatRect
            (
                0,
                getChatLogWindowViewMaxScrollY(),
                tibia::GuiData::ChatLogWindow::width,
                tibia::GuiData::ChatLogWindow::height
            )
        );
    }

    void doChatLogWindowViewScrollToBottom()
    {
        m_chatLogWindowView.reset
        (
            sf::FloatRect
            (
                0,
                0,
                tibia::GuiData::ChatLogWindow::width,
                tibia::GuiData::ChatLogWindow::height
            )
        );
    }
    
    void doInventoryWindowSlotsViewScrollUp()
    {
        m_inventorySlotsWindowView.move(0, -tibia::GuiData::InventoryWindow::Slots::size);
    }

    void doInventoryWindowSlotsViewScrollDown()
    {
        m_inventorySlotsWindowView.move(0, tibia::GuiData::InventoryWindow::Slots::size);
    }

    void doInventoryWindowSlotsViewScrollToTop()
    {
        m_inventorySlotsWindowView.reset
        (
            sf::FloatRect
            (
                0,
                0,
                tibia::GuiData::InventoryWindow::Slots::Window::width,
                tibia::GuiData::InventoryWindow::Slots::Window::height
            )
        );
    }

    void doInventoryWindowSlotsViewScrollToBottom()
    {
        m_inventorySlotsWindowView.reset
        (
            sf::FloatRect
            (
                0,
                getInventoryWindowSlotsViewMaxScrollY(),
                tibia::GuiData::InventoryWindow::Slots::Window::width,
                tibia::GuiData::InventoryWindow::Slots::Window::height
            )
        );
    }

    int getInventoryWindowSlotsViewMaxScrollY()
    {
        int maxScrollY = (m_player->getInventoryItemList()->size() / tibia::GuiData::InventoryWindow::Slots::numSlotsHorizontal) * tibia::GuiData::InventoryWindow::Slots::size;

        maxScrollY -= (tibia::GuiData::InventoryWindow::height - (tibia::GuiData::InventoryWindow::Slots::size * 3));

        maxScrollY = maxScrollY - (maxScrollY % tibia::GuiData::InventoryWindow::Slots::size);

        return maxScrollY;
    }

    void doCombatWindowCreaturesViewScrollUp()
    {
        m_combatCreaturesWindowView.move(0, -(tibia::GuiData::CombatWindow::Creatures::height + tibia::GuiData::CombatWindow::Creatures::distanceBetweenCreatures));
    }

    void doCombatWindowCreaturesViewScrollDown()
    {
        m_combatCreaturesWindowView.move(0, tibia::GuiData::CombatWindow::Creatures::height + tibia::GuiData::CombatWindow::Creatures::distanceBetweenCreatures);
    }

    void doCombatWindowCreaturesViewScrollToTop()
    {
        m_combatCreaturesWindowView.reset
        (
            sf::FloatRect
            (
                0,
                0,
                tibia::GuiData::CombatWindow::Creatures::Window::width,
                tibia::GuiData::CombatWindow::Creatures::Window::height
            )
        );
    }

    void doCombatWindowCreaturesViewScrollToBottom()
    {
        m_combatCreaturesWindowView.reset
        (
            sf::FloatRect
            (
                0,
                getCombatWindowCreaturesViewMaxScrollY(),
                tibia::GuiData::CombatWindow::Creatures::Window::width,
                tibia::GuiData::CombatWindow::Creatures::Window::height
            )
        );
    }

    int getCombatWindowCreaturesViewMaxScrollY()
    {
        int creaturesRemainder = m_combatCreaturesWindowNumCreatures % tibia::GuiData::CombatWindow::Creatures::numCreaturesVisible;

        int creaturesNeeded = 0;

        if (creaturesRemainder != 0 && m_combatCreaturesWindowNumCreatures < tibia::GuiData::CombatWindow::Creatures::numCreaturesVisible)
        {
            creaturesNeeded = tibia::GuiData::CombatWindow::Creatures::numCreaturesVisible - creaturesRemainder;
        }

        int maxScrollY =
        (
            (
                m_combatCreaturesWindowNumCreatures
                *
                (
                    (tibia::GuiData::CombatWindow::Creatures::height + tibia::GuiData::CombatWindow::Creatures::distanceBetweenCreatures)
                    -
                    ((tibia::GuiData::CombatWindow::Creatures::height + tibia::GuiData::CombatWindow::Creatures::distanceBetweenCreatures) * creaturesNeeded)
                )
            )
            -
            (tibia::GuiData::CombatWindow::Creatures::Window::height - tibia::GuiData::CombatWindow::Creatures::numCreaturePixelsHalfVisible)
        );

        return maxScrollY;
    }

    void doCreatureLogic(int z)
    {
        //return

        for (auto& creature : m_tileMapCreatures[z])
        {
            if (creature->isPlayer() == true || creature->isDead() == true || creature->isSleeping() == true)
            {
                continue;
            }

            sf::Time timeLogic = creature->getClockLogic()->getElapsedTime();

            float randomTime = utility::getRandomNumberFloat(1.0, 2.0);

            if (timeLogic.asSeconds() >= randomTime)
            {
                creature->setIsLogicReady(true);
            }

            if (creature->getIsLogicReady() == false)
            {
                continue;
            }

            int randomShouldMove = utility::getRandomNumber(1, 100);

            if (randomShouldMove > 50)
            {
                int direction = utility::getRandomNumber(tibia::Directions::up, tibia::Directions::left);

                handleCreatureMovement(creature, direction);
            }

            if (creature->getType() == tibia::CreatureTypes::cacodemon && creature->getTeam() == tibia::Teams::evil)
            {
                int randomShouldAttack = utility::getRandomNumber(1, 100);

                if (randomShouldAttack > 25)
                {
                    auto targetCreatureIt = std::find_if
                    (
                        m_tileMapCreatures[z].begin(),
                        m_tileMapCreatures[z].end(),
                        [](tibia::Creature::Ptr const& c)
                        { 
                            return
                                c->isDead()     == false &&
                                c->isSleeping() == false &&
                                c->getTeam()    == tibia::Teams::good;
                        }
                    );

                    if (targetCreatureIt != m_tileMapCreatures[z].end())
                    {
                        spawnProjectile
                        (
                            creature,
                            tibia::ProjectileTypes::cacodemon,
                            creature->getDirection(),
                            sf::Vector2f(creature->getTileX(), creature->getTileY()),
                            sf::Vector2f
                            (
                                targetCreatureIt->get()->getTileX(),
                                targetCreatureIt->get()->getTileY()
                            ),
                            creature->getZ()
                        );
                    }
                }
            }

            creature->setIsLogicReady(false);

            creature->getClockLogic()->restart();
        }
    }

    bool doPlayerInteractWithPlayerTileObjects()
    {
        tibia::Tile::Ptr playerTile = getPlayerTile();

        if (playerTile == nullptr)
        {
            return false;
        }

        bool result = false;

        if (playerTile->getObjectList()->size())
        {
            result = doCreatureInteractWithTileObjects(m_player, playerTile);
        }
        else
        {
            result = doCreatureInteractWithTileObjects(m_player, getTileByCreatureDirection(m_player));
        }

        return result;
    }

    void doPlayerClickToMove()
    {
        if (m_map.isTilePositionOutOfBounds(getMouseTilePosition()) == true)
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

    tibia::Tile::Properties_t getTileProperties(tibia::Tile::Ptr tile)
    {
        tibia::Tile::Properties_t tileProperties;

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
        }

        tibia::Object::List* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getFlags() & tibia::SpriteFlags::solid)
            {
                tileProperties.hasSolidObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::blockProjectiles)
            {
                tileProperties.hasBlockProjectilesObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::hasHeight)
            {
                tileProperties.hasHasHeightObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::moveAbove)
            {
                tileProperties.hasMoveAboveObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::moveBelow)
            {
                tileProperties.hasMoveBelowObject = true;
            }

            if (object->getFlags() & tibia::SpriteFlags::modifyHpOnTouch)
            {
                tileProperties.hasModifyHpOnTouchObject = true;
            }

            if (object->getType() == tibia::ObjectTypes::teleporter)
            {
                tileProperties.hasTeleporterObject = true;
            }

            if (object->getId() == tibia::SpriteData::mountainRampLeft)
            {
                tileProperties.hasMountainRampLeftObject = true;
            }
            else if (object->getId() == tibia::SpriteData::mountainRampRight)
            {
                tileProperties.hasMountainRampRightObject = true;
            }
        }

        tibia::Creature::List* creatureList = tile->getCreatureList();

        for (auto& creature : *creatureList)
        {
            if (creature->isDead() == false && creature->isSleeping() == false)
            {
                tileProperties.hasSolidCreature = true;
                break; // break for now since only checking for one thing with creatures
            }
        }

        return tileProperties;
    }

    tibia::Tile::Ptr getTile(sf::Vector2u tileCoords, int z)
    {
        int tileNumber = m_map.getTileNumberByTileCoords(tileCoords);

        tibia::Tile::List* tileList = m_map.tileMapTiles[z].getTileList();

        return tileList->at(tileNumber);
    }

    tibia::Tile::Ptr getPlayerTile()
    {
        return getTile(m_player->getTilePosition(), m_player->getZ());
    }

    tibia::Tile::Ptr getTileByTileDirection(tibia::Tile::Ptr tile, int direction)
    {
        sf::Vector2u position = tile->getPosition();

        sf::Vector2i normal = tibia::Utility::getVectorByDirection(direction);

        position.x += normal.x * tibia::TILE_SIZE;
        position.y += normal.y * tibia::TILE_SIZE;

        return getTile(position, tile->getZ());
    }

    tibia::Tile::Ptr getTileByThingDirection(tibia::Thing::Ptr thing, int direction)
    {
        sf::Vector2u position = thing->getTilePosition();

        sf::Vector2i normal = tibia::Utility::getVectorByDirection(direction);

        position.x += normal.x * tibia::TILE_SIZE;
        position.y += normal.y * tibia::TILE_SIZE;

        return getTile(position, thing->getZ());
    }

    tibia::Tile::Ptr getTileByCreatureDirection(tibia::Creature::Ptr creature)
    {
        return getTileByThingDirection(creature, creature->getDirection());
    }

    tibia::Tile::Ptr getTileForMoveAbove(sf::Vector2u toTileCoords, int toZ)
    {
        tibia::Tile::Ptr tile = getTile(sf::Vector2u(toTileCoords.x - tibia::TILE_SIZE, toTileCoords.y - (tibia::TILE_SIZE * 2)), toZ);

        return tile;
    }

    tibia::Tile::Ptr getTileForMoveBelow(sf::Vector2u toTileCoords, int toZ)
    {
        tibia::Tile::Ptr tile = getTile(sf::Vector2u(toTileCoords.x + tibia::TILE_SIZE, toTileCoords.y + tibia::TILE_SIZE), toZ);

        tibia::Object::List* objectList = tile->getObjectList();

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

    tibia::Object::Ptr getTileObjectById(sf::Vector2u tileCoords, int z, int id)
    {
        tibia::Tile::Ptr tile = getTile(tileCoords, z);

        if (tile == nullptr)
        {
            return nullptr;
        }

        tibia::Object::List* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getId() == id)
            {
                return object;
            }
        }

        return nullptr;
    }

    tibia::Object::Ptr getTileObjectByType(sf::Vector2u tileCoords, int z, int type)
    {
        tibia::Tile::Ptr tile = getTile(tileCoords, z);

        if (tile == nullptr)
        {
            return nullptr;
        }

        tibia::Object::List* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getType() == type)
            {
                return object;
            }
        }

        return nullptr;
    }

    tibia::Object::Ptr getTileObjectByFlag(sf::Vector2u tileCoords, int z, unsigned long flag)
    {
        tibia::Tile::Ptr tile = getTile(tileCoords, z);

        if (tile == nullptr)
        {
            return nullptr;
        }

        tibia::Object::List* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getFlags() & flag)
            {
                return object;
            }
        }

        return nullptr;
    }

    bool doMoveThingToTile
    (
        tibia::Thing::Ptr thing,
        tibia::Tile::Ptr toTile,

        int direction = tibia::Directions::up,

        bool ignoreAllCollision          = false,
        bool ignoreTileCollision         = false,
        bool ignoreTileObjectCollision   = false,
        bool ignoreTileCreatureCollision = false
    )
    {
        tibia::Tile::Ptr fromTile = getThingTile(thing);

        if (fromTile == nullptr || toTile == nullptr)
        {
            return false;
        }

        tibia::Creature::Ptr thingCreature = std::dynamic_pointer_cast<tibia::Creature>(thing);
        tibia::Object::Ptr   thingObject   = std::dynamic_pointer_cast<tibia::Object>(thing);

        tibia::Creature::List::iterator thingCreatureIt;
        tibia::Object::List::iterator   thingObjectIt;

        tibia::Creature::List* thingCreatureList = fromTile->getCreatureList();
        tibia::Object::List*   thingObjectList   = fromTile->getObjectList();

        if (thingCreature != nullptr)
        {
            if (thingCreatureList->size() == 0)
            {
                return false;
            }

            thingCreatureIt = std::find(thingCreatureList->begin(), thingCreatureList->end(), thingCreature);

            if (thingCreatureIt == thingCreatureList->end())
            {
                return false;
            }
        }

        if (thingObject != nullptr)
        {
            if (thingObjectList->size() == 0)
            {
                return false;
            }

            thingObjectIt = std::find(thingObjectList->begin(), thingObjectList->end(), thingObject);

            if (thingObjectIt == thingObjectList->end())
            {
                return false;
            }
        }

        tibia::Tile::Properties_t toTileProperties = getTileProperties(toTile);

        tibia::Object::Ptr teleporterObject = getTileObjectByType(toTile->getPosition(), toTile->getZ(), tibia::ObjectTypes::teleporter);

        int toZ = toTile->getZ();

        bool isTileClimbDown = false;

        if
        (
            fromTile->getZ() != tibia::ZAxis::ground &&
            fromTile->getZ() != tibia::ZAxis::min    &&

            toTile->getId() == tibia::TILE_NULL
        )
        {
            int climbDownToZ = toZ - 1;

            tibia::Tile::Ptr climbDownToTile;

            if (direction == tibia::Directions::down || direction == tibia::Directions::right)
            {
                climbDownToTile = getTile(sf::Vector2u(toTile->getPosition().x + tibia::TILE_SIZE, toTile->getPosition().y + tibia::TILE_SIZE), climbDownToZ);
            }
            else if (direction == tibia::Directions::left)
            {
                climbDownToTile = getTile(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y + tibia::TILE_SIZE), climbDownToZ);
            }
            else if (direction == tibia::Directions::up)
            {
                climbDownToTile = getTile(sf::Vector2u(toTile->getPosition().x + tibia::TILE_SIZE, toTile->getPosition().y), climbDownToZ);
            }

            if (climbDownToTile != nullptr)
            {
                if (climbDownToTile->getHeight() >= tibia::TILE_CLIMB_HEIGHT)
                {
                    toZ = climbDownToZ;

                    toTile = climbDownToTile;

                    ignoreAllCollision = false;

                    isTileClimbDown = true;
                }
            }
        }

        bool isTileClimbUp = false;

        if
        (
            isTileClimbDown == false &&

            fromTile->getZ() != tibia::ZAxis::underGround &&
            fromTile->getZ() != tibia::ZAxis::max         &&

            fromTile->getHeight() >= tibia::TILE_CLIMB_HEIGHT &&

            findTileAboveThing(thing, thing->getZ() + 1) == false
        )
        {
            int climbUpToZ = toZ + 1;

            tibia::Tile::Ptr climbUpToTile;

            if (direction == tibia::Directions::up || direction == tibia::Directions::left)
            {
                climbUpToTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y - tibia::TILE_SIZE), climbUpToZ);
            }
            else if (direction == tibia::Directions::right)
            {
                climbUpToTile = getTile(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y - tibia::TILE_SIZE), climbUpToZ);
            }
            else if (direction == tibia::Directions::down)
            {
                climbUpToTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y), climbUpToZ);
            }

            tibia::Tile::Ptr aboveThingTile = getTile(thing->getTilePosition(), climbUpToZ);

            if (climbUpToTile != nullptr)
            {
                if (aboveThingTile->getId() == tibia::TILE_NULL && climbUpToTile->getId() != tibia::TILE_NULL)
                {
                    tibia::Tile::Properties_t climbUpToTileProperties = getTileProperties(climbUpToTile);

                    if(climbUpToTileProperties.hasSolidObject == false) // && climbUpToTile->getHeight() < tibia::TILE_HEIGHT_MOVEMENT_DIFFERENCE
                    {
                        toZ = climbUpToZ;

                        toTile = climbUpToTile;

                        ignoreAllCollision = false;

                        isTileClimbUp = true;
                    }
                }
            }
        }

        bool isTileMountainRampDown = false;

        if
        (
            isTileClimbDown == false &&
            isTileClimbUp   == false &&

            toTile->getId() == tibia::TILE_NULL
        )
        {
            int mountainRampDownToZ = toZ - 1;

            tibia::Tile::Ptr mountainRampDownToTile = getTile(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y + tibia::TILE_SIZE), mountainRampDownToZ);

            if (mountainRampDownToTile != nullptr)
            {
                if (mountainRampDownToTile->getId() != tibia::TILE_NULL)
                {
                    tibia::Tile::Properties_t mountainRampDownToTileProperties = getTileProperties(mountainRampDownToTile);

                    if
                    (
                        (mountainRampDownToTileProperties.hasMountainRampLeftObject  == true && direction == tibia::Directions::left)  ||
                        (mountainRampDownToTileProperties.hasMountainRampRightObject == true && direction == tibia::Directions::right)
                    )
                    {
                        if (direction == tibia::Directions::left)
                        {
                            mountainRampDownToTile = getTile(sf::Vector2u(toTile->getPosition().x - tibia::TILE_SIZE, toTile->getPosition().y + tibia::TILE_SIZE), mountainRampDownToZ);
                        }
                        else if (direction == tibia::Directions::right)
                        {
                            mountainRampDownToTile = getTile(sf::Vector2u(toTile->getPosition().x + tibia::TILE_SIZE, toTile->getPosition().y + tibia::TILE_SIZE), mountainRampDownToZ);
                        }

                        if (mountainRampDownToTile != nullptr)
                        {
                            toZ = mountainRampDownToZ;

                            toTile = mountainRampDownToTile;

                            ignoreAllCollision = true;

                            isTileMountainRampDown = true;
                        }
                    }
                }
            }
        }

        if (isTileClimbDown == false && isTileClimbUp == false && isTileMountainRampDown == false)
        {
            if (toTileProperties.isMoveAbove == true || toTileProperties.hasMoveAboveObject == true)
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

                ignoreAllCollision = true;
            }
            else if (toTileProperties.isMoveBelow == true || toTileProperties.hasMoveBelowObject == true)
            {
                toZ = toZ - 1;

                // update tile for falling in grass hole
                if (toTile->getId() == tibia::SpriteData::stepTileGrassHole[0])
                {
                    toTile->setId(tibia::SpriteData::stepTileGrassHole[1]);

                    toTile->setFlags(tibia::UMaps::spriteFlags[tibia::SpriteData::stepTileGrassHole[1]]);
                }

                toTile = getTileForMoveBelow(sf::Vector2u(toTile->getPosition().x, toTile->getPosition().y), toZ);
                
                direction = tibia::Directions::down;

                ignoreAllCollision = true;
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

                ignoreAllCollision = true;
            }
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
            toTileProperties = getTileProperties(toTile);

            if (ignoreTileCollision == false && toTileProperties.isSolid == true)
            {
                return false;
            }

            if (ignoreTileObjectCollision == false && toTileProperties.hasSolidObject == true)
            {
                return false;
            }

            if (ignoreTileCreatureCollision == false && toTileProperties.hasSolidCreature == true)
            {
                return false;
            }

            if (thingCreature != nullptr)
            {
                if (toTile->getZ() == fromTile->getZ())
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

            if (thingCreature != m_player) // prevent player hearing the sound twice
            {
                spawnSound(thing->getTilePosition(), thing->getZ(), tibia::Sounds::teleport);
            }
        }

        if (thingCreature != nullptr)
        {
            toTile->addCreature(thingCreature);

            thingCreatureList = fromTile->getCreatureList();

            thingCreatureIt = std::find(thingCreatureList->begin(), thingCreatureList->end(), thingCreature);

            if (thingCreatureIt != thingCreatureList->end())
            {
                thingCreatureList->erase(thingCreatureIt);
            }

            thingCreature->doTurn(direction);

            thingCreature->update();
        }

        if (thingObject != nullptr)
        {
            toTile->addObject(thingObject);

            thingObjectList = fromTile->getObjectList();

            thingObjectIt = std::find(thingObjectList->begin(), thingObjectList->end(), thingObject);

            if (thingObjectIt != thingObjectList->end())
            {
                thingObjectList->erase(thingObjectIt);
            }

            thingObject->update();
        }

        thing->setTileCoords(toTile->getPosition().x, toTile->getPosition().y);

        thing->setZ(toZ);

        updateStepTile(fromTile);
        updateStepTile(toTile);

        if (teleporterObject != nullptr)
        {
            spawnAnimation(toTile->getPosition(), toZ, tibia::Animations::spellBlue);

            spawnSound(toTile->getPosition(), toZ, tibia::Sounds::teleport);
        }

        if (thingCreature != nullptr)
        {
            tibia::Object::Ptr damageOnTouchObject = getTileObjectByFlag(toTile->getPosition(), toTile->getZ(), tibia::SpriteFlags::modifyHpOnTouch);

            if (damageOnTouchObject != nullptr && toTileProperties.hasModifyHpOnTouchObject == true)
            {
                handleCreatureModifyHp
                (
                    nullptr,
                    thingCreature,
                    -(tibia::UMaps::modifyHpOnTouchDamages[damageOnTouchObject->getId()]),
                    tibia::UMaps::modifyHpOnTouchTypes[damageOnTouchObject->getId()]
                );

                int statusEffectType = tibia::UMaps::modifyHpOnTouchStatusEffects[damageOnTouchObject->getId()];

                thingCreature->addStatusEffect
                (
                    tibia::UMaps::modifyHpOnTouchStatusEffects[damageOnTouchObject->getId()],
                    tibia::UMaps::creatureStatusEffectsDamages[statusEffectType],
                    tibia::UMaps::creatureStatusEffectsTicks[statusEffectType],
                    tibia::UMaps::creatureStatusEffectsTimesPerTick[statusEffectType]
                );

                std::vector<int> animationId = tibia::UMaps::modifyHpOnTouchAnimations[damageOnTouchObject->getId()];

                spawnAnimation(toTile->getPosition(), toTile->getZ(), animationId);

                if (thingCreature == m_player)
                {
                    std::string statusEffectName = tibia::UMaps::creatureStatusEffectsNames[statusEffectType];

                    std::transform(statusEffectName.begin(), statusEffectName.end(), statusEffectName.begin(), std::tolower);

                    std::stringstream ssStatusEffectMessage;
                    ssStatusEffectMessage << "You are " << statusEffectName << ".";

                    showStatusBarText(ssStatusEffectMessage.str());
                }
            }
        }

        return true;
    }

    bool doCreatureInteractWithTileObjects(tibia::Creature::Ptr creature, tibia::Tile::Ptr tile)
    {
        if (creature == nullptr || tile == nullptr)
        {
            return false;
        }

        tibia::Object::List* objectList = tile->getObjectList();

        if (objectList->size() == 0)
        {
            return false;
        }

        bool result = false;

        result = doCreatureInteractWithObject(creature, objectList->back());

        if (result == false)
        {
            for (auto& object : *objectList)
            {
                if (doCreatureInteractWithObject(creature, object) == true)
                {
                    return true;
                }
            }
        }

        return result;
    }

    bool doCreaturePushObject(tibia::Creature::Ptr creature, tibia::Object::Ptr object)
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

        tibia::Tile::Ptr fromTile = getThingTile(object);

        if (fromTile == nullptr)
        {
            return false;
        }

        tibia::Tile::Ptr toTile = getTileByTileDirection(fromTile, m_player->getDirection());

        if (toTile == nullptr)
        {
            return false;
        }

        bool ignoreTileObjectCollision = false;

        tibia::Tile::Properties_t toTileProperties = getTileProperties(toTile);

        if ((object->getFlags() & tibia::SpriteFlags::solid) == false && toTileProperties.hasHasHeightObject == true)
        {
            ignoreTileObjectCollision = true;
        }

        bool ignoreTileCreatureCollision = false;

        if ((object->getFlags() & tibia::SpriteFlags::solid) == false)
        {
            ignoreTileCreatureCollision = true;
        }

        bool result = doMoveThingToTile(object, toTile, creature->getDirection(), false, false, ignoreTileObjectCollision, ignoreTileCreatureCollision);

        if (result == false)
        {
            // push solid object to opposite side of player
            if (object->getFlags() & tibia::SpriteFlags::solid)
            {
                tibia::Tile::Ptr toTile = getTileByCreatureDirection(m_player);

                if (toTile == nullptr)
                {
                    return false;
                }

                tibia::Tile::Properties_t toTileProperties = getTileProperties(toTile);

                if ((object->getFlags() & tibia::SpriteFlags::solid) == false && toTileProperties.hasHasHeightObject == true)
                {
                    ignoreTileObjectCollision = true;
                }

                result = doMoveThingToTile(object, toTile, creature->getDirection(), false, false, ignoreTileObjectCollision, ignoreTileCreatureCollision);
            }
        }

        return true;
    }

    bool doesCreatureHaveDoorKey(tibia::Creature::Ptr creature, int keyId)
    {
        if (keyId == tibia::KeyTypes::none) // unlocked
        {
            return true;
        }

        if (creature->hasInventoryItem(tibia::SpriteData::keyRing) == true) // key ring opens all doors
        {
            return true;
        }

        if (creature->hasInventoryItem(tibia::UMaps::keyIds[keyId]) == false)
        {
            std::stringstream ssKeyText;

            ssKeyText << "You need a " << tibia::UMaps::keyTypesStrings[keyId];

            if (keyId != tibia::KeyTypes::crowbar)
            {
                ssKeyText << " key";
            }

            ssKeyText << " to open this door.";

            showStatusBarText(ssKeyText.str());

            return false;
        }

        return true;
    }

    bool doCreatureInteractWithObject(tibia::Creature::Ptr creature, tibia::Object::Ptr object)
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
            if (creature == m_player)
            {
                showStatusBarText("Object is too far away.");
            }

            return false;
        }

        // signs
        if (object->getType() == tibia::ObjectTypes::sign)
        {
            std::string signName;

            if (object->properties.signName.size() == 0)
            {
                signName = tibia::OBJECT_SIGN_NAME_DEFAULT;
            }
            else
            {
                signName = object->properties.signName;
            }

            std::stringstream signText;

            signText << "You see " << signName << ".";

            if (object->properties.signText.size() == 0)
            {
                signText << " It is blank." << std::endl;
            }
            else
            {
                signText << " It reads:\n" << object->properties.signText;
            }

            std::cout << signText.str() << std::endl;

            showGameWindowText(signText.str(), tibia::Colors::Text::green);

            return true;
        }

        // books
        if (object->getType() == tibia::ObjectTypes::book)
        {
            std::string bookName;

            if (object->properties.bookName.size() == 0)
            {
                bookName = tibia::OBJECT_BOOK_NAME_DEFAULT;
            }
            else
            {
                bookName = object->properties.bookName;
            }

            std::stringstream bookText;

            bookText << "You see " << bookName << ".";

            if (object->properties.bookText.size() == 0)
            {
                bookText << " It is blank." << std::endl;
            }
            else
            {
                bookText << " It reads:\n" << object->properties.bookText;
            }

            std::cout << bookText.str() << std::endl;

            IupGetText("Book", (char*)object->properties.bookText.c_str());

            return true;
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

        // doors
        if (object->getType() == tibia::ObjectTypes::door)
        {
            // do not let door close on creatures
            tibia::Tile::Ptr doorTile = getTile(object->getTilePosition(), object->getZ());

            if (doorTile->getCreatureList()->size() != 0)
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
            if (object->getId() == tibia::SpriteData::doorLockedVertical[0])
            {
                if (doesCreatureHaveDoorKey(creature, object->properties.doorKey) == false)
                {
                    return false;
                }

                object->setId(tibia::SpriteData::doorLockedVertical[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorLockedVertical[1])
            {
                object->setId(tibia::SpriteData::doorLockedVertical[0]);
            }

            // door locked horizontal
            if (object->getId() == tibia::SpriteData::doorLockedHorizontal[0])
            {
                if (doesCreatureHaveDoorKey(creature, object->properties.doorKey) == false)
                {
                    return false;
                }

                object->setId(tibia::SpriteData::doorLockedHorizontal[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorLockedHorizontal[1])
            {
                object->setId(tibia::SpriteData::doorLockedHorizontal[0]);
            }

            // door fence small
            if (object->getId() == tibia::SpriteData::doorFenceSmall[0])
            {
                object->setId(tibia::SpriteData::doorFenceSmall[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorFenceSmall[1])
            {
                object->setId(tibia::SpriteData::doorFenceSmall[0]);
            }

            // door fence large
            if (object->getId() == tibia::SpriteData::doorFenceLarge[0])
            {
                object->setId(tibia::SpriteData::doorFenceLarge[1]);
            }
            else if (object->getId() == tibia::SpriteData::doorFenceLarge[1])
            {
                object->setId(tibia::SpriteData::doorFenceLarge[0]);
            }

            return true;
        }

        // ladder and rope up
        if (object->getId() == tibia::SpriteData::ladder || object->getId() == tibia::SpriteData::ropeUp)
        {
            if (object->getId() == tibia::SpriteData::ropeUp && creature->hasInventoryItem(tibia::SpriteData::rope) == false)
            {
                showStatusBarText("You need a rope to climb up.");
                return false;
            }

            tibia::Tile::Ptr toTile = getTileForMoveAbove(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() + 1);

            doMoveThingToTile(creature, toTile, tibia::Directions::up, true);

            return true;
        }

        // sewer grate
        if (object->getId() == tibia::SpriteData::sewerGrate)
        {
            tibia::Tile::Ptr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), creature->getZ() - 1);

            doMoveThingToTile(creature, toTile, tibia::Directions::down, true);

            return true;
        }

        // dig holes
        if (object->getId() == tibia::SpriteData::digHole[0] || object->getId() == tibia::SpriteData::digHoleIce[0])
        {
            if (creature->hasInventoryItem(tibia::SpriteData::shovel) == false && creature->hasInventoryItem(tibia::SpriteData::pick) == false)
            {
                showStatusBarText("You need a shovel or pick to dig.");
                return false;
            }

            if (object->getId() == tibia::SpriteData::digHole[0])
            {
                object->getClockRegenerate()->restart();
                object->setId(tibia::SpriteData::digHole[1]);
            }
            else if (object->getId() == tibia::SpriteData::digHoleIce[0])
            {
                object->getClockRegenerate()->restart();
                object->setId(tibia::SpriteData::digHoleIce[1]);
            }

            // things fall down the hole if it is dug while things are on top of it
            tibia::Tile::Ptr fromTile = getThingTile(object);

            tibia::Tile::Ptr toTile = getTileForMoveBelow(sf::Vector2u(object->getTileX(), object->getTileY()), object->getZ() - 1);

            if (fromTile != nullptr && toTile != nullptr)
            {
                tibia::Thing::List fallThingList;

                tibia::Creature::List* fromCreatureList = fromTile->getCreatureList();

                for (auto fromCreature : *fromCreatureList)
                {
                    fallThingList.push_back(fromCreature);
                }

                tibia::Object::List* fromObjectList = fromTile->getObjectList();

                for (auto fromObject : *fromObjectList)
                {
                    // do not let the dig hole fall into itself
                    if (fromObject == object)
                    {
                        continue;
                    }

                    fallThingList.push_back(fromObject);
                }

                // need to use fall thing list because moving things modifies from thing list
                for (auto fallThing : fallThingList)
                {
                    doMoveThingToTile(fallThing, toTile, tibia::Directions::down, true);
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

                tibia::Object::Ptr nextObject = getTileObjectById
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

        // blueberry bushes
        if (object->getId() == tibia::SpriteData::bushBlueberry)
        {
            tibia::Object::Ptr blueBerriesObject = std::make_shared<tibia::Object>
            (
                m_player->getTilePosition(),
                m_player->getZ(),
                tibia::SpriteData::blueBerries.at(4)
            );

            creature->addInventoryItem(blueBerriesObject->getId(), blueBerriesObject->getCount(), blueBerriesObject->getFlags());

            object->getClockRegenerate()->restart();
            object->setId(tibia::SpriteData::bushBlueberryEmpty);

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
        //else if (object->getId() == tibia::SpriteData::torchBig[2])
        //{
            // this torch gets picked up only

            //return true;
        //}
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
            tibia::Tile::Ptr toTile = getTile(sf::Vector2u(object->getTilePosition().x, object->getTilePosition().y - tibia::TILE_SIZE), object->getZ());

            // do not open the counter if there are objects on the tile the counter is moving to
            if (toTile->getObjectList()->size() > 1)
            {
                if (creature == m_player)
                {
                    showStatusBarText("The counter top is blocked by an object.");
                }

                return false;
            }

            doMoveThingToTile(object, toTile, tibia::Directions::null, true);

            object->setId(tibia::SpriteData::counterVertical[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::counterVertical[1])
        {
            tibia::Tile::Ptr toTile = getTile(sf::Vector2u(object->getTilePosition().x, object->getTilePosition().y + tibia::TILE_SIZE), object->getZ());

            // do not close the counter on solid objects or creatures
            tibia::Tile::Properties_t tileProperties = getTileProperties(toTile);
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
            tibia::Tile::Ptr toTile = getTile(sf::Vector2u(object->getTilePosition().x - tibia::TILE_SIZE, object->getTilePosition().y), object->getZ());

            // do not open the counter if there are objects on the tile the counter is moving to
            if (toTile->getObjectList()->size() > 1)
            {
                if (creature == m_player)
                {
                    showStatusBarText("The counter top is blocked by an object.");
                }

                return false;
            }

            doMoveThingToTile(object, toTile, tibia::Directions::null, true);

            object->setId(tibia::SpriteData::counterHorizontal[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::counterHorizontal[1])
        {
            tibia::Tile::Ptr toTile = getTile(sf::Vector2u(object->getTilePosition().x + tibia::TILE_SIZE, object->getTilePosition().y), object->getZ());

            // do not close the counter on solid objects or creatures
            tibia::Tile::Properties_t tileProperties = getTileProperties(toTile);
            if (tileProperties.hasSolidObject == true || tileProperties.hasSolidCreature == true)
            {
                return false;
            }

            doMoveThingToTile(object, toTile);

            object->setId(tibia::SpriteData::counterHorizontal[0]);

            return true;
        }

        // chair (rotates)
        auto chairIt = std::find(tibia::SpriteData::chair.begin(), tibia::SpriteData::chair.end(), object->getId());

        if (chairIt != tibia::SpriteData::chair.end())
        {
            chairIt++;

            if (chairIt == tibia::SpriteData::chair.end())
            {
                chairIt = tibia::SpriteData::chair.begin();
            }

            object->setId(tibia::SpriteData::chair[chairIt - tibia::SpriteData::chair.begin()]);

            return true;
        }

        // chair throne (rotates)
        if (object->getId() == tibia::SpriteData::chairThrone[0])
        {
            object->setId(tibia::SpriteData::chairThrone[1]);

            return true;
        }
        else if (object->getId() == tibia::SpriteData::chairThrone[1])
        {
            object->setId(tibia::SpriteData::chairThrone[0]);

            return true;
        }

        // jungle grass
        if (object->getId() == tibia::SpriteData::grassJungle)
        {
            if (creature->hasInventoryItem(tibia::SpriteData::machete) == false)
            {
                showStatusBarText("You need a machete to cut the grass.");
                return false;
            }

            object->getClockRegenerate()->restart();
            object->setId(tibia::SpriteData::grassJungleCut);

            return true;
        }

        // wheat
        if (object->getId() == tibia::SpriteData::wheatGreen || object->getId() == tibia::SpriteData::wheatYellow)
        {
            if (creature->hasInventoryItem(tibia::SpriteData::scythe) == false)
            {
                showStatusBarText("You need a scythe to cut the wheat.");
                return false;
            }

            if (object->getId() == tibia::SpriteData::wheatGreen)
            {
                object->getClockRegenerate()->restart();
                object->setId(tibia::SpriteData::wheatCut);
            }
            else if (object->getId() == tibia::SpriteData::wheatYellow)
            {
                object->getClockRegenerate()->restart();
                object->setId(tibia::SpriteData::wheatCut);

                spawnObject
                (
                    object->getTilePosition(),
                    object->getZ(),
                    tibia::SpriteData::wheat
                );
            }

            return true;
        }

        return false;
    }

    void doCreatureDropInventoryItem(tibia::Creature::Ptr creature, tibia::Creature::InventoryItemList::iterator& inventoryItemIt)
    {
        tibia::Object::Ptr droppedObject = spawnObject(creature->getTilePosition(), creature->getZ(), inventoryItemIt->id);

        if (inventoryItemIt->count > 1)
        {
            if (inventoryItemIt->flags & tibia::SpriteFlags::groupable)
            {
                droppedObject->setCount(inventoryItemIt->count);

                creature->removeInventoryItem(inventoryItemIt);
            }
            else
            {
                inventoryItemIt->count--;
            }
        }
        else
        {
            creature->removeInventoryItem(inventoryItemIt);
        }
    }

    void doCreatureDropAllInventoryItems(tibia::Creature::Ptr creature)
    {
        tibia::Creature::InventoryItemList* inventoryItemList = creature->getInventoryItemList();

        if (inventoryItemList->size() == 0)
        {
            return;
        }

        for (auto inventoryItemIt = inventoryItemList->begin(); inventoryItemIt != inventoryItemList->end(); ++inventoryItemIt)
        {
            //std::cout << inventoryItemIt->id << std::endl;

            doCreatureDropInventoryItem(creature, inventoryItemIt);

            inventoryItemIt--;
        }
    }

    void doCreatureSpeech(tibia::Creature::Ptr creature, const std::string& text, int speechType = tibia::SpeechTypes::say)
    {
        std::stringstream ssText;

        if (speechType != tibia::SpeechTypes::monster)
        {
            if (creature->isPlayer() == true)
            {
                ssText << "You ";
            }
            else
            {
                ssText << creature->getName() << " ";
            }

            ssText << tibia::UMaps::speechTypes[speechType];

            if (creature->isPlayer() == false)
            {
                ssText << "s";
            }

            ssText << ":\n" << text;
        }

        sf::Vector2u textPosition = creature->getTilePosition();

        textPosition.x += tibia::TILE_SIZE / 2;

        textPosition.y -= ((2 * m_bitmapFontDefault.getGlyphSize()->y) + 8);

        spawnGameText
        (
            textPosition,
            creature->getZ(),
            ssText.str(),
            tibia::UMaps::speechTypesTextColors[speechType]
        );

        std::string chatLogWindowText = ssText.str();

        boost::replace_all(chatLogWindowText, "\n", " ");

        doChatLogWindowAddText(chatLogWindowText);
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

        // highlight objects and creatures
        if (m_mouseTile != nullptr)
        {
            tibia::Object::List* objectList = m_mouseTile->getObjectList();

            for (auto& object : *objectList)
            {
                if (object->getFlags() & tibia::SpriteFlags::interactive)
                {
                    m_mouseCursor.setColor(tibia::Colors::MouseCursor::pink);
                    setMouseCursorType(tibia::MouseCursorTypes::open);
                    break;
                }
            }

            tibia::Creature::List* creatureList = m_mouseTile->getCreatureList();

            for (auto& creature : *creatureList)
            {
                if (creature->getTeam() == tibia::Teams::evil)
                {
                    m_mouseCursor.setColor(tibia::Colors::MouseCursor::red);
                    setMouseCursorType(tibia::MouseCursorTypes::open);
                    break;
                }
            }
        }

        mainWindow->draw(m_mouseCursor);
    }

    bool isCreatureMovementOutOfBounds(tibia::Creature::Ptr creature, int direction)
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
            if (creature->getX() == m_map.getWidth() - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::down)
        {
            if (creature->getY() == m_map.getHeight() - 1)
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
            if (creature->getY() == 0 || creature->getX() == m_map.getWidth() - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::downLeft)
        {
            if (creature->getX() == 0 || creature->getY() == m_map.getHeight() - 1)
            {
                return true;
            }
        }
        else if (direction == tibia::Directions::downRight)
        {
            if (creature->getX() == m_map.getWidth() - 1 || creature->getY() == m_map.getHeight() - 1)
            {
                return true;
            }
        }

        return false;
    }

    void handleCreatureMovement(tibia::Creature::Ptr creature, int direction, bool turnOnly = false)
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

        tibia::Tile::Ptr toTile = getThingTileByMovement(creature, direction);

        doMoveThingToTile(creature, toTile, direction);

        creature->setIsMovementReady(false);

        creature->getClockMovement()->restart();

        creature->update();
    }

    bool handleCreatureModifyHp(tibia::Creature::Ptr attacker, tibia::Creature::Ptr defender, int hpChange, int modifyHpType)
    {
        if (defender == nullptr)
        {
            return false;
        }

        if (attacker != nullptr)
        {
            if (attacker->isDead() == true || attacker->isSleeping() == true)
            {
                return false;
            }
        }

        // neutral team cannot be damaged or healed
        if (defender->getTeam() == tibia::Teams::neutral)
        {
            return false;
        }

        if (defender->isDead() == true || defender->isSleeping() == true)
        {
            return false;
        }

        bool isDamage = false;

        if (hpChange < 0)
        {
            isDamage = true;
        }

        if (attacker != nullptr)
        {
            if (isDamage == true)
            {
                // do not let creature damage itself
                if (attacker == defender)
                {
                    return false;
                }

                // friendly fire
                if (attacker->getTeam() == defender->getTeam())
                {
                    if (this->options.isGameFriendlyFireEnabled == false)
                    {
                        return false;
                    }
                }

                defender->setLastAttacker(attacker);
            }
            else
            {
                // cannot heal the other team
                if (attacker->getTeam() != defender->getTeam())
                {
                    return false;
                }
            }
        }

        if (defender == m_player && this->options.isCheatInfiniteHealthEnabled == true)
        {
            hpChange = 0;

            defender->setHp(defender->getHpMax());
        }

        defender->modifyHp(hpChange);

        if (hpChange != 0)
        {
            sf::Color textColor = tibia::Colors::Text::white;

            auto textColorIt = tibia::UMaps::modifyHpTextColors.find(modifyHpType);

            if (textColorIt != tibia::UMaps::modifyHpTextColors.end())
            {
                textColor = textColorIt->second;
            }

            spawnFloatingText
            (
                defender->getTilePosition(),
                defender->getZ(),
                std::to_string(hpChange),
                textColor
            );
        }

        std::vector<int> animation;

        if (modifyHpType == tibia::ModifyHpTypes::blood)
        {
            animation = tibia::UMaps::creatureBloodTypeAnimations[defender->getBloodType()];
        }
        else
        {
            animation = tibia::UMaps::modifyHpAnimations[modifyHpType];
        }

        spawnAnimation
        (
            defender->getTilePosition(),
            defender->getZ(),
            animation
        );

        if (hpChange != 0)
        {
            if (attacker != nullptr)
            {
                if (attacker == m_player)
                {
                    std::cout << "You";
                }
                else
                {
                    std::cout << attacker->getName();
                }

                if (isDamage == true)
                {
                    std::cout << " damaged ";
                }
                else
                {
                    std::cout << " healed ";
                }

                std::cout
                    << defender->getName()
                    << "for "
                    << std::abs(hpChange)
                    << " points of health."
                    << std::endl;
            }
            else
            {
                std::cout << defender->getName();

                std::cout << " was";

                if (isDamage == true)
                {
                    std::cout << " damaged ";
                }
                else
                {
                    std::cout << " healed ";
                }

                std::cout
                    << "for "
                    << std::abs(hpChange)
                    << " points of health."
                    << std::endl;
            }
        }

        if (isDamage == true)
        {
            if (defender->isDead() == false)
            {
                if (defender->getBloodType() != tibia::CreatureBloodTypes::none)
                {
                    if (modifyHpType == tibia::ModifyHpTypes::blood) // prevents spells,etc from creating blood splats
                    {
                        spawnDecayObject
                        (
                            defender->getTilePosition(),
                            defender->getZ(),
                            tibia::UMaps::creatureBloodTypesSplats[defender->getBloodType()],
                            true
                        );
                    }
                }
            }
            else
            {
                if (defender->getBloodType() != tibia::CreatureBloodTypes::none)
                {
                    spawnDecayObject
                    (
                        defender->getTilePosition(),
                        defender->getZ(),
                        tibia::UMaps::creatureBloodTypesPools[defender->getBloodType()],
                        true
                    );
                }

                doCreatureDropAllInventoryItems(defender);

                if (defender->isPlayer() == true)
                {
                    showGameWindowText("You are dead.", tibia::Colors::Text::white);

                    doChatLogWindowAddText("You are dead.", sf::Color::Red);
                    //doChatLogWindowAddText("You will respawn at your home location.", sf::Color::Red);
                }

                auto itDeathSound = tibia::UMaps::creatureDeathSounds.find(defender->getType());

                if (itDeathSound != tibia::UMaps::creatureDeathSounds.end())
                {
                    spawnSound(defender->getTilePosition(), defender->getZ(), itDeathSound->second);
                }

                if (attacker != nullptr)
                {
                    std::cout
                        << attacker->getName()
                        << " killed "
                        << defender->getName()
                        << std::endl;
                }
                else
                {
                    std::cout << defender->getName() << " died." << std::endl;
                }
            }
        }

        return true;
    }

    void updateStepTile(tibia::Tile::Ptr tile)
    {
        tibia::Object::List* objectList = tile->getObjectList();

        tibia::Creature::List* creatureList = tile->getCreatureList();

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

    void updateTileGroupedObjects(tibia::Tile::Ptr tile)
    {
        tibia::Object::List* objectList = tile->getObjectList();

        if (objectList->size() < 2)
        {
            return;
        }

        tibia::Object::Ptr first = objectList->back();

        tibia::Object::Ptr second = objectList->end()[-2];

        if ((first->getFlags() & tibia::SpriteFlags::groupable) && (second->getFlags() & tibia::SpriteFlags::groupable))
        {
            if (first->getCount() == tibia::INVENTORY_ITEM_COUNT_MAX || second->getCount() == INVENTORY_ITEM_COUNT_MAX)
            {
                return;
            }

            int groupableObjectsIndex = 0;

            for (auto& groupableObjects : tibia::groupedObjectsList)
            {
                for (auto groupableObject : groupableObjects)
                {
                    if (groupableObject == first->getId())
                    {
                        for (auto groupableObject : groupableObjects)
                        {
                            if (groupableObject == second->getId())
                            {
                                int groupedCount = first->getCount() + second->getCount();

                                if (groupedCount > INVENTORY_ITEM_COUNT_MAX)
                                {
                                    int remainderCount = groupedCount - INVENTORY_ITEM_COUNT_MAX;

                                    first->setId
                                    (
                                        tibia::groupedObjectsList
                                            .at(groupableObjectsIndex)
                                            .at(tibia::Utility::getGroupableObjectIndexByCount(remainderCount))
                                    );

                                    first->setCount(remainderCount);

                                    second->setId
                                    (
                                        tibia::groupedObjectsList
                                            .at(groupableObjectsIndex)
                                            .at(tibia::Utility::getGroupableObjectIndexByCount(INVENTORY_ITEM_COUNT_MAX))
                                    );

                                    second->setCount(INVENTORY_ITEM_COUNT_MAX);

                                    return;
                                }

                                first->setId
                                (
                                    tibia::groupedObjectsList
                                        .at(groupableObjectsIndex)
                                        .at(tibia::Utility::getGroupableObjectIndexByCount(groupedCount))
                                );

                                first->setCount(groupedCount);

                                second->setIsReadyForErase(true);

                                return;
                            }
                        }
                    }
                }

                groupableObjectsIndex++;
            }
        }
    }

    void updateTileHeight(tibia::Tile::Ptr tile)
    {
        int height = 0;

        int numOffsetObjects = 0;

        tibia::Object::List* objectList = tile->getObjectList();

        for (auto& object : *objectList)
        {
            if (object->getFlags() & tibia::SpriteFlags::ignoreHeight || object->getFlags() & tibia::SpriteFlags::decal)
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

                if (height >= tibia::TILE_HEIGHT_MAX) height = tibia::TILE_HEIGHT_MAX;

                if (object->getFlags() & tibia::SpriteFlags::offset)
                {
                    numOffsetObjects++;
                }
            }
        }

        tibia::Creature::List* creatureList = tile->getCreatureList();

        for (auto& creature : *creatureList)
        {
            creature->setDrawOffset(height);
        }

        tibia::Animation::List* animationList = tile->getAnimationList();

        for (auto& animation : *animationList)
        {
            animation->setDrawOffset(height);
        }

        height = height - numOffsetObjects;

        if (height >= tibia::TILE_HEIGHT_MAX) height = tibia::TILE_HEIGHT_MAX;

        tile->setHeight(height);
    }

    void updateSounds()
    {
        for (auto soundList_it = m_soundList.begin(); soundList_it != m_soundList.end(); soundList_it++)
        {
            sf::Sound* sound = soundList_it->get();

            if (sound == nullptr)
            {
                return;
            }

            if (sound->getStatus() == sf::SoundSource::Status::Stopped)
            {
                soundList_it = m_soundList.erase(soundList_it);
                soundList_it--;
                continue;
            }
        }
    }

    void spawnSound(sf::Vector2u tilePosition, int z, sf::SoundBuffer& soundBuffer)
    {
        if (this->options.isAudioSoundEnabled == false)
        {
            return;
        }

        int soundTileDistance = tibia::Utility::calculateTileDistance
        (
            m_player->getTileX(),
            m_player->getTileY(),
            tilePosition.x,
            tilePosition.y
        );

        if (soundTileDistance > tibia::NUM_TILES_X)
        {
            return;
        }

        float soundVolume = 100;

        if (z != m_player->getZ())
        {
            soundVolume = 50;
        }

        soundVolume = tibia::Utility::calculateVolumeByTileDistance(soundVolume, soundTileDistance);

        if (soundVolume <= 0)
        {
            return;
        }

        tibia::Sound::Ptr sound = std::make_shared<tibia::Sound>();
        sound->setBuffer(soundBuffer);
        sound->setVolume(soundVolume);

        sound->play();

        //std::cout << "spawnSound volume: " << sound->getVolume() << std::endl;

        m_soundList.push_back(sound);
    }

    void updateProjectiles(int z)
    {
        tibia::Projectile::List* projectileList = &m_projectileList[z];

        for (auto projectileList_it = projectileList->begin(); projectileList_it != projectileList->end(); projectileList_it++)
        {
            tibia::Projectile::Ptr projectile = *projectileList_it;

            if (projectile == nullptr)
            {
                continue;
            }

            projectile->update(m_timeDelta);

            if (projectile->isReadyForErase() == true)
            {
                projectileList_it = projectileList->erase(projectileList_it);
                projectileList_it--;
                continue;
            }

            sf::Vector2u projectileSpriteTilePosition = projectile->getSpriteTilePosition();

            // out of bounds of map
            if (m_map.isTilePositionOutOfBounds(projectileSpriteTilePosition) == true)
            {
                projectile->setIsReadyForErase(true);
                continue;
            }

            int projectileTileDistanceTravelled = projectile->getTileDistanceTravelled();

            int projectileTileDistanceTravelledPrevious = projectile->getTileDistanceTravelledPrevious();

            if (projectileTileDistanceTravelled == projectileTileDistanceTravelledPrevious)
            {
                continue;
            }

            projectile->setTileDistanceTravelledPrevious(projectileTileDistanceTravelled);

            projectileTileDistanceTravelledPrevious = projectileTileDistanceTravelled;

            tibia::Tile::Ptr projectileTile = getTile(projectileSpriteTilePosition, projectile->getZ());

            // do not let projectile hit its owner
            if (projectileTile->getPosition() == projectile->getCreatureOwner()->getTilePosition())
            {
                continue;
            }

            tibia::Tile::Properties_t projectileTileProperties = getTileProperties(projectileTile);

            tibia::Object::Ptr teleporterObject = getTileObjectByType(projectileTile->getPosition(), projectileTile->getZ(), tibia::ObjectTypes::teleporter);

            if (teleporterObject != nullptr)
            {
                spawnAnimation
                (
                    projectileSpriteTilePosition,
                    projectile->getZ(),
                    tibia::Animations::electricity
                );

                projectile->setIsReadyForErase(true);
                continue;
            }

            tibia::Creature::List* creatureList = projectileTile->getCreatureList();

            if (creatureList->size())
            {
                tibia::Creature::Ptr defender = creatureList->back();

                tibia::Creature::Ptr attacker = projectile->getCreatureOwner();

                bool result = handleCreatureModifyHp(attacker, defender, -projectile->getDamage(), projectile->getModifyHpType());

                if (result == true)
                {
                    if (projectile->getType() == tibia::ProjectileTypes::spear)
                    {
                        spawnObject
                        (
                            projectileSpriteTilePosition,
                            projectile->getZ(),
                            tibia::SpriteData::spear
                        );
                    }

                    projectile->setIsReadyForErase(true);
                    continue;
                }
            }

            if
            (
                projectileTileProperties.isSolid                   == true            ||
                projectileTileProperties.hasBlockProjectilesObject == true            ||
                projectileTile->getHeight() >= tibia::TILE_HEIGHT_MOVEMENT_DIFFERENCE
            )
            {
                if
                (
                    (projectileTile->getFlags() & tibia::SpriteFlags::water) == false &&
                    (projectileTile->getFlags() & tibia::SpriteFlags::lava)  == false
                )
                {
                    spawnAnimation
                    (
                        projectileSpriteTilePosition,
                        projectile->getZ(),
                        tibia::Animations::hitBlock
                    );

                    projectile->setIsReadyForErase(true);
                    continue;
                }
            }

            if (projectile->getTileDistanceTravelled() >= projectile->getRange())
            {
                if (projectileTile->getFlags() & tibia::SpriteFlags::water)
                {
                    spawnAnimation
                    (
                        projectileSpriteTilePosition,
                        projectile->getZ(),
                        tibia::Animations::waterSplash
                    );
                }
                else
                {
                    spawnAnimation
                    (
                        projectileSpriteTilePosition,
                        projectile->getZ(),
                        tibia::Animations::hitMiss
                    );

                    if (projectile->getType() == tibia::ProjectileTypes::spear)
                    {
                        spawnObject
                        (
                            projectileSpriteTilePosition,
                            projectile->getZ(),
                            tibia::SpriteData::spear
                        );
                    }
                }

                projectile->setIsReadyForErase(true);
                continue;
            }
        }
    }

    void spawnProjectile(tibia::Creature::Ptr creature, int projectileType, int direction, sf::Vector2f origin, sf::Vector2f destination, int z, bool isPrecise = false, bool isChild = false)
    {
        tibia::Projectile::Ptr projectile = std::make_shared<tibia::Projectile>(projectileType, direction, origin, destination, isPrecise, isChild);
        projectile->setTileCoords(origin.x, origin.y);
        projectile->setZ(creature->getZ());
        projectile->setCreatureOwner(creature);

        m_projectileList[creature->getZ()].push_back(projectile);
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

    int getMouseInventoryWindowSlotIndex()
    {
        sf::Vector2f slotPosition = m_inventorySlotsWindow.mapPixelToCoords(getMouseWindowPosition(), m_inventorySlotsWindowView);

        slotPosition.x -= tibia::GuiData::InventoryWindow::Slots::Window::x;
        slotPosition.y -= tibia::GuiData::InventoryWindow::Slots::Window::y;

        if (slotPosition.x < 0) slotPosition.x = 0;
        if (slotPosition.y < 0) slotPosition.y = 0;

        sf::Vector2u slotPositionActual;
        slotPositionActual.x = slotPosition.x;
        slotPositionActual.y = slotPosition.y;

        // round down to nearest multiple of slot size
        slotPositionActual.x = slotPositionActual.x - (slotPositionActual.x % tibia::GuiData::InventoryWindow::Slots::size);
        slotPositionActual.y = slotPositionActual.y - (slotPositionActual.y % tibia::GuiData::InventoryWindow::Slots::size);

        //std::cout << "slotPositionActual x: " << slotPositionActual.x << std::endl;
        //std::cout << "slotPositionActual y: " << slotPositionActual.y << std::endl;

        slotPositionActual.x = slotPositionActual.x / tibia::GuiData::InventoryWindow::Slots::size;
        slotPositionActual.y = slotPositionActual.y / tibia::GuiData::InventoryWindow::Slots::size;

        //std::cout << "slotPositionActual x: " << slotPositionActual.x << std::endl;
        //std::cout << "slotPositionActual y: " << slotPositionActual.y << std::endl;

        int slotIndex = slotPositionActual.x + (slotPositionActual.y * tibia::GuiData::InventoryWindow::Slots::numSlotsHorizontal);

        //std::cout << "slotIndex: " << slotIndex << std::endl;

        return slotIndex;
    }

    void updateMouseTile()
    {
        m_mouseTile = std::move(getTileUnderMouseCursor());
    }

    bool isMouseInsideGameWindow()
    {
        return tibia::GuiData::GameWindow::rect.contains(getMouseWindowPosition());
    }

    bool isMouseInsideMiniMapWindow()
    {
        return tibia::GuiData::MiniMapWindow::rect.contains(getMouseWindowPosition());
    }

    sf::Vector2u getMouseTilePosition()
    {
        sf::Vector2u mouseTilePosition = static_cast<sf::Vector2u>(m_gameWindow.mapPixelToCoords(getMouseWindowPosition(), m_gameWindowView));

        // game window is offset
        mouseTilePosition.x -= tibia::GuiData::GameWindow::x;
        mouseTilePosition.y -= tibia::GuiData::GameWindow::y;

        // convert mouse tile position to actual tile position
        // round down to nearest multiple of tile size
        mouseTilePosition.x = mouseTilePosition.x - (mouseTilePosition.x % tibia::TILE_SIZE);
        mouseTilePosition.y = mouseTilePosition.y - (mouseTilePosition.y % tibia::TILE_SIZE);

        return mouseTilePosition;
    }

    tibia::Tile::Ptr getTileUnderMouseCursor()
    {
        if (isMouseInsideGameWindow() == false)
        {
            return nullptr;
        }

        sf::Vector2u mouseTilePosition = getMouseTilePosition();

        if (m_map.isTilePositionOutOfBounds(mouseTilePosition) == true)
        {
            return nullptr;
        }

        return getTile(mouseTilePosition, m_player->getZ());
    }

    tibia::Tile::Ptr getThingTile(tibia::Thing::Ptr thing)
    {
        if (thing == nullptr)
        {
            return nullptr;
        }

        tibia::Tile::List* tileList = m_map.tileMapTiles[thing->getZ()].getTileList();

        if (tileList->size() == 0)
        {
            return nullptr;
        }

        int tileNumber = m_map.getTileNumberByTileCoords(thing->getTilePosition());

        return tileList->at(tileNumber);
    }

    tibia::Tile::Ptr getThingTileByMovement(tibia::Thing::Ptr thing, int direction)
    {
        sf::Vector2i vecMovement = tibia::Utility::getVectorByDirection(direction);

        vecMovement.x = vecMovement.x * tibia::TILE_SIZE;
        vecMovement.y = vecMovement.y * tibia::TILE_SIZE;

        int tileNumber = m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() + vecMovement.x, thing->getTileY() + vecMovement.y));
 
        return m_map.tileMapTiles[thing->getZ()].getTileList()->at(tileNumber);
    }

    tibia::Object::Ptr spawnObject(sf::Vector2u tileCoords, int z, int id)
    {
        tibia::Object::Ptr object = std::make_shared<tibia::Object>(tileCoords, z, id);

        tibia::Tile::Ptr tile = getTile(tileCoords, z);

        if (tile == nullptr)
        {
            return nullptr;
        }

        tile->addObject(object);

        return object;
    }

    tibia::Object::Ptr spawnDecayObject(sf::Vector2u tileCoords, int z, std::vector<int> id, bool isDecal = false)
    {
        tibia::Object::Ptr object = std::make_shared<tibia::Object>(tileCoords, z, id[0]);

        object->setIsDecay(true);

        tibia::Tile::Ptr tile = getTile(tileCoords, z);

        if (tile == nullptr)
        {
            return nullptr;
        }

        tibia::Object::List* objectList = tile->getObjectList();

        if (objectList->size())
        {
            if (isDecal == true)
            {
                for (auto& object : *objectList)
                {
                    if (object->getFlags() & tibia::SpriteFlags::decal)
                    {
                        object->setIsReadyForErase(true);
                    }
                }
            }

            tibia::Object::Ptr topObject = objectList->back();

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

        return object;
    }

    void spawnAnimation(sf::Vector2u tileCoords, int z, std::vector<int> animationId, float frameTime = tibia::AnimationTimes::default)
    {
        tibia::Animation::Ptr animation = std::make_shared<tibia::Animation>(tileCoords.x, tileCoords.y, z, animationId[0], animationId[1]);

        animation->setFrameTime(frameTime);

        tibia::Tile::Ptr tile = getTile(tileCoords, z);

        tile->addAnimation(animation);
    }

    sf::IntRect getGameWindowViewDrawRect()
    {
        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        x1 = x1 - tibia::NUM_TILES_FROM_CENTER_X - 1;
        y1 = y1 - tibia::NUM_TILES_FROM_CENTER_Y - 1;

        int x2 = tibia::NUM_TILES_X + 2;
        int y2 = tibia::NUM_TILES_Y + 2;

        return sf::IntRect(x1, y1, x2, y2);
    }

    void drawTileMap(tibia::TileMap* tileMap)
    {
        tibia::Tile::List* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return;
        }

        m_tileMapTileVertices.clear();

        int x1 = getGameWindowViewDrawRect().left;
        int y1 = getGameWindowViewDrawRect().top;

        int x2 = getGameWindowViewDrawRect().width;
        int y2 = getGameWindowViewDrawRect().height;

        for (int i = x1; i < x1 + x2; i++)
        {
            for (int j = y1; j < y1 + y2; j++)
            {
                if (i < 0) continue;
                if (j < 0) continue;

                if (i > m_map.getWidth()  - 1) continue;
                if (j > m_map.getHeight() - 1) continue;

                int tileNumber = i + j * m_map.getWidth();

                if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

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

                unsigned long tileFlags = tile->getFlags();

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
                    tibia::Object::List* tileObjects = tile->getObjectList();

                    if (tileObjects->size())
                    {
                        tibia::Object::List* tileMapObjects = &m_tileMapObjects[tileMap->getZ()];

                        tileMapObjects->reserve(tileObjects->size());

                        std::copy(tileObjects->begin(), tileObjects->end(), std::back_inserter(*tileMapObjects));
                    }

                    //

                    tibia::Creature::List* tileCreatures = tile->getCreatureList();

                    if (tileCreatures->size())
                    {
                        tibia::Creature::List* tileMapCreatures = &m_tileMapCreatures[tileMap->getZ()];

                        tileMapCreatures->reserve(tileCreatures->size());

                        std::copy(tileCreatures->begin(), tileCreatures->end(), std::back_inserter(*tileMapCreatures));
                    }

                    //

                    tibia::Animation::List* tileAnimations = tile->getAnimationList();

                    if (tileAnimations->size())
                    {
                        tibia::Animation::List* tileMapAnimations = &m_tileMapAnimations[tileMap->getZ()];

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
        tibia::Tile::List* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return;
        }

        int x1 = getGameWindowViewDrawRect().left;
        int y1 = getGameWindowViewDrawRect().top;

        int x2 = getGameWindowViewDrawRect().width;
        int y2 = getGameWindowViewDrawRect().height;

        for (int i = x1; i < x1 + x2; i++)
        {
            for (int j = y1; j < y1 + y2; j++)
            {
                if (i < 0) continue;
                if (j < 0) continue;

                if (i > m_map.getWidth()  - 1) continue;
                if (j > m_map.getHeight() - 1) continue;

                int tileNumber = i + j * m_map.getWidth();

                if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

                if (tileId == tibia::TILE_NULL)
                {
                    continue;
                }

                updateStepTile(tile);
                updateTileHeight(tile);
                updateTileGroupedObjects(tile);

                tibia::Creature::List* tileCreatures = tile->getCreatureList();

                if (tileCreatures->size())
                {
                    for (auto& creatureIt = tileCreatures->begin(); creatureIt != tileCreatures->end(); creatureIt++)
                    {
                        tibia::Creature::Ptr creature = *creatureIt;

                        if (creature->isDead() == true)
                        {
                            sf::Clock* clockCorpse = creature->getClockCorpse();

                            sf::Time timeElapsedCorpse = clockCorpse->getElapsedTime();

                            if (timeElapsedCorpse.asSeconds() >= tibia::CreatureData::corpseDecayTime)
                            {
                                creature->updateCorpse();
                            }

                            sf::Clock* clockDead = creature->getClockDead();

                            sf::Time timeElapsedDead = clockDead->getElapsedTime();

                            if (timeElapsedDead.asSeconds() >= tibia::CreatureData::respawnTime)
                            {
                                if (creature->isPlayer() == true)
                                {
                                    respawnPlayer(creature);
                                }
                            }
                        }

                        tibia::Creature::StatusEffectList* statusEffectList = creature->getStatusEffectList();

                        if (creature->isDead() == true || creature->isSleeping() == true)
                        {
                            statusEffectList->clear();
                        }

                        if (statusEffectList->size() != 0)
                        {
                            for (auto& statusEffectIt = statusEffectList->begin(); statusEffectIt != statusEffectList->end(); statusEffectIt++)
                            {
                                if (statusEffectIt->clock.getElapsedTime().asSeconds() > statusEffectIt->timePerTick.asSeconds())
                                {
                                    if (statusEffectIt->ticks == 0)
                                    {
                                        std::string statusEffectName = statusEffectIt->name;

                                        std::transform(statusEffectName.begin(), statusEffectName.end(), statusEffectName.begin(), std::tolower);

                                        std::stringstream ssStatusEffectMessage;
                                        ssStatusEffectMessage << "You are no longer " << statusEffectName << ".";

                                        showStatusBarText(ssStatusEffectMessage.str());

                                        statusEffectIt = statusEffectList->erase(statusEffectIt);
                                        statusEffectIt--;
                                        continue;
                                    }

                                    //std::cout << "statusEffect name:  " << statusEffectIt->name  << std::endl;
                                    //std::cout << "statusEffect ticks: " << statusEffectIt->ticks << std::endl;

                                    handleCreatureModifyHp
                                    (
                                        nullptr,
                                        creature,
                                        -(statusEffectIt->damage),
                                        tibia::UMaps::creatureStatusEffectsModifyHpTypes[statusEffectIt->type]
                                    );

                                    statusEffectIt->ticks--;

                                    statusEffectIt->clock.restart();
                                }
                            }
                        }

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

                tibia::Object::List* tileObjects = tile->getObjectList();

                if (tileObjects->size())
                {
                    for (auto& objectIt = tileObjects->begin(); objectIt != tileObjects->end(); objectIt++)
                    {
                        tibia::Object::Ptr object = *objectIt;

                        if (object->isDecay() == true)
                        {
                            sf::Time timeElapsedDecay = object->getClockDecay()->getElapsedTime();

                            if (timeElapsedDecay.asSeconds() >= tibia::DecayObjects::time)
                            {
                                object->doDecay();
                            }
                        }

                        sf::Time timeElapsedRegenerate = object->getClockRegenerate()->getElapsedTime();

                        if (timeElapsedRegenerate.asSeconds() >= tibia::RegenerateObjects::time)
                        {
                            tibia::Tile::Ptr tile = getTile(object->getTilePosition(), object->getZ());

                            tibia::Object::List* objectList = tile->getObjectList();

                            tibia::Creature::List* creatureList = tile->getCreatureList();

                            // does not regenerate if there are creatures or objects on top of it
                            if (creatureList->size() == 0 && objectList->size() != 0 && (objectList->back() == object))
                            {
                                object->doRegenerate();
                            }
                            else
                            {
                                object->getClockRegenerate()->restart();
                            }
                        }

                        object->update();

                        if (object->isReadyForErase() == true)
                        {
                            objectIt = tileObjects->erase(objectIt);
                            objectIt--;
                            continue;
                        }

                        // fix drawing order of special objects
                        if (object->getFlags() & tibia::SpriteFlags::fixDrawOrder)
                        {
                            object->setTileX(object->getTileX() + tibia::TILE_SIZE);
                            object->setTileY(object->getTileY() + tibia::TILE_SIZE);
                        }
                    }
                }

                //

                tibia::Animation::List* tileAnimations = tile->getAnimationList();

                if (tileAnimations->size())
                {
                    for (auto& animationIt = tileAnimations->begin(); animationIt != tileAnimations->end(); animationIt++)
                    {
                        tibia::Animation::Ptr animation = *animationIt;

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

    void drawThingList(tibia::Thing::List* thingList)
    {
        std::stable_sort(thingList->begin(), thingList->end(), tibia::ThingSort::sortByTileCoords());

        for (auto& thing : *thingList)
        {
            m_gameWindowLayer.draw(*thing);
        }
    }

    void drawLightAtTilePosition(tibia::Light& light, sf::Vector2u tilePosition)
    {
        light.setPosition
        (
            tilePosition.x + (tibia::TILE_SIZE / 2),
            tilePosition.y + (tibia::TILE_SIZE / 2)
        );

        m_lightLayer.draw(light, sf::BlendAdd);
    }

    void drawTileMapLights(int z)
    {
        m_lightLayer.setView(m_gameWindowView);

        if (z < tibia::ZAxis::ground)
        {
            m_lightLayer.clear(sf::Color(tibia::LightBrightness::underGround, tibia::LightBrightness::underGround, tibia::LightBrightness::underGround));
        }
        else
        {
            m_lightLayer.clear(sf::Color(m_lightBrightness, m_lightBrightness, m_lightBrightness));
        }

        m_light.setSize(tibia::LightSizes::small);

        for (auto& creature : m_tileMapCreatures[z])
        {
            if (creature->isDead() == true || creature->isSleeping() == true)
            {
                continue;
            }

            drawLightAtTilePosition(m_light, creature->getTilePosition());
        }

        m_light.setSize(tibia::LightSizes::medium);

        for (auto& object : m_tileMapObjects[z])
        {
            if (object->getFlags() & tibia::SpriteFlags::lightSource)
            {
                //light.setColorbyId(object->getId());

                drawLightAtTilePosition(m_light, object->getTilePosition());
            }

            if (z == tibia::ZAxis::ground - 1)
            {
                if (object->getId() == tibia::SpriteData::stairs || object->getId() == tibia::SpriteData::ladder || object->getId() == tibia::SpriteData::ropeUp)
                {
                    //light.setColor(tibia::Colors::light);

                    drawLightAtTilePosition(m_light, object->getTilePosition());
                }
            }
        }

        m_light.setSize(tibia::LightSizes::tiny);

        for (auto& animation : m_tileMapAnimations[z])
        {
            if (animation->getFlags() & tibia::SpriteFlags::lightSource)
            {
                drawLightAtTilePosition(m_light, animation->getTilePosition());
            }
        }

        for (auto& projectile : m_tileMapProjectiles[z])
        {
            if (projectile->getFlags() & tibia::SpriteFlags::lightSource)
            {
                drawLightAtTilePosition(m_light, projectile->getTilePosition());
            }
        }

        m_lightLayer.display();

        m_lightLayerSprite.setTexture(m_lightLayer.getTexture());

        m_lightLayerSprite.setPosition(getGameWindowViewDrawRect().left * tibia::TILE_SIZE, getGameWindowViewDrawRect().top * tibia::TILE_SIZE);

        m_gameWindowLayer.draw(m_lightLayerSprite, sf::BlendMultiply);
    }

    void drawGameLayer(int z)
    {
        m_gameWindowLayer.setView(m_gameWindowView);
        m_gameWindowLayer.clear(tibia::Colors::transparent);

        drawTileMap(&m_map.tileMapTiles[z]);
        drawTileMap(&m_map.tileMapTileEdges[z]);

        // this only updates things being drawn
        updateTileThings(&m_map.tileMapTiles[z]);

        tibia::Projectile::List* projectileList = &m_projectileList[z];

        if (projectileList->size())
        {
            // TODO: do not draw projectiles that are offscreen

            tibia::Projectile::List* tileMapProjectiles = &m_tileMapProjectiles[z];

            tileMapProjectiles->reserve(projectileList->size());

            std::copy(projectileList->begin(), projectileList->end(), std::back_inserter(*tileMapProjectiles));
        }

        updateProjectiles(z);

        tibia::Thing::List* tileMapThings = &m_tileMapThings[z];

        std::size_t reserveThingsSize = m_tileMapObjects[z].size()    +
                                        m_tileMapCreatures[z].size()  +
                                        m_tileMapAnimations[z].size() +
                                        m_tileMapProjectiles[z].size();

        tileMapThings->reserve(reserveThingsSize);

        std::copy(m_tileMapObjects[z].begin(),     m_tileMapObjects[z].end(),     std::back_inserter(*tileMapThings));
        std::copy(m_tileMapCreatures[z].begin(),   m_tileMapCreatures[z].end(),   std::back_inserter(*tileMapThings));
        std::copy(m_tileMapAnimations[z].begin(),  m_tileMapAnimations[z].end(),  std::back_inserter(*tileMapThings));
        std::copy(m_tileMapProjectiles[z].begin(), m_tileMapProjectiles[z].end(), std::back_inserter(*tileMapThings));

        drawThingList(&m_tileMapThings[z]);

        if (z < tibia::ZAxis::ground || m_lightBrightness != tibia::LightBrightness::max)
        {
            drawTileMapLights(z);
        }

        doCreatureLogic(z);

        m_gameWindowLayer.display();

        m_gameWindowLayerSprite.setTexture(m_gameWindowLayer.getTexture());

        m_gameWindowLayerSprite.setPosition((getGameWindowViewDrawRect().left + 1) * tibia::TILE_SIZE, (getGameWindowViewDrawRect().top + 1) * tibia::TILE_SIZE);

        m_gameWindow.draw(m_gameWindowLayerSprite);
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

        unsigned int zBegin = tibia::ZAxis::min;
        unsigned int zEnd   = tibia::ZAxis::max;

        if (m_player->getZ() < tibia::ZAxis::ground)
        {
            zBegin = tibia::ZAxis::min;
            zEnd   = tibia::ZAxis::underGround;
        }
        else
        {
            zBegin = tibia::ZAxis::ground;
            zEnd   = tibia::ZAxis::max;
        }

        for (unsigned int i = zBegin; i < zEnd + 1; i++)
        {
            m_tileMapObjects[i].clear();
            m_tileMapCreatures[i].clear();
            m_tileMapAnimations[i].clear();
            m_tileMapProjectiles[i].clear();
            m_tileMapThings[i].clear();

            //updateTileThings(&m_map.tileMapTiles[i]); // lags too much here, updated elsewhere
        }

        //std::cout << "drawGameLayer START ------------------" << std::endl;

        for (unsigned int i = zBegin; i < zEnd + 1; i++)
        {
            if (isTileMapVisible(&m_map.tileMapTiles[i]) == true)
            {
                drawGameLayer(i);

                //std::cout << "drawGameLayer: " << i << std::endl;
            }

            if (i == tibia::ZAxis::max)
            {
                break;
            }

            if (findTilesAboveThing(m_player, i + 1) == true)
            {
                break;
            }
        }

        //std::cout << "drawGameLayer STOP -------------------" << std::endl;

        drawFloatingTextList();
        drawGameTextList();

        drawGameWindowText();

        m_gameWindow.display();

        m_gameWindowSprite.setTexture(m_gameWindow.getTexture());
        m_gameWindowSprite.setPosition(tibia::GuiData::GameWindow::position);

        mainWindow->draw(m_gameWindowSprite);
    }

    void addMiniMapWindowTiles(tibia::TileMap& tileMap, std::vector<sf::Vertex>& vertexList, int x1, int y1, int x2, int y2)
    {
        tibia::Tile::List* tileList = tileMap.getTileList();

        if (tileList->size() == 0)
        {
            return;
        }

        if (tileMap.getType() != tibia::TileMapTypes::tiles)
        {
            return;
        }

        for (int i = x1; i < x1 + x2; i++)
        {
            for (int j = y1; j < y1 + y2; j++)
            {
                if (i < 0) continue;
                if (j < 0) continue;

                if (i > m_map.getWidth()  - 1) continue;
                if (j > m_map.getHeight() - 1) continue;

                int tileNumber = i + j * m_map.getWidth();

                if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

                if (tileId == tibia::TILE_NULL)
                {
                    continue;
                }

                unsigned long tileFlags = tile->getFlags();

                sf::Vertex quad[4];

                quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                sf::Color tileColor = tibia::Colors::MiniMap::default;

                if (tileFlags & tibia::SpriteFlags::solid)
                {
                    tileColor = tibia::Colors::MiniMap::solid;
                }

                if (tileFlags & tibia::SpriteFlags::water)
                {
                    tileColor = tibia::Colors::MiniMap::water;
                }

                if (tileFlags & tibia::SpriteFlags::lava)
                {
                    tileColor = tibia::Colors::MiniMap::lava;
                }

                if (tileFlags & (tibia::SpriteFlags::moveAbove | tibia::SpriteFlags::moveBelow))
                {
                    tileColor = tibia::Colors::MiniMap::moveAboveOrBelow;
                }

                auto miniMapColorIt = tibia::UMaps::miniMapColors.find(tileId);

                if (miniMapColorIt != tibia::UMaps::miniMapColors.end())
                {
                    tileColor = miniMapColorIt->second;
                }

                quad[0].color = tileColor;
                quad[1].color = tileColor;
                quad[2].color = tileColor;
                quad[3].color = tileColor;

                vertexList.push_back(quad[0]);
                vertexList.push_back(quad[1]);
                vertexList.push_back(quad[2]);
                vertexList.push_back(quad[3]);

                tibia::Object::List* tileObjects = tile->getObjectList();

                if (tileObjects->size())
                {
                    for (auto& object : *tileObjects)
                    {
                        unsigned long objectFlags = object->getFlags();

                        sf::Vertex quad[4];

                        quad[0].position = sf::Vector2f(i       * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                        quad[1].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, j       * tibia::TILE_SIZE);
                        quad[2].position = sf::Vector2f((i + 1) * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);
                        quad[3].position = sf::Vector2f(i       * tibia::TILE_SIZE, (j + 1) * tibia::TILE_SIZE);

                        sf::Color objectColor = tibia::Colors::transparent;

                        if (objectFlags & tibia::SpriteFlags::solid)
                        {
                            objectColor = tibia::Colors::MiniMap::solid;
                        }

                        if
                        (
                            (objectFlags & (tibia::SpriteFlags::moveAbove | tibia::SpriteFlags::moveBelow)) ||
                            object->getType() == tibia::ObjectTypes::teleporter                             ||
                            object->getId()   == tibia::SpriteData::ladder                                  ||
                            object->getId()   == tibia::SpriteData::ropeUp
                        )
                        {
                            objectColor = tibia::Colors::MiniMap::moveAboveOrBelow;
                        }

                        auto miniMapColorIt = tibia::UMaps::miniMapColors.find(object->getId());

                        if (miniMapColorIt != tibia::UMaps::miniMapColors.end())
                        {
                            objectColor = miniMapColorIt->second;
                        }

                        quad[0].color = objectColor;
                        quad[1].color = objectColor;
                        quad[2].color = objectColor;
                        quad[3].color = objectColor;

                        vertexList.push_back(quad[0]);
                        vertexList.push_back(quad[1]);
                        vertexList.push_back(quad[2]);
                        vertexList.push_back(quad[3]);
                    }
                }
            }
        }
    }

    void updateMiniMapWindow()
    {
        m_miniMapWindow.clear(tibia::Colors::black);

        m_miniMapVertices.clear();

        int x1 = ((m_gameWindowView.getCenter().x - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);
        int y1 = ((m_gameWindowView.getCenter().y - (tibia::TILE_SIZE / 2)) / tibia::TILE_SIZE);

        //x1 = x1 - (NUM_TILES_FROM_CENTER_X * m_miniMapWindowZoom) - 1;
        //y1 = y1 - (NUM_TILES_FROM_CENTER_Y * m_miniMapWindowZoom) - 1;
        x1 = x1 - (8 * m_miniMapWindowZoom);
        y1 = y1 - (8 * m_miniMapWindowZoom);

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;

        //int x2 = (tibia::NUM_TILES_X * m_miniMapWindowZoom) + 1;
        //int y2 = (tibia::NUM_TILES_Y * m_miniMapWindowZoom) + 1;
        int x2 = (16 * m_miniMapWindowZoom);
        int y2 = (16 * m_miniMapWindowZoom);

        addMiniMapWindowTiles(m_map.tileMapTiles[m_player->getZ()], m_miniMapVertices, x1, y1, x2, y2);

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
        m_miniMapWindowSprite.setPosition(tibia::GuiData::MiniMapWindow::position);

        mainWindow->draw(m_miniMapWindowSprite);
    }

    void updateLightBrightness()
    {
        sf::Time timeElapsed = clockLightBrightness.getElapsedTime();

        if (timeElapsed.asMilliseconds() > tibia::LightBrightness::updateTime * m_timeDelta.asMilliseconds())
        {
            clockLightBrightness.restart();
        }
        else
        {
            return;
        }

        if (m_timeOfDay == tibia::TimeOfDay::day)
        {
            if (m_lightBrightness < tibia::LightBrightness::day)
            {
                m_lightBrightness++;
            }
        }
        else if (m_timeOfDay == tibia::TimeOfDay::night)
        {
            if (m_lightBrightness > tibia::LightBrightness::night)
            {
                m_lightBrightness--;
            }
        }
    }

    void spawnGameText(const sf::Vector2u& tilePosition, int z, const std::string& text, sf::Color textColor)
    {
        if (z != m_player->getZ())
        {
            return;
        }

        tibia::GameText gt;
        gt.setText(m_bitmapFontDefault, tilePosition, z, text, textColor);

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

            float displayTime = tibia::GameTextData::time;

            if (gameTextIt->getNumTextLines() > 3)
            {
                displayTime *= 2;
            }

            float textTileDistance = utility::calculateDistance
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

            sf::Clock* clock = gameTextIt->getClock();

            sf::Time timeElapsed = clock->getElapsedTime();

            if (timeElapsed.asSeconds() > displayTime || textIsOffScreen == true)
            {
                gameTextIt = m_gameTextList.erase(gameTextIt);
                gameTextIt--;
                continue;
            }

            m_gameWindow.draw(*gameTextIt);
        }
    }

    void showGameWindowText(const std::string& text, sf::Color textColor = sf::Color::White)
    {
        std::vector<std::string> textLines;
        boost::split(textLines, text, boost::is_any_of("\n"));

        m_gameWindowText.setText
        (
            m_bitmapFontDefault,
            sf::Vector2u
            (
                m_player->getTileX() + (tibia::TILE_SIZE / 2),
                m_player->getTileY() + (tibia::TILE_SIZE / 2) - ((m_bitmapFontDefault.getGlyphSize()->y * textLines.size()) / 2)
            ),
            m_player->getZ(),
            text,
            textColor
        );

        m_gameWindowText.getClock()->restart();
    }

    void drawGameWindowText()
    {
        float displayTime = tibia::GameWindowTextData::time;

        int numTextLines = m_gameWindowText.getNumTextLines();

        if (numTextLines > 2)
        {
            displayTime += (float)numTextLines;
        }

        sf::Time timeElapsed = m_gameWindowText.getClock()->getElapsedTime();

        if (timeElapsed.asSeconds() < displayTime)
        {
            std::string text = m_gameWindowText.getText();

            sf::Color* textColor = m_gameWindowText.getTextColor();

            std::vector<std::string> textLines;
            boost::split(textLines, text, boost::is_any_of("\n"));

            m_gameWindowText.setText
            (
                m_bitmapFontDefault,
                sf::Vector2u
                (
                    m_player->getTileX() + (tibia::TILE_SIZE / 2),
                    m_player->getTileY() + (tibia::TILE_SIZE / 2) - ((m_bitmapFontDefault.getGlyphSize()->y * textLines.size()) / 2)
                ),
                m_player->getZ(),
                text,
                *textColor
            );

            m_gameWindow.draw(m_gameWindowText);
        }
    }

    void spawnFloatingText(const sf::Vector2u& tilePosition, int z, const std::string& text, sf::Color textColor)
    {
        if (this->options.isGameShowFloatingTextEnabled == false)
        {
            return;
        }

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

            sf::Time timeElapsed = clock->getElapsedTime();

            if (timeElapsed.asSeconds() > tibia::FloatingTextData::time)
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
        m_statusBarText.setText(&m_bitmapFontModern, text, tibia::Colors::Text::white, true);

        m_clockStatusBar.restart();
    }

    void drawStatusBarText(sf::RenderWindow* mainWindow)
    {
        sf::Time timeElapsed = m_clockStatusBar.getElapsedTime();

        if (timeElapsed.asSeconds() < tibia::StatusBarTextData::time)
        {
            mainWindow->draw(m_statusBarText);
        }
    }

    void drawStatusEffectIcons(sf::RenderWindow* mainWindow)
    {
        tibia::Creature::StatusEffectList* statusEffectList = m_player->getStatusEffectList();

        if (statusEffectList->size() == 0)
        {
            return;
        }

        int statusEffectIndex = 0;

        for (auto& statusEffect : *statusEffectList)
        {
            if (statusEffect.ticks == 0)
            {
                continue;
            }

            int statusEffectIconId = tibia::UMaps::creatureStatusEffectsIcons[statusEffect.type];

            tibia::Sprite statusEffectIcon;
            statusEffectIcon.setId(statusEffectIconId);

            statusEffectIcon.setPosition
            (
                sf::Vector2f
                (
                    tibia::GuiData::StatusEffectIcons::x,
                    tibia::GuiData::StatusEffectIcons::y + (tibia::GuiData::StatusEffectIcons::distanceBetweenIcons * statusEffectIndex)
                )
            );

            tibia::BitmapFontText statusEffectTicksText;
            statusEffectTicksText.setText(&m_bitmapFontTiny, std::to_string(statusEffect.ticks), sf::Color::White);

            statusEffectTicksText.setPosition
            (
                sf::Vector2f
                (
                    tibia::GuiData::StatusEffectIcons::x + 2,
                    tibia::GuiData::StatusEffectIcons::y + 2 + (tibia::GuiData::StatusEffectIcons::distanceBetweenIcons * statusEffectIndex)
                )
            );

            mainWindow->draw(statusEffectIcon);
            mainWindow->draw(statusEffectTicksText);

            statusEffectIndex++;
        }
    }

    void drawBars(sf::RenderWindow* mainWindow)
    {
        tibia::Bar bar;

        // Health

        int hp    = m_player->getHp();
        int hpMax = m_player->getHpMax();

        int hpBarType = tibia::BarsTypes::red;

        if (m_player->hasStatusEffect(tibia::CreatureStatusEffects::drunk) == true)
        {
            hpBarType = tibia::BarsTypes::purple;
        }

        if (m_player->hasStatusEffect(tibia::CreatureStatusEffects::poisoned) == true)
        {
            hpBarType = tibia::BarsTypes::green;
        }

        bar.setValues(hpBarType, hp, hpMax);

        bar.setPosition(tibia::GuiData::Bars::Hp::position);

        std::stringstream hpTextStream;
        hpTextStream << hp << " / " << hpMax;

        tibia::BitmapFontText hpText;
        hpText.setText(&m_bitmapFontModern, hpTextStream.str(), sf::Color::White);

        hpText.setPosition
        (
            sf::Vector2f
            (
                bar.getPosition().x + (tibia::BarsData::width / 2) - (hpText.getVertexArray()->getBounds().width / 2),
                bar.getPosition().y + 2
            )
        );

        mainWindow->draw(bar);
        mainWindow->draw(hpText);

        // Mana

        int mp    = m_player->getMp();
        int mpMax = m_player->getMpMax();

        bar.setValues(tibia::BarsTypes::blue, mp, mpMax);

        bar.setPosition(tibia::GuiData::Bars::Mp::position);

        std::stringstream mpTextStream;
        mpTextStream << mp << " / " << mpMax;

        tibia::BitmapFontText mpText;
        mpText.setText(&m_bitmapFontModern, mpTextStream.str(), sf::Color::White);

        mpText.setPosition
        (
            sf::Vector2f
            (
                bar.getPosition().x + (tibia::BarsData::width / 2) - (mpText.getVertexArray()->getBounds().width / 2),
                bar.getPosition().y + 2
            )
        );

        mainWindow->draw(bar);
        mainWindow->draw(mpText);
    }

/*
    void drawOptionsButton(sf::RenderWindow* mainWindow)
    {
        tibia::Sprite optionsButton;
        optionsButton.setId(tibia::SpriteData::Gui::optionsButton);
        optionsButton.setPosition(tibia::GuiData::OptionsButton::position);

        mainWindow->draw(optionsButton);
    }
*/

    void drawChatLogWindow(sf::RenderWindow* mainWindow)
    {
        if (m_chatLogWindowTextList.size() == 0)
        {
            return;
        }
        else if (m_chatLogWindowTextList.size() > tibia::GuiData::ChatLogWindow::maxLines)
        {
            m_chatLogWindowTextList.erase(m_chatLogWindowTextList.begin(), m_chatLogWindowTextList.end() - tibia::GuiData::ChatLogWindow::maxLines);
        }

        if (tibia::Utility::getViewPosition(m_chatLogWindowView).y < 0)
        {
            tibia::Sprite buttonScrollDown;
            buttonScrollDown.setId(tibia::SpriteData::Gui::scrollButtonDown[1]);
            buttonScrollDown.setPosition(tibia::GuiData::ChatLogWindow::Buttons::ScrollDown::position);

            mainWindow->draw(buttonScrollDown);

            this->gui.chatLogWindowButtonScrollDown = true;
        }
        else
        {
            this->gui.chatLogWindowButtonScrollDown = false;
        }

        if (m_chatLogWindowTextList.size() > tibia::GuiData::ChatLogWindow::numLinesVisible)
        {
            if (tibia::Utility::getViewPosition(m_chatLogWindowView).y > getChatLogWindowViewMaxScrollY())
            {
                tibia::Sprite buttonScrollUp;
                buttonScrollUp.setId(tibia::SpriteData::Gui::scrollButtonUp[1]);
                buttonScrollUp.setPosition(tibia::GuiData::ChatLogWindow::Buttons::ScrollUp::position);

                mainWindow->draw(buttonScrollUp);

                this->gui.chatLogWindowButtonScrollUp = true;
            }
            else
            {
                this->gui.chatLogWindowButtonScrollUp = false;
            }
        }
        else
        {
            this->gui.chatLogWindowButtonScrollUp = false;
        }

        m_chatLogWindow.setView(m_chatLogWindowView);
        m_chatLogWindow.clear(tibia::Colors::transparent);

        sf::Vector2f chatLogWindowTextPosition
        (
            tibia::GuiData::ChatLogWindow::textOffset,
            tibia::GuiData::ChatLogWindow::height - tibia::Fonts::System::characterSize - 1
        );

        for (auto chatLogWindowTextIt = m_chatLogWindowTextList.rbegin(); chatLogWindowTextIt != m_chatLogWindowTextList.rend(); ++chatLogWindowTextIt)
        {
            //std::cout << chatLogWindowTextIt->getString().toAnsiString() << std::endl;

            chatLogWindowTextIt->setPosition(chatLogWindowTextPosition);

            m_chatLogWindow.draw(*chatLogWindowTextIt);

            chatLogWindowTextPosition.y -= tibia::GuiData::ChatLogWindow::textHeight + tibia::GuiData::ChatLogWindow::textOffset;
        }

        m_chatLogWindow.display();

        m_chatLogWindowSprite.setTexture(m_chatLogWindow.getTexture());
        m_chatLogWindowSprite.setTextureRect(sf::IntRect(0, 0, tibia::GuiData::ChatLogWindow::width, tibia::GuiData::ChatLogWindow::height));
        m_chatLogWindowSprite.setPosition(tibia::GuiData::ChatLogWindow::position);

        mainWindow->draw(m_chatLogWindowSprite);
    }

    void drawTabButtons(sf::RenderWindow* mainWindow)
    {
        int tabButtonId = tibia::SpriteData::Gui::tabButtonMiniMap[1];

        sf::Vector2f tabButtonPosition = tibia::GuiData::TabButtons::MiniMap::position;

        switch (this->gui.tabButtonsState)
        {
            case tibia::GuiData::TabButtons::State::inventory:
                tabButtonId       = tibia::SpriteData::Gui::tabButtonInventory[1];
                tabButtonPosition = tibia::GuiData::TabButtons::Inventory::position;
                break;

            case tibia::GuiData::TabButtons::State::status:
                tabButtonId       = tibia::SpriteData::Gui::tabButtonStatus[1];
                tabButtonPosition = tibia::GuiData::TabButtons::Status::position;
                break;

            case tibia::GuiData::TabButtons::State::combat:
                tabButtonId       = tibia::SpriteData::Gui::tabButtonCombat[1];
                tabButtonPosition = tibia::GuiData::TabButtons::Combat::position;
                break;

            case tibia::GuiData::TabButtons::State::miniMap:
                tabButtonId       = tibia::SpriteData::Gui::tabButtonMiniMap[1];
                tabButtonPosition = tibia::GuiData::TabButtons::MiniMap::position;
                break;
        }

        tibia::Sprite tabButton;
        tabButton.setId(tabButtonId);
        tabButton.setPosition(tabButtonPosition);

        mainWindow->draw(tabButton);
    }

    void drawStatusWindow(sf::RenderWindow* mainWindow)
    {
        sf::Sprite statusWindow;
        statusWindow.setTexture(tibia::Textures::status);
        statusWindow.setPosition(tibia::GuiData::StatusWindow::position);

        mainWindow->draw(statusWindow);

        tibia::BitmapFontText statusText;

        // HP
        statusText.setText(&m_bitmapFontTiny, std::to_string(m_player->getHp()), tibia::Colors::white);
        statusText.setPosition
        (
            tibia::GuiData::StatusWindow::Stats::Hp::x + tibia::GuiData::StatusWindow::Icons::width + 1 - statusText.getVertexArray()->getBounds().width,
            tibia::GuiData::StatusWindow::Stats::Hp::y + tibia::GuiData::StatusWindow::Icons::width + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(statusText);

        // CAP
        int cap = m_player->getCap() - m_player->getInventoryItemList()->size();

        statusText.setText(&m_bitmapFontTiny, std::to_string(cap), tibia::Colors::white);
        statusText.setPosition
        (
            tibia::GuiData::StatusWindow::Stats::Cap::x + tibia::GuiData::StatusWindow::Icons::width + 1 - statusText.getVertexArray()->getBounds().width,
            tibia::GuiData::StatusWindow::Stats::Cap::y + tibia::GuiData::StatusWindow::Icons::width + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(statusText);

        // EXP
        statusText.setText(&m_bitmapFontTiny, std::to_string(m_player->getExp()), tibia::Colors::white);
        statusText.setPosition
        (
            tibia::GuiData::StatusWindow::Stats::Exp::x + tibia::GuiData::StatusWindow::Icons::width + 1 - statusText.getVertexArray()->getBounds().width,
            tibia::GuiData::StatusWindow::Stats::Exp::y + tibia::GuiData::StatusWindow::Icons::width + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(statusText);

        // MANA
        statusText.setText(&m_bitmapFontTiny, std::to_string(m_player->getMp()), tibia::Colors::white);
        statusText.setPosition
        (
            tibia::GuiData::StatusWindow::Stats::Mp::x + tibia::GuiData::StatusWindow::Icons::width + 1 - statusText.getVertexArray()->getBounds().width,
            tibia::GuiData::StatusWindow::Stats::Mp::y + tibia::GuiData::StatusWindow::Icons::width + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(statusText);

        // LEV
        statusText.setText(&m_bitmapFontTiny, std::to_string(m_player->getLevel()), tibia::Colors::white);
        statusText.setPosition
        (
            tibia::GuiData::StatusWindow::Stats::Level::x + tibia::GuiData::StatusWindow::Icons::width + 1 - statusText.getVertexArray()->getBounds().width,
            tibia::GuiData::StatusWindow::Stats::Level::y + tibia::GuiData::StatusWindow::Icons::width + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(statusText);

        // MAG
        statusText.setText(&m_bitmapFontTiny, std::to_string(m_player->getMagicLevel()), tibia::Colors::white);
        statusText.setPosition
        (
            tibia::GuiData::StatusWindow::Stats::magicLevel::x + tibia::GuiData::StatusWindow::Icons::width + 1 - statusText.getVertexArray()->getBounds().width,
            tibia::GuiData::StatusWindow::Stats::magicLevel::y + tibia::GuiData::StatusWindow::Icons::width + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(statusText);
    }

    void drawEquipmentWindow(sf::RenderWindow* mainWindow)
    {
        sf::Sprite equipmentWindow;
        equipmentWindow.setTexture(tibia::Textures::equipment);
        equipmentWindow.setPosition(tibia::GuiData::EquipmentWindow::position);

        tibia::Sprite slotContainer;
        slotContainer.setId(tibia::SpriteData::backpack);
        slotContainer.setPosition(tibia::GuiData::EquipmentWindow::Slots::Container::position);
        slotContainer.setScale(tibia::GuiData::EquipmentWindow::Slots::Container::scale, tibia::GuiData::EquipmentWindow::Slots::Container::scale);

        mainWindow->draw(equipmentWindow);
        mainWindow->draw(slotContainer);
    }

    void drawInventoryWindow(sf::RenderWindow* mainWindow)
    {
        sf::Sprite inventoryWindow;

        if (this->gui.isInventorySplit == true)
        {
            inventoryWindow.setTexture(tibia::Textures::inventorySplit);
        }
        else
        {
            inventoryWindow.setTexture(tibia::Textures::inventory);
        }

        inventoryWindow.setPosition(tibia::GuiData::InventoryWindow::position);

        tibia::Sprite inventoryIconContainer;
        inventoryIconContainer.setId(tibia::SpriteData::backpack);
        inventoryIconContainer.setPosition(tibia::GuiData::InventoryWindow::Icons::Container::position);

        mainWindow->draw(inventoryWindow);
        mainWindow->draw(inventoryIconContainer);

        tibia::Creature::InventoryItemList* inventoryList = m_player->getInventoryItemList();

        if (inventoryList->size() > tibia::GuiData::InventoryWindow::Slots::numSlotsVisible)
        {
            if (tibia::Utility::getViewPosition(m_inventorySlotsWindowView).y < getInventoryWindowSlotsViewMaxScrollY())
            {
                tibia::Sprite buttonScrollDown;
                buttonScrollDown.setId(tibia::SpriteData::Gui::scrollButtonDown[1]);
                buttonScrollDown.setPosition(tibia::GuiData::InventoryWindow::Buttons::ScrollDown::position);

                mainWindow->draw(buttonScrollDown);

                this->gui.inventoryWindowButtonScrollDown = true;
            }
            else
            {
                this->gui.inventoryWindowButtonScrollDown = false;
            }
        }
        else
        {
            this->gui.inventoryWindowButtonScrollDown = false;
        }

        if (tibia::Utility::getViewPosition(m_inventorySlotsWindowView).y > 0)
        {
            tibia::Sprite buttonScrollUp;
            buttonScrollUp.setId(tibia::SpriteData::Gui::scrollButtonUp[1]);
            buttonScrollUp.setPosition(tibia::GuiData::InventoryWindow::Buttons::ScrollUp::position);

            mainWindow->draw(buttonScrollUp);

            this->gui.inventoryWindowButtonScrollUp = true;
        }
        else
        {
            this->gui.inventoryWindowButtonScrollUp = false;
        }

        tibia::Creature::InventoryItemList* inventoryItemList = m_player->getInventoryItemList();

        if (inventoryItemList->size() != 0)
        {
            m_inventorySlotsWindow.setView(m_inventorySlotsWindowView);
            m_inventorySlotsWindow.clear(tibia::Colors::transparent);

            tibia::BitmapFontText inventoryItemCountText;

            int inventoryItemIndex = 0;

            sf::Vector2f inventoryItemPosition(0, 0);

            for (auto& inventoryItem : *inventoryItemList)
            {
                tibia::Sprite sprInventoryItem;
                sprInventoryItem.setId(inventoryItem.id);
                sprInventoryItem.setPosition(inventoryItemPosition);

                m_inventorySlotsWindow.draw(sprInventoryItem);

                if (inventoryItem.count > 1)
                {
                    inventoryItemCountText.setText(&m_bitmapFontTiny, std::to_string(inventoryItem.count), tibia::Colors::white);

                    inventoryItemCountText.setPosition
                    (
                        inventoryItemPosition.x + tibia::GuiData::InventoryWindow::Slots::width  + 1 - inventoryItemCountText.getVertexArray()->getBounds().width,
                        inventoryItemPosition.y + tibia::GuiData::InventoryWindow::Slots::height + 1 - tibia::BitmapFonts::Tiny::glyphSize.y
                    );

                    m_inventorySlotsWindow.draw(inventoryItemCountText);
                }

                if (inventoryItemIndex % tibia::GuiData::InventoryWindow::Slots::numSlotsHorizontal == 2)
                {
                    inventoryItemPosition.x = 0;

                    inventoryItemPosition.y += tibia::TILE_SIZE + tibia::GuiData::InventoryWindow::Slots::distanceBetweenSlots;
                }
                else
                {
                    inventoryItemPosition.x += tibia::TILE_SIZE + tibia::GuiData::InventoryWindow::Slots::distanceBetweenSlots;
                }

                inventoryItemIndex++;
            }

            m_inventorySlotsWindow.display();

            m_inventorySlotsWindowSprite.setTexture(m_inventorySlotsWindow.getTexture());
            m_inventorySlotsWindowSprite.setTextureRect(sf::IntRect(0, 0, tibia::GuiData::InventoryWindow::Slots::Window::width, tibia::GuiData::InventoryWindow::Slots::Window::height));
            m_inventorySlotsWindowSprite.setPosition(tibia::GuiData::InventoryWindow::Slots::Window::position);

            mainWindow->draw(m_inventorySlotsWindowSprite);
        }
    }

    void drawSkillsWindow(sf::RenderWindow* mainWindow)
    {
        sf::Sprite skillsWindow;
        skillsWindow.setTexture(tibia::Textures::skills);
        skillsWindow.setPosition(tibia::GuiData::SkillsWindow::position);

        mainWindow->draw(skillsWindow);

        std::vector<int> skillList =
        {
            m_player->skills.fistFighting,
            m_player->skills.clubFighting,
            m_player->skills.swordFighting,
            m_player->skills.axeFighting,
            m_player->skills.distanceFighting,
            m_player->skills.shielding,
            m_player->skills.fishing,
        };

        int skillTextX = tibia::GuiData::SkillsWindow::Text::startX;
        int skillTextY = tibia::GuiData::SkillsWindow::Text::startY;

        tibia::BitmapFontText skillText;

        for (auto skillValue : skillList)
        {
            skillText.setText(&m_bitmapFontTiny, std::to_string(skillValue), tibia::Colors::white);
            skillText.setPosition
            (
                skillTextX + tibia::GuiData::SkillsWindow::Text::alignTextRightOffset - skillText.getVertexArray()->getBounds().width,
                skillTextY
            );

            mainWindow->draw(skillText);

            skillTextY += tibia::GuiData::SkillsWindow::Text::distanceBetweenText;
        }

        // vocation
        skillText.setText(&m_bitmapFontTiny, "Vocation:", tibia::Colors::white, true);
        skillText.setPosition
        (
            tibia::GuiData::SkillsWindow::x + (tibia::GuiData::SkillsWindow::width  / 2),
            tibia::GuiData::SkillsWindow::y + (tibia::GuiData::SkillsWindow::height / 2) - (tibia::Fonts::System::characterSize / 2)
        );

        mainWindow->draw(skillText);

        skillText.setText(&m_bitmapFontTiny, tibia::UMaps::vocations[m_player->getVocation()], tibia::Colors::white, true);
        skillText.setPosition
        (
            tibia::GuiData::SkillsWindow::x + (tibia::GuiData::SkillsWindow::width  / 2),
            tibia::GuiData::SkillsWindow::y + (tibia::GuiData::SkillsWindow::height / 2) + (tibia::Fonts::System::characterSize / 2)
        );

        mainWindow->draw(skillText);
    }

    void drawOutfitButtons(sf::RenderWindow* mainWindow)
    {
        tibia::Sprite outfitButtonHead;
        outfitButtonHead.setId(tibia::SpriteData::Gui::outfitButtonHead);
        outfitButtonHead.setPosition(tibia::GuiData::OutfitButtons::Head::position);

        tibia::Sprite outfitButtonBody;
        outfitButtonBody.setId(tibia::SpriteData::Gui::outfitButtonBody);
        outfitButtonBody.setPosition(tibia::GuiData::OutfitButtons::Body::position);

        tibia::Sprite outfitButtonLegs;
        outfitButtonLegs.setId(tibia::SpriteData::Gui::outfitButtonLegs);
        outfitButtonLegs.setPosition(tibia::GuiData::OutfitButtons::Legs::position);

        tibia::Sprite outfitButtonFeet;
        outfitButtonFeet.setId(tibia::SpriteData::Gui::outfitButtonFeet);
        outfitButtonFeet.setPosition(tibia::GuiData::OutfitButtons::Feet::position);

        tibia::Sprite outfitButtonAll;
        outfitButtonAll.setId(tibia::SpriteData::Gui::outfitButtonAll);
        outfitButtonAll.setPosition(tibia::GuiData::OutfitButtons::All::position);

        mainWindow->draw(outfitButtonHead);
        mainWindow->draw(outfitButtonBody);
        mainWindow->draw(outfitButtonLegs);
        mainWindow->draw(outfitButtonFeet);
        mainWindow->draw(outfitButtonAll);

        tibia::BitmapFontText outfitText;

        outfitText.setText(&m_bitmapFontTiny, "Head", tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Head::position.x + tibia::GuiData::OutfitButtons::borderSize,
            tibia::GuiData::OutfitButtons::Head::position.y + tibia::GuiData::OutfitButtons::borderSize
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, std::to_string(m_player->getOutfitHead() + 1), tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Head::position.x +
                tibia::GuiData::OutfitButtons::width  + 1 - tibia::GuiData::OutfitButtons::borderSize - outfitText.getVertexArray()->getBounds().width,
            tibia::GuiData::OutfitButtons::Head::position.y +
                tibia::GuiData::OutfitButtons::height + 1 - tibia::GuiData::OutfitButtons::borderSize - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, "Body", tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Body::position.x + tibia::GuiData::OutfitButtons::borderSize,
            tibia::GuiData::OutfitButtons::Body::position.y + tibia::GuiData::OutfitButtons::borderSize
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, std::to_string(m_player->getOutfitBody() + 1), tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Body::position.x +
                tibia::GuiData::OutfitButtons::width  + 1 - tibia::GuiData::OutfitButtons::borderSize - outfitText.getVertexArray()->getBounds().width,
            tibia::GuiData::OutfitButtons::Body::position.y +
                tibia::GuiData::OutfitButtons::height + 1 - tibia::GuiData::OutfitButtons::borderSize - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, "Legs", tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Legs::position.x + tibia::GuiData::OutfitButtons::borderSize,
            tibia::GuiData::OutfitButtons::Legs::position.y + tibia::GuiData::OutfitButtons::borderSize
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, std::to_string(m_player->getOutfitLegs() + 1), tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Legs::position.x +
                tibia::GuiData::OutfitButtons::width  + 1 - tibia::GuiData::OutfitButtons::borderSize - outfitText.getVertexArray()->getBounds().width,
            tibia::GuiData::OutfitButtons::Legs::position.y +
                tibia::GuiData::OutfitButtons::height + 1 - tibia::GuiData::OutfitButtons::borderSize - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, "Feet", tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Feet::position.x + tibia::GuiData::OutfitButtons::borderSize,
            tibia::GuiData::OutfitButtons::Feet::position.y + tibia::GuiData::OutfitButtons::borderSize
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, std::to_string(m_player->getOutfitFeet() + 1), tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::Feet::position.x +
                tibia::GuiData::OutfitButtons::width  + 1 - tibia::GuiData::OutfitButtons::borderSize - outfitText.getVertexArray()->getBounds().width,
            tibia::GuiData::OutfitButtons::Feet::position.y +
                tibia::GuiData::OutfitButtons::height + 1 - tibia::GuiData::OutfitButtons::borderSize - tibia::BitmapFonts::Tiny::glyphSize.y
        );

        mainWindow->draw(outfitText);

        outfitText.setText(&m_bitmapFontTiny, "All", tibia::Colors::white);
        outfitText.setPosition
        (
            tibia::GuiData::OutfitButtons::All::position.x + tibia::GuiData::OutfitButtons::borderSize,
            tibia::GuiData::OutfitButtons::All::position.y + tibia::GuiData::OutfitButtons::borderSize
        );

        mainWindow->draw(outfitText);
    }

    void drawCombatWindow(sf::RenderWindow* mainWindow)
    {
        sf::Sprite combatWindow;
        combatWindow.setTexture(tibia::Textures::combat);
        combatWindow.setPosition(tibia::GuiData::CombatWindow::position);

        mainWindow->draw(combatWindow);

        m_combatCreaturesWindowNumCreatures = 0;

        tibia::Creature::List* creatureList = &m_tileMapCreatures[m_player->getZ()];

        if (creatureList->size() != 0)
        {
            m_combatCreaturesWindow.setView(m_combatCreaturesWindowView);
            m_combatCreaturesWindow.clear(tibia::Colors::transparent);

            sf::Vector2f creaturePosition(tibia::GuiData::CombatWindow::Creatures::creatureIconLeftOffset, 0);

            for (auto& creature : *creatureList)
            {
                if (creature->isPlayer() == true)
                {
                    continue;
                }

                if (creature->isDead() == true)
                {
                    sf::Clock* clockDead = creature->getClockDead();

                    sf::Time timeElapsed = clockDead->getElapsedTime();

                    if (timeElapsed.asSeconds() >= tibia::GuiData::CombatWindow::Creatures::timeSecondsToShowDeadCreatures)
                    {
                        continue;
                    }
                }

                m_combatCreaturesWindowNumCreatures++;

                if (creature->hasOutfit() == true)
                {
                    tibia::Sprite outfitFeet;
                    outfitFeet.setId(tibia::Outfits::feet[(creature->getOutfitFeet() * 4) + tibia::Directions::down]);
                    outfitFeet.setPosition(creaturePosition);

                    m_combatCreaturesWindow.draw(outfitFeet);

                    tibia::Sprite outfitLegs;
                    outfitLegs.setId(tibia::Outfits::legs[(creature->getOutfitLegs() * 4) + tibia::Directions::down]);
                    outfitLegs.setPosition(creaturePosition);

                    m_combatCreaturesWindow.draw(outfitLegs);

                    tibia::Sprite outfitBody;
                    outfitBody.setId(tibia::Outfits::body[(creature->getOutfitBody() * 4) + tibia::Directions::down]);
                    outfitBody.setPosition(creaturePosition);

                    m_combatCreaturesWindow.draw(outfitBody);

                    tibia::Sprite outfitHead;
                    outfitHead.setId(tibia::Outfits::head[(creature->getOutfitHead() * 4) + tibia::Directions::down]);
                    outfitHead.setPosition(creaturePosition);

                    m_combatCreaturesWindow.draw(outfitHead);
                }
                else
                {
                    if (creature->getSize() == tibia::CreatureSizes::small)
                    {
                        tibia::Sprite sprCreature;
                        sprCreature.setId(creature->getSpritesList()->at(tibia::Directions::down));
                        sprCreature.setPosition(creaturePosition);

                        m_combatCreaturesWindow.draw(sprCreature);
                    }
                    else if (creature->getSize() == tibia::CreatureSizes::medium)
                    {
                        // top to bottom

                        tibia::Sprite sprCreature1;
                        sprCreature1.setId(creature->getSpritesList()->at(tibia::Directions::down) - 1);

                        if (creature->getType() == tibia::CreatureTypes::santaClaus || creature->getType() == tibia::CreatureTypes::witch)
                        {
                            // santa claus and witch have a hat drawn above their head
                            sprCreature1.setPosition(sf::Vector2f(creaturePosition.x, creaturePosition.y - tibia::GuiData::CombatWindow::Creatures::height));
                        }
                        else
                        {
                            sprCreature1.setPosition(creaturePosition);
                            sprCreature1.setScale(sf::Vector2f(1.0f, 0.5f));
                        }

                        tibia::Sprite sprCreature2;
                        sprCreature2.setId(creature->getSpritesList()->at(tibia::Directions::down));

                        if (creature->getType() == tibia::CreatureTypes::santaClaus || creature->getType() == tibia::CreatureTypes::witch)
                        {
                            // santa claus and witch body are drawn normally
                            sprCreature2.setPosition(creaturePosition);
                        }
                        else
                        {
                            sprCreature2.setPosition(sf::Vector2f(creaturePosition.x, creaturePosition.y + (tibia::GuiData::CombatWindow::Creatures::height / 2)));
                            sprCreature2.setScale(sf::Vector2f(1.0f, 0.5f));
                        }

                        m_combatCreaturesWindow.draw(sprCreature1);
                        m_combatCreaturesWindow.draw(sprCreature2);
                    }
                    else if (creature->getSize() == tibia::CreatureSizes::large)
                    {
                        // left to right, top to bottom

                        tibia::Sprite sprCreature1;
                        sprCreature1.setId(creature->getSpritesList()->at(tibia::Directions::down) - 3);
                        sprCreature1.setPosition(creaturePosition);
                        sprCreature1.setScale(sf::Vector2f(0.5f, 0.5f));

                        tibia::Sprite sprCreature2;
                        sprCreature2.setId(creature->getSpritesList()->at(tibia::Directions::down) - 2);
                        sprCreature2.setPosition(sf::Vector2f(creaturePosition.x + (tibia::GuiData::CombatWindow::Creatures::width / 2), creaturePosition.y));
                        sprCreature2.setScale(sf::Vector2f(0.5f, 0.5f));

                        tibia::Sprite sprCreature3;
                        sprCreature3.setId(creature->getSpritesList()->at(tibia::Directions::down) - 1);
                        sprCreature3.setPosition(sf::Vector2f(creaturePosition.x, creaturePosition.y + (tibia::GuiData::CombatWindow::Creatures::height / 2)));
                        sprCreature3.setScale(sf::Vector2f(0.5f, 0.5f));

                        tibia::Sprite sprCreature4;
                        sprCreature4.setId(creature->getSpritesList()->at(tibia::Directions::down));
                        sprCreature4.setPosition(sf::Vector2f(creaturePosition.x + (tibia::GuiData::CombatWindow::Creatures::width / 2), creaturePosition.y + (tibia::GuiData::CombatWindow::Creatures::height / 2)));
                        sprCreature4.setScale(sf::Vector2f(0.5f, 0.5f));

                        m_combatCreaturesWindow.draw(sprCreature1);
                        m_combatCreaturesWindow.draw(sprCreature2);
                        m_combatCreaturesWindow.draw(sprCreature3);
                        m_combatCreaturesWindow.draw(sprCreature4);
                    }
                }

                sf::Text creatureText;
                creatureText.setFont(m_fontSystem);
                creatureText.setCharacterSize(tibia::Fonts::System::characterSize);
                creatureText.setColor(sf::Color::Black);

                creatureText.setString(creature->getName());
                creatureText.setPosition
                (
                    sf::Vector2f
                    (
                        creaturePosition.x + tibia::GuiData::CombatWindow::Creatures::width + tibia::GuiData::CombatWindow::Creatures::creatureTextLeftOffsetFromIcon - 1,
                        creaturePosition.y + tibia::GuiData::CombatWindow::Creatures::distanceBetweenText - 1
                    )
                );

                m_combatCreaturesWindow.draw(creatureText);

                creatureText.setString(tibia::UMaps::healthStates[creature->getHealthState()]);
                creatureText.setPosition
                (
                    sf::Vector2f
                    (
                        creaturePosition.x + tibia::GuiData::CombatWindow::Creatures::width + tibia::GuiData::CombatWindow::Creatures::creatureTextLeftOffsetFromIcon - 1,
                        creaturePosition.y + (tibia::GuiData::CombatWindow::Creatures::distanceBetweenText * 2) + tibia::Fonts::System::characterSize - 1
                    )
                );

                m_combatCreaturesWindow.draw(creatureText);

                creaturePosition.y += tibia::GuiData::CombatWindow::Creatures::height + tibia::GuiData::CombatWindow::Creatures::distanceBetweenCreatures;
            }

            m_combatCreaturesWindow.display();

            m_combatCreaturesWindowSprite.setTexture(m_combatCreaturesWindow.getTexture());
            m_combatCreaturesWindowSprite.setTextureRect(sf::IntRect(0, 0, tibia::GuiData::CombatWindow::Creatures::Window::width, tibia::GuiData::CombatWindow::Creatures::Window::height));
            m_combatCreaturesWindowSprite.setPosition(tibia::GuiData::CombatWindow::Creatures::Window::position);

            mainWindow->draw(m_combatCreaturesWindowSprite);
        }

        if (m_combatCreaturesWindowNumCreatures > tibia::GuiData::CombatWindow::Creatures::numCreaturesVisible)
        {
            if (tibia::Utility::getViewPosition(m_combatCreaturesWindowView).y < getCombatWindowCreaturesViewMaxScrollY())
            {
                tibia::Sprite buttonScrollDown;
                buttonScrollDown.setId(tibia::SpriteData::Gui::scrollButtonDown[1]);
                buttonScrollDown.setPosition(tibia::GuiData::CombatWindow::Buttons::ScrollDown::position);

                mainWindow->draw(buttonScrollDown);

                this->gui.combatWindowButtonScrollDown = true;
            }
            else
            {
                this->gui.combatWindowButtonScrollDown = false;
            }

            // automatically scroll to bottom if you scrolled past the maximum by creatures moving around
            if (tibia::Utility::getViewPosition(m_combatCreaturesWindowView).y > getCombatWindowCreaturesViewMaxScrollY())
            {
                doCombatWindowCreaturesViewScrollToBottom();
            }
        }
        else
        {
            // automatically scroll to top when there are not enough creatures in the list for scrolling down
            doCombatWindowCreaturesViewScrollToTop();

            this->gui.combatWindowButtonScrollDown = false;
        }

        if (tibia::Utility::getViewPosition(m_combatCreaturesWindowView).y > 0)
        {
            tibia::Sprite buttonScrollUp;
            buttonScrollUp.setId(tibia::SpriteData::Gui::scrollButtonUp[1]);
            buttonScrollUp.setPosition(tibia::GuiData::CombatWindow::Buttons::ScrollUp::position);

            mainWindow->draw(buttonScrollUp);

            this->gui.combatWindowButtonScrollUp = true;
        }
        else
        {
            this->gui.combatWindowButtonScrollUp = false;
        }
    }

    void drawCombatButtons(sf::RenderWindow* mainWindow)
    {
        int spriteIndex = 0;

        if (this->gui.combatButtonsState == tibia::GuiData::CombatButtons::State::offense)
        {
            spriteIndex = 1;
        }

        tibia::Sprite combatButtonFullOffense;
        combatButtonFullOffense.setId(tibia::SpriteData::Gui::combatButtonFullOffense[spriteIndex]);
        combatButtonFullOffense.setPosition(tibia::GuiData::CombatButtons::FullOffense::position);

        mainWindow->draw(combatButtonFullOffense);

        spriteIndex = 0;

        if (this->gui.combatButtonsState == tibia::GuiData::CombatButtons::State::normal)
        {
            spriteIndex = 1;
        }

        tibia::Sprite combatButtonHalfOffenseDefense;
        combatButtonHalfOffenseDefense.setId(tibia::SpriteData::Gui::combatButtonHalfOffenseDefense[spriteIndex]);
        combatButtonHalfOffenseDefense.setPosition(tibia::GuiData::CombatButtons::HalfOffenseDefense::position);

        mainWindow->draw(combatButtonHalfOffenseDefense);

        spriteIndex = 0;

        if (this->gui.combatButtonsState == tibia::GuiData::CombatButtons::State::defense)
        {
            spriteIndex = 1;
        }

        tibia::Sprite combatButtonFullDefense;
        combatButtonFullDefense.setId(tibia::SpriteData::Gui::combatButtonFullDefense[spriteIndex]);
        combatButtonFullDefense.setPosition(tibia::GuiData::CombatButtons::FullDefense::position);

        mainWindow->draw(combatButtonFullDefense);
    }

    bool isTileMapVisible(tibia::TileMap* tileMap)
    {
        tibia::Tile::List* tileList = tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return false;
        }

        int x1 = getGameWindowViewDrawRect().left;
        int y1 = getGameWindowViewDrawRect().top;

        int x2 = getGameWindowViewDrawRect().width;
        int y2 = getGameWindowViewDrawRect().height;

        for (int i = x1; i < x1 + x2; i++)
        {
            for (int j = y1; j < y1 + y2; j++)
            {
                if (i < 0) continue;
                if (j < 0) continue;

                if (i > m_map.getWidth()  - 1) continue;
                if (j > m_map.getHeight() - 1) continue;

                int tileNumber = i + j * m_map.getWidth();

                if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
                {
                    continue;
                }

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

                int tileId = tile->getId();

                if (tileId != tibia::TILE_NULL)
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool findTileAboveThing(tibia::Thing::Ptr thing, int z)
    {
        tibia::Tile::List* tileList = m_map.tileMapTiles[z].getTileList();

        if (tileList->size() == 0)
        {
            return false;
        }

        int tileNumber = m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() - tibia::TILE_SIZE, thing->getTileY() - tibia::TILE_SIZE));

        if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
        {
            return false;
        }

        tibia::Tile::Ptr tile = tileList->at(tileNumber);

        if (tile->getId() != tibia::TILE_NULL)
        {
            if (tile->getZ() > thing->getZ())
            {
                return true;
            }
        }

        return false;
    }

    bool findTilesAboveThing(tibia::Thing::Ptr thing, int z)
    {
        tibia::Tile::List* tileList = m_map.tileMapTiles[z].getTileList();

        if (tileList->size() == 0)
        {
            return false;
        }

        std::vector<int> nearbyTileNumbers;

        //for (int i = -2; i < 3; i++)
        for (int i = -2; i < 2; i++)
        {
            nearbyTileNumbers.push_back(m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() - (2 * tibia::TILE_SIZE), thing->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() - (1 * tibia::TILE_SIZE), thing->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX()                         , thing->getTileY() + (i * tibia::TILE_SIZE))));
            nearbyTileNumbers.push_back(m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() + (1 * tibia::TILE_SIZE), thing->getTileY() + (i * tibia::TILE_SIZE))));
            //nearbyTileNumbers.push_back(m_map.getTileNumberByTileCoords(sf::Vector2u(thing->getTileX() + (2 * tibia::TILE_SIZE), thing->getTileY() + (i * tibia::TILE_SIZE))));
        }

        for (auto tileNumber : nearbyTileNumbers)
        {
            if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
            {
                continue;
            }

            tibia::Tile::Ptr tile = tileList->at(tileNumber);

            if (tile->getId() != tibia::TILE_NULL)
            {
                if (tile->getZ() > thing->getZ())
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool isWaterVisible()
    {
        int x1 = getGameWindowViewDrawRect().left;
        int y1 = getGameWindowViewDrawRect().top;

        int x2 = getGameWindowViewDrawRect().width;
        int y2 = getGameWindowViewDrawRect().height;

        for (int i = x1; i < x1 + x2; i++)
        {
            for (int j = y1; j < y1 + y2; j++)
            {
                if (i < 0) continue;
                if (j < 0) continue;

                if (i > m_map.getWidth()  - 1) continue;
                if (j > m_map.getHeight() - 1) continue;

                int tileNumber = i + j * m_map.getWidth();

                if (m_map.isTileNumberOutOfBounds(tileNumber) == true)
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
        unsigned int zBegin = tibia::ZAxis::min;
        unsigned int zEnd   = tibia::ZAxis::max;

        if (m_player->getZ() < tibia::ZAxis::ground)
        {
            zBegin = tibia::ZAxis::min;
            zEnd   = tibia::ZAxis::underGround;
        }
        else
        {
            zBegin = tibia::ZAxis::ground;
            zEnd   = tibia::ZAxis::max;
        }

        for (unsigned int i = zBegin; i < zEnd + 1; i++)
        {
            //std::cout << "doAnimateObjectList Z: " << i << std::endl;

            doAnimateObjectList(&m_tileMapObjects[i]);
        }
    }

    void doAnimateObjectList(tibia::Object::List* objectList)
    {
        if (objectList->size() == 0)
        {
            return;
        }

        int numAnimatedObjects = 0;

        for (auto object : *objectList)
        {
            if (object->getFlags() & tibia::SpriteFlags::animated)
            {
                object->doAnimation();

                //std::cout << "animated object id: "    << object->getId()    << std::endl;
                //std::cout << "animated object flags: " << object->getFlags() << std::endl;

                numAnimatedObjects++;
            }
        }

        //std::cout << "numAnimatedObjects: " << numAnimatedObjects << std::endl;
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

    sf::Font* getFontConsole()
    {
        return &m_fontConsole;
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

    tibia::Creature::Ptr getPlayer()
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

    void setMainWindow(sf::RenderWindow* window)
    {
        m_mainWindow = window;
    }

private:

    sf::RenderWindow* m_mainWindow;

    sf::Clock m_clockDelta;
    sf::Time m_timeDelta;

    sf::Clock m_clockGame;
    sf::Clock m_clockAnimatedWaterAndObjects;

    sf::Sprite m_mouseCursor;

    sf::Vector2i m_mouseWindowPosition;

    tibia::Tile::Ptr m_mouseTile;

    sf::Font m_fontDefault;
    sf::Font m_fontConsole;
    sf::Font m_fontSystem;

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

    sf::RenderTexture m_gameWindowLayer;
    sf::Sprite m_gameWindowLayerSprite;

    float m_gameWindowZoomLevel  = 1.0f;
    float m_gameWindowZoomFactor = 0.4f;

    tibia::GameText m_gameWindowText;

    sf::View m_miniMapWindowView;
    sf::RenderTexture m_miniMapWindow;
    sf::Sprite m_miniMapWindowSprite;

    int m_miniMapWindowZoom;

    sf::RenderTexture m_chatLogWindow;
    sf::View m_chatLogWindowView;
    sf::Sprite m_chatLogWindowSprite;

    std::vector<sf::Text> m_chatLogWindowTextList;

    sf::RenderTexture m_inventorySlotsWindow;
    sf::View m_inventorySlotsWindowView;
    sf::Sprite m_inventorySlotsWindowSprite;

    sf::RenderTexture m_combatCreaturesWindow;
    sf::View m_combatCreaturesWindowView;
    sf::Sprite m_combatCreaturesWindowSprite;

    int m_combatCreaturesWindowNumCreatures;

    sf::RenderTexture m_lightLayer;
    sf::Sprite m_lightLayerSprite;

    tibia::Light m_light;

    unsigned int m_lightBrightness;

    int m_timeOfDay;

    sf::Clock clockLightBrightness;

    tibia::Map m_map;

    std::vector<sf::Vertex> m_miniMapVertices;

    sf::VertexArray m_tileMapTileVertices;

    tibia::Object::List m_tileMapObjects[tibia::NUM_Z_LEVELS];
    tibia::Creature::List m_tileMapCreatures[tibia::NUM_Z_LEVELS];
    tibia::Animation::List m_tileMapAnimations[tibia::NUM_Z_LEVELS];
    tibia::Projectile::List m_tileMapProjectiles[tibia::NUM_Z_LEVELS];
    tibia::Thing::List m_tileMapThings[tibia::NUM_Z_LEVELS];

    tibia::Projectile::List m_projectileList[tibia::NUM_Z_LEVELS];

    tibia::Creature::Ptr m_player;

    tibia::Sound::List m_soundList;

}; // class Game

} // namespace tibia

#endif // TIBIA_GAME_HPP

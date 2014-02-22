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
#include "tibia/Tile.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Map.hpp"
#include "tibia/Thing.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Player.hpp"
#include "tibia/Animation.hpp"
#include "tibia/Projectile.hpp"

namespace tibia
{

class Game
{

public:

    typedef std::vector<tibia::Thing*>::iterator thingsListIterator;

    typedef std::shared_ptr<tibia::Creature> creatureSharedPtr;
    typedef std::vector<creatureSharedPtr>::iterator creaturesListIterator;

    typedef std::shared_ptr<tibia::Animation> animationSharedPtr;
    typedef std::vector<animationSharedPtr>::iterator animationsListIterator;
    typedef std::vector<animationSharedPtr>::iterator animatedDecalsListIterator;

    typedef std::shared_ptr<tibia::Projectile> projectileSharedPtr;
    typedef std::vector<projectileSharedPtr>::iterator projectilesListIterator;

    typedef std::shared_ptr<sf::Sound> soundSharedPtr;
    typedef std::vector<soundSharedPtr>::iterator soundsListIterator;

    Game()
    :
        m_windowView(sf::FloatRect(0, 0, tibia::TILES_WIDTH, tibia::TILES_HEIGHT)),
        m_miniMapWindowView(sf::FloatRect(0, 0, tibia::TILES_WIDTH * 2, tibia::TILES_HEIGHT * 2))
    {

        creatureSharedPtr player(new tibia::Creature);
        player->setName("Player");
        player->setIsPlayer(true);
        player->setHasOutfit(true);
        player->setIsSitting(false);
        player->setTeam(tibia::Teams::good);
        player->setHpMax(1000);
        player->setHp(1000);
        player->setMovementSpeed(tibia::MovementSpeeds::player);

        m_player = player;

        spawnCreature(m_player);

        m_rtLight.create(tibia::MAP_TILE_XY_MAX, tibia::MAP_TILE_XY_MAX);

        m_rectLight.setPosition(0, 0);
        m_rectLight.setSize(sf::Vector2f(tibia::MAP_TILE_XY_MAX, tibia::MAP_TILE_XY_MAX));
    }

    bool createWindows()
    {
        if (m_window.create(tibia::TILES_WIDTH, tibia::TILES_HEIGHT) == false)
        {
            return false;
        }

        m_windowBorder.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
        m_windowBorder.setOutlineColor(tibia::Colors::windowBorderColor);
        m_windowBorder.setOutlineThickness(1);
        m_windowBorder.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        if (m_miniMapWindow.create(tibia::GuiData::miniMapWindowSizeX, tibia::GuiData::miniMapWindowSizeY) == false)
        {
            return false;
        }

        m_miniMapWindowBorder.setSize(sf::Vector2f(m_miniMapWindow.getSize().x, m_miniMapWindow.getSize().y));
        m_miniMapWindowBorder.setOutlineColor(tibia::Colors::windowBorderColor);
        m_miniMapWindowBorder.setOutlineThickness(1);
        m_miniMapWindowBorder.setPosition(tibia::GuiData::miniMapWindowX, tibia::GuiData::miniMapWindowY);

        return true;
    }

    bool loadTextures()
    {
        if (tibia::Textures::sprites.loadFromFile("images/sprites.png") == false)
        {
            return false;
        }

        if (tibia::Textures::font.loadFromFile("images/font.png") == false)
        {
            return false;
        }

        if (tibia::Textures::font2.loadFromFile("images/font2.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light.loadFromFile("images/light.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light2.loadFromFile("images/light2.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light3.loadFromFile("images/light3.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light4.loadFromFile("images/light4.png") == false)
        {
            return false;
        }

        if (tibia::Textures::light5.loadFromFile("images/light5.png") == false)
        {
            return false;
        }

        return true;
    }

    bool loadFonts()
    {
        if (m_font.loadFromFile(tibia::Fonts::default) == false)
        {
            return false;
        }

        if (m_fontSmall.loadFromFile(tibia::Fonts::small) == false)
        {
            return false;
        }

        if (m_fontMartel.loadFromFile(tibia::Fonts::martel) == false)
        {
            return false;
        }

        return true;
    }

    bool loadSounds()
    {
        if (tibia::Sounds::death.loadFromFile("sounds/everquest/snd1/death_m.wav") == false)
        {
            return false;
        }

        return true;
    }

    bool loadMap(std::string filename)
    {
        return m_map.load(filename);
    }

    void loadCreatures()
    {
        //
    }

    void handleInput()
    {
        /*
        bool isKeyControlPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        {
            isKeyControlPressed = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            handleCreatureMovement(m_player.get(), tibia::getDirectionByKey(sf::Keyboard::Up), isKeyControlPressed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            handleCreatureMovement(m_player.get(), tibia::getDirectionByKey(sf::Keyboard::Right), isKeyControlPressed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            handleCreatureMovement(m_player.get(), tibia::getDirectionByKey(sf::Keyboard::Down), isKeyControlPressed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            handleCreatureMovement(m_player.get(), tibia::getDirectionByKey(sf::Keyboard::Left), isKeyControlPressed);
        }
        */
    }

    bool handleCreatureMovement(tibia::Creature* creature, int direction, bool turnOnly = false)
    {
        bool collisionOccured = false;

        if (turnOnly == false)
        {
            if (checkMovementCollision(creature, direction) == false)
            {
                if (checkMovementMoveAboveOrBelow(creature, direction) == true)
                {
                    return false;
                }

                checkMovementStepTile(creature, direction, true);

                creature->doMove(direction);

                checkMovementStepTile(creature, direction, false);
            }
            else
            {
                collisionOccured == true;
            }
        }

        creature->doTurn(direction);

        creature->setIsSitting(checkCreatureIsSitting(creature));

        return collisionOccured;
    }

    void doCreatureLogic()
    {
        for (auto creature : m_creaturesList)
        {
            if (creature->isPlayer() == true)
            {
                continue;
            }

            if (creature->isDead() == true)
            {
                continue;
            }

            if (m_creaturesList.size() > tibia::CREATURES_MAX_LOAD)
            {
                if (creature->getDistanceFromPlayer() > tibia::DRAW_DISTANCE_MAX)
                {
                    continue;
                }
            }

            int random = tibia::getRandomNumber(1, 100);

            if (random > 50)
            {
                int direction = creature->getDirection();

                if (random > 75)
                {
                    direction = tibia::getRandomNumber(tibia::Directions::up, tibia::Directions::left);
                }

                if (handleCreatureMovement(creature.get(), direction) == false)
                {
                    if (random > 90)
                    {
                        creature->doTurn(direction);
                    }
                }
            }
            else
            {
                for (auto findCreature : m_creaturesList)
                {
                    if (findCreature == creature)
                    {
                        continue;
                    }

                    if (findCreature->isDead() == true)
                    {
                        continue;
                    }

                    if (findCreature->getZ() != creature->getZ())
                    {
                        continue;
                    }

                    if (findCreature->getTeam() == creature->getTeam() || findCreature->getTeam() == tibia::Teams::neutral)
                    {
                        continue;
                    }

                    sf::Vector2f origin;
                    origin.x = creature->getTileX();
                    origin.y = creature->getTileY();

                    sf::Vector2f destination;
                    destination.x = findCreature->getTileX();
                    destination.y = findCreature->getTileY();

                    sf::Vector2f normal = tibia::getNormalByVectors(origin, destination);

                    normal.x = tibia::getFloatNormalEx(normal.x);
                    normal.y = tibia::getFloatNormalEx(normal.y);

                    int direction = tibia::getDirectionByVector(normal);

                    if (random < 25)
                    {
                        if (calculateDistanceBetweenCreatures(creature.get(), findCreature.get()) > tibia::ProjectileRanges::default)
                        {
                            continue;
                        }

                        sf::Vector2u checkBlockProjectilesPosition = getTileCoordsByCreatureDirection(creature.get(), direction);

                        if (checkTileIsBlockProjectiles(checkBlockProjectilesPosition, creature->getZ()) == true)
                        {
                            continue;
                        }

                        int projectileType = tibia::ProjectileTypes::spellFire;

                        switch (creature->getTeam())
                        {
                        case tibia::Teams::good:
                            projectileType = tibia::ProjectileTypes::spellBlue;
                            break;

                        case tibia::Teams::evil:
                            projectileType = tibia::ProjectileTypes::spellBlack;
                            break;
                        }

                        bool projectileIsDiagnonal = true;

                        if (findCreature->getTileX() == creature->getTileX() || findCreature->getTileY() == creature->getTileY())
                        {
                            projectileIsDiagnonal = false;
                        }

                        bool creatureShouldShootProjectile = true;

                        int random3 = tibia::getRandomNumber(1, 100);

                        if (random3 > 10 && projectileIsDiagnonal == true)
                        {
                            creatureShouldShootProjectile = false;
                        }

                        if (creatureShouldShootProjectile == true)
                        {
                            spawnProjectile
                            (
                                creature.get(),
                                projectileType,
                                direction,
                                origin,
                                destination
                            );
                        }

                        creature->doTurn(direction);

                        break;
                    }
                    else
                    {
                        int random2 = tibia::getRandomNumber(1, 100);

                        if (random2 > 10)
                        {
                            continue;
                        }

                        handleCreatureMovement(creature.get(), direction);

                        creature->doTurn(direction);

                        break;
                    }
                }
            }

            random = tibia::getRandomNumber(1, 100);

            if (random > 10)
            {
                doCreatureUseLadder(creature.get(), creature->getTilePosition());
            }

            creature->update();
        }
    }

    void updatePlayer()
    {
        m_player->update();
    }

    void updateCreatures()
    {
        for (auto creature : m_creaturesList)
        {
            if (creature->isPlayer() == true)
            {
                continue;
            }

            float distanceFromPlayer = calculateDistanceBetweenCreatures(m_player.get(), creature.get());

            creature->setDistanceFromPlayer(distanceFromPlayer);

            creature->update();
        }
    }

    void updateAnimations()
    {
        for (auto animation : m_animationsList)
        {
            animation->update();
        }
    }

    void updateAnimatedDecals()
    {
        for (auto animatedDecal : m_animatedDecalsList)
        {
            animatedDecal->update();
        }
    }

    void updateProjectiles()
    {
        for (auto projectile : m_projectilesList)
        {
            projectile->update();
        }
    }

    void updateSounds()
    {
        soundsListIterator soundsList_it;
        for (soundsList_it = m_soundsList.begin(); soundsList_it != m_soundsList.end(); soundsList_it++)
        {
            sf::Sound* sound = soundsList_it->get();

            if (sound->getStatus() == sf::SoundSource::Status::Stopped)
            {
                soundsList_it = m_soundsList.erase(soundsList_it);

                //if (m_soundsList.size() == 0)
                //{
                    //return;
                //}

                soundsList_it--;
                continue;
            }
        }
    }

    void spawnSound(soundSharedPtr sound)
    {
        sound->play();

        m_soundsList.push_back(sound);
    }

    void spawnCreature(creatureSharedPtr creature)
    {
        m_creaturesSpawnList.push_back(creature);
    }

    void spawnAnimation(int animationId[], int tileX, int tileY, int z, float frameTime = tibia::AnimationTimes::default)
    {
        if (m_player->getZ() == tibia::ZAxis::underGround && z != tibia::ZAxis::underGround)
        {
            return;
        }

        animationSharedPtr animation(new tibia::Animation(animationId[0], animationId[1]));

        animation->setTileCoords(tileX, tileY);
        animation->setZ(z);
        animation->setFrameTime(frameTime);

        if
        (
            animationId[0] == tibia::Animations::fire[0]        ||
            animationId[0] == tibia::Animations::electricity[0]
        )
        {
            animation->setNumRepeat(1);
        }

        m_animationsSpawnList.push_back(animation);
    }

    void spawnAnimatedDecal(int animationId[], int tileX, int tileY, int z, float frameTime = tibia::AnimationTimes::decal)
    {
        animationSharedPtr animatedDecal(new tibia::Animation(animationId[0], animationId[1]));
        animatedDecal->setTileCoords(tileX, tileY);
        animatedDecal->setZ(z);
        animatedDecal->setFrameTime(frameTime);

        m_animatedDecalsSpawnList.push_back(animatedDecal);
    }

    void spawnProjectile(tibia::Creature* creature, int projectileType, int direction, sf::Vector2f origin, sf::Vector2f destination, bool isPrecise = false, bool isChild = false)
    {
        projectileSharedPtr projectile(new tibia::Projectile(projectileType, direction, origin, destination, isPrecise, isChild));
        projectile->setTileCoords(origin.x, origin.y);
        projectile->setZ(creature->getZ());
        projectile->setCreatureOwner(creature);

        m_projectilesSpawnList.push_back(projectile);
    }

    void drawCreatureBars()
    {
        if (m_creaturesList.size() == 0)
        {
            return;
        }

        int barWidth  = 24;
        int barHeight = 2;

        sf::RectangleShape barBackground(sf::Vector2f(barWidth, barHeight));
        barBackground.setFillColor(tibia::Colors::black);
        barBackground.setOutlineThickness(1);
        barBackground.setOutlineColor(tibia::Colors::black);

        sf::RectangleShape barHealth(sf::Vector2f(barWidth, barHeight));
        barHealth.setFillColor(tibia::Colors::white);
        barHealth.setOutlineThickness(0);

        for (auto creature : m_creaturesList)
        {
            if (creature->isPlayer() == true)
            {
                continue;
            }

            if (creature->isDead() == true)// || creature->getHp() == creature->getHpMax())
            {
                continue;
            }

            if (creature->getZ() != m_player->getZ())
            {
                continue;
            }

            if (creature->getDistanceFromPlayer() > tibia::DRAW_DISTANCE_MAX)
            {
                continue;
            }

            switch (creature->getTeam())
            {
                case tibia::Teams::neutral:
                    continue;
                    break;

                case tibia::Teams::good:
                    barHealth.setFillColor(tibia::Colors::green);
                    break;

                case tibia::Teams::evil:
                    barHealth.setFillColor(tibia::Colors::red);
                    break;
            }

            sf::Vector2f barPosition
            (
                creature->getTileX() + ((tibia::TILE_SIZE - barWidth) / 2) - tibia::TILE_DRAW_OFFSET,
                creature->getTileY() - (tibia::TILE_SIZE / 2)
            );

            if (creature->isSitting() == true)
            {
                barPosition.x -= tibia::TILE_DRAW_OFFSET;
                barPosition.y -= tibia::TILE_DRAW_OFFSET;
            }

            barBackground.setPosition(barPosition);
            barHealth.setPosition(barPosition);

            int hp    = creature->getHp();
            int hpMax = creature->getHpMax();

            //int hpPercent = hpMax / hp;

            barHealth.setSize
            (
                sf::Vector2f
                (
                    hp * barWidth / hpMax,
                    barHealth.getSize().y
                )
            );

            m_window.draw(barBackground);
            m_window.draw(barHealth);
        }
    }

    void drawThings()
    {
        if (m_thingsList.size() == 0)
        {
            return;
        }

        std::sort(m_thingsList.begin(), m_thingsList.end(), tibia::Thing::sortByTileNumber());

        for (auto thing : m_thingsList)
        {
            m_window.draw(*thing);
        }
    }

    void drawPlayer()
    {
        m_window.draw(*m_player);
    }

    void drawCreatures(bool deadOnly = false)
    {
        creaturesListIterator creaturesSpawnList_it;
        for (creaturesSpawnList_it = m_creaturesSpawnList.begin(); creaturesSpawnList_it != m_creaturesSpawnList.end(); creaturesSpawnList_it++)
        {
            m_creaturesList.push_back(*creaturesSpawnList_it);
        }
        m_creaturesSpawnList.clear();

        if (m_creaturesList.size() == 0)
        {
            return;
        }

        creaturesListIterator creaturesList_it;
        for (creaturesList_it = m_creaturesList.begin(); creaturesList_it != m_creaturesList.end(); creaturesList_it++)
        {
            tibia::Creature* creature = creaturesList_it->get();

            if (creature == nullptr)
            {
                return;
            }

            if (creature->hasDecayed() == true)
            {
                creaturesList_it = m_creaturesList.erase(creaturesList_it);
                creaturesList_it--;
                continue;
            }

            if (deadOnly == true)
            {
                if (creature->isDead() == false)
                {
                    continue;
                }
            }
            else
            {
                if (creature->isDead() == true)
                {
                    continue;
                }
            }

            if (m_player->getZ() != tibia::ZAxis::underGround && creature->getZ() == tibia::ZAxis::underGround)
            {
                continue;
            }

            if (m_player->getZ() == tibia::ZAxis::underGround && creature->getZ() != tibia::ZAxis::underGround)
            {
                continue;
            }

            if (creature->getDistanceFromPlayer() > tibia::DRAW_DISTANCE_MAX)
            {
                continue;
            }

            //std::cout << "Drawing creature: " << creature->getName() << std::endl;

            if (creature->isDead() == true)
            {
                m_window.draw(*creature);
            }
            else
            {
                m_thingsList.push_back(creature);
            }
        }
    }

    void drawAnimations()
    {
        animationsListIterator animationsSpawnList_it;
        for (animationsSpawnList_it = m_animationsSpawnList.begin(); animationsSpawnList_it != m_animationsSpawnList.end(); animationsSpawnList_it++)
        {
            m_animationsList.push_back(*animationsSpawnList_it);
        }
        m_animationsSpawnList.clear();

        if (m_animationsList.size() == 0)
        {
            return;
        }

        animationsListIterator animationsList_it;
        for (animationsList_it = m_animationsList.begin(); animationsList_it != m_animationsList.end(); animationsList_it++)
        {
            tibia::Animation* animation = animationsList_it->get();

            if (animation == nullptr)
            {
                return;
            }

            if (animation->getCurrentFrame() > animation->getNumFrames() - 1)
            {
                animationsList_it = m_animationsList.erase(animationsList_it);
                animationsList_it--;
                continue;
            }

            if (m_player->getZ() != tibia::ZAxis::underGround && animation->getZ() == tibia::ZAxis::underGround)
            {
                continue;
            }

            if (m_player->getZ() == tibia::ZAxis::underGround && animation->getZ() != tibia::ZAxis::underGround)
            {
                continue;
            }

            m_window.draw(*animation);
        }
    }

    void drawAnimatedDecals()
    {
        animatedDecalsListIterator animatedDecalsSpawnList_it;
        for (animatedDecalsSpawnList_it = m_animatedDecalsSpawnList.begin(); animatedDecalsSpawnList_it != m_animatedDecalsSpawnList.end(); animatedDecalsSpawnList_it++)
        {
            m_animatedDecalsList.push_back(*animatedDecalsSpawnList_it);
        }
        m_animatedDecalsSpawnList.clear();

        if (m_animatedDecalsList.size() == 0)
        {
            return;
        }

        animatedDecalsListIterator animatedDecalsList_it;
        for (animatedDecalsList_it = m_animatedDecalsList.begin(); animatedDecalsList_it != m_animatedDecalsList.end(); animatedDecalsList_it++)
        {
            tibia::Animation* animatedDecal = animatedDecalsList_it->get();

            if (animatedDecal == nullptr)
            {
                return;
            }

            if (animatedDecal->getCurrentFrame() > animatedDecal->getNumFrames() - 1)
            {
                animatedDecalsList_it = m_animatedDecalsList.erase(animatedDecalsList_it);
                animatedDecalsList_it--;
                continue;
            }

            if (m_player->getZ() != tibia::ZAxis::underGround && animatedDecal->getZ() == tibia::ZAxis::underGround)
            {
                continue;
            }

            if (m_player->getZ() == tibia::ZAxis::underGround && animatedDecal->getZ() != tibia::ZAxis::underGround)
            {
                continue;
            }

            if (calculateDistanceBetweenThings(m_player.get(), animatedDecal) > tibia::DRAW_DISTANCE_MAX)
            {
                continue;
            }

            m_window.draw(*animatedDecal);
        }
    }

    void drawProjectiles()
    {
        projectilesListIterator projectilesSpawnList_it;
        for (projectilesSpawnList_it = m_projectilesSpawnList.begin(); projectilesSpawnList_it != m_projectilesSpawnList.end(); projectilesSpawnList_it++)
        {
            m_projectilesList.push_back(*projectilesSpawnList_it);
        }
        m_projectilesSpawnList.clear();

        if (m_projectilesList.size() == 0)
        {
            return;
        }

        projectilesListIterator projectilesList_it;
        for (projectilesList_it = m_projectilesList.begin(); projectilesList_it != m_projectilesList.end(); projectilesList_it++)
        {
            tibia::Projectile* projectile = projectilesList_it->get();

            if (projectile == nullptr)
            {
                return;
            }

            if (m_player->getZ() != tibia::ZAxis::underGround && projectile->getZ() == tibia::ZAxis::underGround)
            {
                continue;
            }

            if (m_player->getZ() == tibia::ZAxis::underGround && projectile->getZ() != tibia::ZAxis::underGround)
            {
                continue;
            }

            sf::Vector2f projectileSpritePosition     = projectile->getSpritePosition();
            sf::Vector2f projectileSpriteTilePosition = projectile->getSpriteTilePosition();

            //std::cout << "projectileSpriteTilePosition x,y: " << projectileSpriteTilePosition.x << "," << projectileSpriteTilePosition.y << std::endl;

            //spawnAnimation(tibia::Animations::spellBlue, projectileSpriteTilePosition.x, projectileSpriteTilePosition.y, projectile->getZ());

            int projectileDistanceTravelled = projectile->getDistanceTravelled();

            if (projectileDistanceTravelled > 0 && projectileDistanceTravelled % tibia::TILE_SIZE == 0)
            {
                if (checkTileIsBlockProjectiles(sf::Vector2u(projectileSpriteTilePosition.x, projectileSpriteTilePosition.y), projectile->getZ() == true))
                {
                    spawnAnimation
                    (
                        projectile->getAnimationOnBlock(),
                        projectileSpriteTilePosition.x,
                        projectileSpriteTilePosition.y,
                        projectile->getZ()
                    );

                    projectilesList_it = m_projectilesList.erase(projectilesList_it);
                    projectilesList_it--;
                    continue;
                }

                tibia::Creature* defender = checkTileHasCreature(sf::Vector2u(projectileSpriteTilePosition.x, projectileSpriteTilePosition.y), projectile->getZ());

                if (defender != nullptr)
                {
                    tibia::Creature* attacker = projectile->getCreatureOwner();

                    if (attacker != nullptr)
                    {
                        if (attacker == defender)
                        {
                            continue;
                        }

                        if (attacker->getTeam() == defender->getTeam())
                        {
                            continue;
                        }

                        int damage = projectile->getDamage();

                        defender->takeDamageFromCreature(attacker, damage);

                        std::cout << attacker->getName()
                                  << " hit "
                                  << defender->getName()
                                  << " for "
                                  << damage
                                  << " damage."
                                  << std::endl;

                        if (defender->isDead() == true)
                        {
                            //soundSharedPtr sound(new sf::Sound);
                            //sound->setBuffer(tibia::Sounds::death);
                            //sound->setVolume(tibia::calculateVolumeByDistance(creature->getDistanceFromPlayer()));
                            //spawnSound(sound);

                            int *deathAnimatedDecal = tibia::AnimatedDecals::poolRed;

                            if (projectile->getType() == tibia::ProjectileTypes::arrowPoison)
                            {
                                deathAnimatedDecal = tibia::AnimatedDecals::poolGreen;
                            }

                            spawnAnimatedDecal
                            (
                                deathAnimatedDecal,
                                defender->getTileX(),
                                defender->getTileY(),
                                defender->getZ(),
                                tibia::AnimationTimes::decal
                            );

                            std::cout << attacker->getName()
                                      << " killed "
                                      << defender->getName()
                                      << std::endl;
                        }
                    }

                    spawnAnimation
                    (
                        projectile->getAnimationOnHit(),
                        projectileSpriteTilePosition.x,
                        projectileSpriteTilePosition.y,
                        projectile->getZ()
                    );

                    if (defender->isDead() == false)
                    {
                        spawnAnimatedDecal
                        (
                            projectile->getAnimatedDecalOnHit(), //tibia::AnimatedDecals::splatRed,
                            projectileSpriteTilePosition.x,
                            projectileSpriteTilePosition.y,
                            projectile->getZ(),
                            tibia::AnimationTimes::decal
                        );
                    }

                    projectilesList_it = m_projectilesList.erase(projectilesList_it);
                    projectilesList_it--;
                    continue;
                }

                int projectileTileDistanceTravelled = projectile->getTileDistanceTravelled();

                //std::cout << "projectileTileDistanceTravelled: " << projectileTileDistanceTravelled << std::endl;

                if
                (
                    projectileTileDistanceTravelled >= projectile->getRange()
                    //std::abs(projectileSpritePosition.x) >= (projectile->getRange() * tibia::TILE_SIZE) ||
                    //std::abs(projectileSpritePosition.y) >= (projectile->getRange() * tibia::TILE_SIZE)
                )
                {
                    int *hitMissAnimation = tibia::Animations::hitMiss;

                    if (checkTileIsWater(sf::Vector2u(projectileSpriteTilePosition.x, projectileSpriteTilePosition.y), projectile->getZ() == true))
                    {
                        hitMissAnimation = tibia::Animations::waterSplash;
                    }

                    spawnAnimation
                    (
                        hitMissAnimation,
                        projectileSpriteTilePosition.x,
                        projectileSpriteTilePosition.y,
                        projectile->getZ()
                    );

                    projectilesList_it = m_projectilesList.erase(projectilesList_it);
                    projectilesList_it--;
                    continue;
                }
            }

            //tibia::Sprite spr;
            //spr.setId(1);
            //spr.setPosition(projectile->getSpriteTilePosition().x, projectile->getSpriteTilePosition().y);
            //m_window.draw(spr);

            m_window.draw(*projectile);
        }
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

        // poison
        animatedObject.clear();
        animatedObject.push_back(3158);
        animatedObject.push_back(3159);
        animatedObject.push_back(3160);
        animatedObject.push_back(3161);
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

        // candles
        animatedObject.clear();
        animatedObject.push_back(395);
        animatedObject.push_back(396);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(525);
        animatedObject.push_back(526);
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

        // christmas tree
        animatedObject.clear();
        animatedObject.push_back(3319);
        animatedObject.push_back(3323);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(3320);
        animatedObject.push_back(3324);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(3321);
        animatedObject.push_back(3325);
        animatedObjectsList.push_back(animatedObject);

        animatedObject.clear();
        animatedObject.push_back(3322);
        animatedObject.push_back(3326);
        animatedObjectsList.push_back(animatedObject);
    }

    void doAnimatedWater()
    {
        m_map.tileMapGroundTiles.doAnimatedWater();
    }

    void doAnimatedObjects()
    {
        m_map.doAnimatedObjects();
    }

    void drawTextList(std::vector<sf::Text> textList)
    {
        int textPositionOffsetY = 0;

        for (auto text : textList)
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

            textShadow.setPosition(text.getPosition().x,     text.getPosition().y + 1);
            m_window.draw(textShadow);

            textShadow.setPosition(text.getPosition().x,     text.getPosition().y - 1);
            m_window.draw(textShadow);

            textShadow.setPosition(text.getPosition().x + 1, text.getPosition().y - 1);
            m_window.draw(textShadow);

            textShadow.setPosition(text.getPosition().x - 1, text.getPosition().y + 1);
            m_window.draw(textShadow);

            m_window.draw(text);

            textPositionOffsetY += tibia::TILE_SIZE;
        }
    }

    std::vector<sf::Text> createTextList(std::string text, unsigned int textSize, sf::Color textColor, sf::Vector2f textPosition)
    {
        std::vector<sf::Text> textList;

        std::vector<std::string> textTokens;

        boost::split(textTokens, text, boost::is_any_of("\n"));

        for (auto textToken : textTokens)
        {
            sf::Text text;
            text.setString(textToken);
            text.setFont(m_font);
            text.setCharacterSize(textSize);
            text.setColor(textColor);

            //sf::FloatRect textRect = text.getLocalBounds();

            //text.setOrigin
            //(
                //textRect.left + (textRect.width  / 2),
                //textRect.top  + (textRect.height / 2)
            //);

            text.setPosition
            (
                textPosition.x - (text.getLocalBounds().width / 2),
                textPosition.y
            );

            textList.push_back(text);
        }

        return textList;
    }

    void showGameText(std::string text, unsigned int textSize, sf::Color textColor)
    {
        m_textList.clear();

        m_textList = createTextList
        (
            text,
            textSize,
            textColor,
            sf::Vector2f
            (
                (m_player->getTileX() + tibia::TILE_DRAW_OFFSET),
                (m_player->getTileY() - (tibia::TILE_SIZE * 2))
            )
        );

        m_clockText.restart();
    }

    void drawGameText()
    {
        sf::Time timeText = m_clockText.getElapsedTime();

        if (timeText.asSeconds() < tibia::TEXT_TIME)
        {
            drawTextList(m_textList);
        }
    }

    void drawGameWindow(sf::RenderWindow* mainWindow)
    {
        m_windowView.setCenter
        (
            m_player->getTileX() + (tibia::TILE_SIZE / 2),
            m_player->getTileY() + (tibia::TILE_SIZE / 2)
        );

        m_window.setView(m_windowView);
        m_window.clear(tibia::Colors::black);

        int playerX = m_player->getTileX();
        int playerY = m_player->getTileY();
        int playerZ = m_player->getZ();

        if (playerZ == tibia::ZAxis::underGround)
        {
            m_window.draw(m_map.tileMapUnderGroundTiles);
            m_window.draw(m_map.tileMapUnderGroundEdges);
            m_window.draw(m_map.tileMapUnderGroundWalls);
            m_window.draw(m_map.tileMapUnderGroundWallsEx);
            m_window.draw(m_map.tileMapUnderGroundWalls2);
            m_window.draw(m_map.tileMapUnderGroundObjects);
            m_window.draw(m_map.tileMapUnderGroundObjects2);
            m_window.draw(m_map.tileMapUnderGroundObjects3);
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
            m_window.draw(m_map.tileMapGroundObjects3);
        }

        //

        drawAnimatedDecals();

        drawCreatures(true);
        //drawPlayer();
        drawCreatures(false);
        drawThings(); // creatures and player

        drawAnimations();

        drawProjectiles();

        //

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

        std::vector<int>* objectsDrawLastTiles;
        std::vector<int>* groundObjectsDrawLastTiles      = m_map.tileMapGroundObjectsDrawLast.getTiles();
        std::vector<int>* underGroundObjectsDrawLastTiles = m_map.tileMapUnderGroundObjectsDrawLast.getTiles();
        std::vector<int>* aboveGroundObjectsDrawLastTiles = m_map.tileMapAboveGroundObjectsDrawLast.getTiles();

        int numDrawThingAgain = 0;

        thingsListIterator thingsList_it;
        for (thingsList_it = m_thingsList.begin(); thingsList_it != m_thingsList.end(); thingsList_it++)
        {
            tibia::Thing* thing = *thingsList_it;

            int thingZ = thing->getZ();

            switch (thingZ)
            {
                case tibia::ZAxis::underGround:
                    objectsDrawLastTiles = underGroundObjectsDrawLastTiles;
                    break;

                case tibia::ZAxis::ground:
                    objectsDrawLastTiles = groundObjectsDrawLastTiles;
                    break;

                case tibia::ZAxis::aboveGround:
                    objectsDrawLastTiles = aboveGroundObjectsDrawLastTiles;
                    break;
            }

            if (objectsDrawLastTiles->size())
            {
                int thingX = thing->getTileX();
                int thingY = thing->getTileY();

                int objectsDrawLastTileNumberA = tibia::getTileNumberByTileCoords(thingX - 1, thingY - 1);
                int objectsDrawLastTileNumberB = tibia::getTileNumberByTileCoords(thingX,     thingY - 1);
                int objectsDrawLastTileNumberC = tibia::getTileNumberByTileCoords(thingX,     thingY);

                if
                (
                    objectsDrawLastTiles->at(objectsDrawLastTileNumberA) != tibia::TILE_NULL ||
                    objectsDrawLastTiles->at(objectsDrawLastTileNumberB) != tibia::TILE_NULL
                )
                {
                    if (objectsDrawLastTiles->at(objectsDrawLastTileNumberC) == tibia::TILE_NULL)
                    {
                        float distanceFromPlayer = calculateDistanceBetweenThings(m_player.get(), thing);

                        if (distanceFromPlayer < tibia::DRAW_DISTANCE_MAX)
                        {
                            //std::cout << "Drawing thing again" << std::endl;

                            numDrawThingAgain++;

                            m_window.draw(*thing);
                        }
                    }
                }
            }
        }

        if (numDrawThingAgain > 0)
        {
            drawAnimations();
            drawProjectiles();
        }

        //std::cout << "Had to redraw " << numDrawThingAgain << "/" << m_thingsList.size() << " things" << std::endl;

        if (playerZ != tibia::ZAxis::underGround)
        {
            bool drawAboveGround = true;

            if (playerZ == tibia::ZAxis::ground)
            {
                std::vector<int>* aboveGroundTiles = m_map.tileMapAboveGroundTiles.getTiles();

                std::vector<int> aboveGroundTileNumbers;

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

                std::vector<int>::iterator aboveGroundTileNumbers_it;
                for (aboveGroundTileNumbers_it = aboveGroundTileNumbers.begin(); aboveGroundTileNumbers_it != aboveGroundTileNumbers.end(); aboveGroundTileNumbers_it++)
                {
                    int tileNumber = *aboveGroundTileNumbers_it;

                    if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
                    {
                        continue;
                    }

                   if (aboveGroundTiles->at(tileNumber) != tibia::TILE_NULL)
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
                m_window.draw(m_map.tileMapAboveGroundObjects3);

                if (playerZ == tibia::ZAxis::aboveGround)
                {
                    drawPlayer();
                }

                m_window.draw(m_map.tileMapAboveGroundObjectsDrawLast);
                m_window.draw(m_map.tileMapAboveGroundObjectsDrawLast2);
            }
        }

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

        drawCreatureBars();

        drawGameText();

        m_thingsList.clear();

        m_window.display();

        m_windowSprite.setTexture(m_window.getTexture());
        m_windowSprite.setPosition(tibia::GuiData::gameWindowX, tibia::GuiData::gameWindowY);

        mainWindow->draw(m_windowBorder);
        mainWindow->draw(m_windowSprite);
    }

    std::vector<sf::Vertex> getMiniMapCreatures()
    {
        int playerZ = m_player->getZ();

        std::vector<sf::Vertex> verticesList;

        for (auto creature : m_creaturesList)
        {
            if (creature->isDead() == true)
            {
                continue;
            }

            if (creature->getZ() != playerZ)
            {
                continue;
            }

            if (creature->getDistanceFromPlayer() > (tibia::DRAW_DISTANCE_MAX * 2))
            {
                continue;
            }

            int tileNumber = creature->getTileNumber();

            if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
            {
                continue;
            }

            //std::cout << "tileNumber: " << tileNumber << std::endl;

            sf::Vertex quad[4];
            quad[0].position = sf::Vector2f(creature->getTileX(),                    creature->getTileY());
            quad[1].position = sf::Vector2f(creature->getTileX() + tibia::TILE_SIZE, creature->getTileY());
            quad[2].position = sf::Vector2f(creature->getTileX() + tibia::TILE_SIZE, creature->getTileY() + tibia::TILE_SIZE);
            quad[3].position = sf::Vector2f(creature->getTileX(),                    creature->getTileY() + tibia::TILE_SIZE);

            //quad[0].color = tibia::Colors::transparent;
            //quad[1].color = tibia::Colors::transparent;
            //quad[2].color = tibia::Colors::transparent;
            //quad[3].color = tibia::Colors::transparent;

            sf::Color creatureColor = tibia::Colors::white;

            switch (creature->getTeam())
            {
                case tibia::Teams::good:
                    creatureColor = tibia::Colors::green;
                    break;

                case tibia::Teams::evil:
                    creatureColor = tibia::Colors::red;
                    break;
            }

            quad[0].color = creatureColor;
            quad[1].color = creatureColor;
            quad[2].color = creatureColor;
            quad[3].color = creatureColor;

            verticesList.push_back(quad[0]);
            verticesList.push_back(quad[1]);
            verticesList.push_back(quad[2]);
            verticesList.push_back(quad[3]);
        }

        return verticesList;
    }

    void updateMiniMapWindow()
    {
        m_miniMapWindow.clear(tibia::Colors::black);

        int playerZ = m_player->getZ();

        std::vector<sf::Vertex> miniMapTiles;

        switch (playerZ)
        {
            case tibia::ZAxis::underGround:
                miniMapTiles = m_map.tileMapUnderGroundTiles.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapUnderGroundWalls.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapUnderGroundWallsEx.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapUnderGroundWalls2.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapUnderGroundObjects.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapUnderGroundObjects2.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapUnderGroundObjects3.getMiniMapTiles(miniMapTiles);
                break;

            case tibia::ZAxis::ground:
                miniMapTiles = m_map.tileMapGroundTiles.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapGroundWalls.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapGroundWallsEx.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapGroundWalls2.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapGroundObjects.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapGroundObjects2.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapGroundObjects3.getMiniMapTiles(miniMapTiles);
                break;

            case tibia::ZAxis::aboveGround:
                miniMapTiles = m_map.tileMapAboveGroundTiles.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapAboveGroundWalls.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapAboveGroundWallsEx.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapAboveGroundWalls2.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapAboveGroundObjects.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapAboveGroundObjects2.getMiniMapTiles(miniMapTiles);
                miniMapTiles = m_map.tileMapAboveGroundObjects3.getMiniMapTiles(miniMapTiles);
                break;
        }

        m_miniMapWindow.draw(&miniMapTiles[0], miniMapTiles.size(), sf::Quads);

        std::vector<sf::Vertex> miniMapCreatures = getMiniMapCreatures();

        m_miniMapWindow.draw(&miniMapCreatures[0], miniMapCreatures.size(), sf::Quads);

        sf::RectangleShape rectPlayer;
        rectPlayer.setSize(sf::Vector2f(tibia::TILE_SIZE, tibia::TILE_SIZE));
        rectPlayer.setPosition(m_player->getTileX(), m_player->getTileY());
        rectPlayer.setFillColor(tibia::Colors::pink);

        m_miniMapWindow.draw(rectPlayer);
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

        mainWindow->draw(m_miniMapWindowBorder);
        mainWindow->draw(m_miniMapWindowSprite);
    }

    bool checkCreatureIsSitting(tibia::Creature* creature)
    {
        creature->updateTileCoords();

        if (checkTileIsChair(sf::Vector2u(creature->getTileX(), creature->getTileY()), creature->getZ()) == true)
        {
            return true;
        }

        return false;
    }

    tibia::Tile getTileInfo(sf::Vector2u tilePosition, int tileZ, bool tilesOnly = false, bool objectsOnly = false)
    {
        tibia::Tile tileInfo;

        tileInfo.setIsNull(false);
        tileInfo.setIsSolid(false);
        tileInfo.setIsBlockProjectiles(false);
        tileInfo.setIsWater(false);
        tileInfo.setIsLava(false);
        tileInfo.setIsChair(false);
        tileInfo.setIsOffset(false);
        tileInfo.setIsLadder(false);
        tileInfo.setIsMoveAbove(false);
        tileInfo.setIsMoveBelow(false);

        tibia::Map::tileMapListIterator tileMapList_it;
        for (tileMapList_it = m_map.tileMapList.begin(); tileMapList_it != m_map.tileMapList.end(); tileMapList_it++)
        {
            tibia::TileMap* tileMap = *tileMapList_it;

            if (tileMap->getZ() != tileZ)
            {
                continue;
            }

            if
            (
                tileMap->getType() == tibia::TileMapTypes::edges ||
                tileMap->getType() == tibia::TileMapTypes::objectsDrawLast
            )
            {
                continue;
            }

            if (tilesOnly == true)
            {
                if (tileMap->getType() != tibia::TileMapTypes::tiles)
                {
                    continue;
                }
            }

            if (objectsOnly == true)
            {
                if (tileMap->getType() != tibia::TileMapTypes::objects)
                {
                    continue;
                }
            }

            int tileNumber = tibia::getTileNumberByTileCoords(tilePosition.x, tilePosition.y);

            if (tileNumber < 0 || tileNumber > tibia::TILE_NUMBER_MAX)
            {
                continue;
            }

            std::vector<tibia::TileMap::tileSharedPtr> *tilesList = tileMap->getTilesList();

            if (tilesList->size() == 0 || tilesList->size() < tileNumber)
            {
                continue;
            }

            tibia::Tile* tile = tilesList->at(tileNumber).get();

            if (tile->isNull() == true)
            {
                tileInfo.setIsNull(true);
            }

            if (tile->isSolid() == true)
            {
                tileInfo.setIsSolid(true);
            }

            if (tile->isBlockProjectiles() == true)
            {
                tileInfo.setIsBlockProjectiles(true);
            }

            if (tile->isWater() == true)
            {
                tileInfo.setIsWater(true);
            }

            if (tile->isLava() == true)
            {
                tileInfo.setIsLava(true);
            }

            if (tile->isChair() == true)
            {
                tileInfo.setIsChair(true);
            }

            if (tile->isLadder() == true)
            {
                tileInfo.setIsLadder(true);
            }

            if (tile->isMoveAbove() == true)
            {
                tileInfo.setIsMoveAbove(true);
            }

            if (tile->isMoveBelow() == true)
            {
                tileInfo.setIsMoveBelow(true);
            }
        }

        return tileInfo;
    }

    bool checkTileIsNull(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ);

        return tile.isNull();
    }

    bool checkTileIsSolid(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ);

        return tile.isSolid();
    }

    bool checkTileIsBlockProjectiles(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ);

        return tile.isBlockProjectiles();
    }

    bool checkTileIsWater(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ, true, false);

        return tile.isWater();
    }

    bool checkTileIsLava(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ, true, false);

        return tile.isLava();
    }

    bool checkTileIsChair(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ, false, true);

        return tile.isChair();
    }

    bool checkTileIsOffset(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ);

        return tile.isOffset();
    }

    bool checkTileIsLadder(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ, false, true);

        return tile.isLadder();
    }

    bool checkTileIsMoveAbove(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ, false, true);

        return tile.isMoveAbove();
    }

    bool checkTileIsMoveBelow(sf::Vector2u tilePosition, int tileZ)
    {
        tibia::Tile tile = getTileInfo(tilePosition, tileZ);

        return tile.isMoveBelow();
    }

    tibia::Creature* checkTileHasCreature(sf::Vector2u tilePosition, int tileZ, bool skipDead = true)
    {
        tibia::Creature* foundCreature = nullptr;

        for (auto creature : m_creaturesList)
        {
            //if (creature->isPlayer() == true)
            //{
                //continue;
            //}

            if (skipDead == true)
            {
                if (creature->isDead() == true)
                {
                    continue;
                }
            }

            if (creature->getZ() == tileZ)
            {
                if (creature->getTileX() == tilePosition.x && creature->getTileY() == tilePosition.y)
                {
                    return creature.get();
                }
            }
        }

        return foundCreature;
    }

    sf::Vector2u getTileCoordsByCreatureDirection(tibia::Creature* creature, int direction)
    {
        sf::Vector2u tileCoords;

        tileCoords.x = -1;
        tileCoords.y = -1;

        switch (direction)
        {
            case tibia::Directions::up:
                if (creature->getY() == 0)
                {
                    break;
                }
                tileCoords.x = creature->getTileX();
                tileCoords.y = creature->getTileY() - tibia::TILE_SIZE;
                break;

            case tibia::Directions::right:
                if (creature->getX() == tibia::MAP_SIZE - 1)
                {
                    break;
                }
                tileCoords.x = creature->getTileX() + tibia::TILE_SIZE;
                tileCoords.y = creature->getTileY();
                break;

            case tibia::Directions::down:
                if (creature->getY() == tibia::MAP_SIZE - 1)
                {
                    break;
                }
                tileCoords.x = creature->getTileX();
                tileCoords.y = creature->getTileY() + tibia::TILE_SIZE;
                break;

            case tibia::Directions::left:
                if (creature->getX() == 0)
                {
                    break;
                }
                tileCoords.x = creature->getTileX() - tibia::TILE_SIZE;
                tileCoords.y = creature->getTileY();
                break;

            case tibia::Directions::upLeft:
                if (creature->getY() == 0 || creature->getX() == 0)
                {
                    break;
                }
                tileCoords.x = creature->getTileX() - tibia::TILE_SIZE;
                tileCoords.y = creature->getTileY() - tibia::TILE_SIZE;
                break;

            case tibia::Directions::upRight:
                if (creature->getY() == 0 || creature->getX() == tibia::MAP_SIZE - 1)
                {
                    break;
                }
                tileCoords.x = creature->getTileX() + tibia::TILE_SIZE;
                tileCoords.y = creature->getTileY() - tibia::TILE_SIZE;
                break;

            case tibia::Directions::downLeft:
                if (creature->getY() == tibia::MAP_SIZE - 1 || creature->getX() == 0)
                {
                    break;
                }
                tileCoords.x = creature->getTileX() - tibia::TILE_SIZE;
                tileCoords.y = creature->getTileY() + tibia::TILE_SIZE;
                break;

            case tibia::Directions::downRight:
                if (creature->getY() == tibia::MAP_SIZE - 1 || creature->getX() == tibia::MAP_SIZE - 1)
                {
                    break;
                }
                tileCoords.x = creature->getTileX() + tibia::TILE_SIZE;
                tileCoords.y = creature->getTileY() + tibia::TILE_SIZE;
                break;
        }

        return tileCoords;
    }

    bool checkMovementCollision(tibia::Creature* creature, int direction)
    {
        bool collision = false;

        sf::Vector2u checkTileCoords = getTileCoordsByCreatureDirection(creature, direction);

        if (checkTileCoords.x < 0 || checkTileCoords.y < 0)
        {
            return true;
        }

        int checkTileNumber = tibia::getTileNumberByTileCoords(checkTileCoords.x, checkTileCoords.y);

        if (creature->getZ() == tibia::ZAxis::aboveGround)
        {
            int checkTileId = m_map.tileMapAboveGroundTiles.getTilesList()->at(checkTileNumber)->getId();

            if (checkTileId == tibia::TILE_NULL)
            {
                return true;
            }
        }

        if (checkTileIsSolid(checkTileCoords, creature->getZ()) == true)
        {
            return true;
        }

        if (checkTileHasCreature(checkTileCoords, creature->getZ()) != nullptr)
        {
            return true;
        }

        if (creature->getZ() == m_player->getZ())
        {
            if (checkTileCoords.x == m_player->getTileX() && checkTileCoords.y == m_player->getTileY())
            {
                return true;
            }
        }

        return collision;
    }

    bool checkMovementMoveAboveOrBelow(tibia::Creature* creature, int direction)
    {
        sf::Vector2u checkTileCoords = getTileCoordsByCreatureDirection(creature, direction);

        if (checkTileCoords.x < 0 || checkTileCoords.y < 0)
        {
            return false;
        }

        if (checkTileIsMoveAbove(checkTileCoords, creature->getZ()) == true)
        {
            doCreatureMoveAbove(creature, checkTileCoords);

            return true;
        }

        if (checkTileIsMoveBelow(checkTileCoords, creature->getZ()) == true)
        {
            doCreatureMoveBelow(creature, checkTileCoords);

            return true;
        }

        return false;
    }

    bool checkMovementStepTile(tibia::Creature* creature, int direction, bool stepOn)
    {
        int checkTileNumber = 0;

        if (stepOn == true)
        {
            sf::Vector2u checkTileCoords = getTileCoordsByCreatureDirection(creature, direction);

            if (checkTileCoords.x < 0 || checkTileCoords.y < 0)
            {
                return false;
            }

            checkTileNumber = tibia::getTileNumberByTileCoords(checkTileCoords.x, checkTileCoords.y);
        }
        else
        {
            checkTileNumber = creature->getTileNumber();
        }

        if (checkTileNumber == 0)
        {
            return false;
        }

        tibia::TileMap* tileMap;

        switch (creature->getZ())
        {
        case tibia::ZAxis::aboveGround:
            tileMap = &m_map.tileMapAboveGroundTiles;
            break;

        case tibia::ZAxis::ground:
            tileMap = &m_map.tileMapGroundTiles;
            break;

        case tibia::ZAxis::underGround:
            tileMap = &m_map.tileMapUnderGroundTiles;
            break;
        }

        int checkTileId = tileMap->getTilesList()->at(checkTileNumber)->getId();

        int newTileId = 0;

        if (stepOn == true)
        {
            if (checkTileId == tibia::SpriteData::stepTileWood[0])
            {
                newTileId = tibia::SpriteData::stepTileWood[1];
            }
            else if (checkTileId == tibia::SpriteData::stepTileStone[0])
            {
                newTileId = tibia::SpriteData::stepTileStone[1];
            }
        }
        else
        {
            if (checkTileId == tibia::SpriteData::stepTileWood[1])
            {
                newTileId = tibia::SpriteData::stepTileWood[0];
            }
            else if (checkTileId == tibia::SpriteData::stepTileStone[1])
            {
                newTileId = tibia::SpriteData::stepTileStone[0];
            }
        }

        if (newTileId == 0)
        {
            return false;
        }

        tileMap->updateTile(checkTileNumber, newTileId);

        return true;
    }

    bool doCreatureUseLever(tibia::Creature* creature, sf::Vector2u tilePosition)
    {
        int creatureDistanceFromTile = calculateDistanceByTile(creature->getTileX(), creature->getTileY(), tilePosition.x, tilePosition.y);

        //std::cout << "creatureDistanceFromTile: " << creatureDistanceFromTile << std::endl;

        if (creatureDistanceFromTile > 1)
        {
            return false;
        }

        tibia::TileMap* tileMap;

        switch (creature->getZ())
        {
            case tibia::ZAxis::aboveGround:
                tileMap = &m_map.tileMapAboveGroundObjects;
                break;

            case tibia::ZAxis::ground:
                tileMap = &m_map.tileMapGroundObjects;
                break;

            case tibia::ZAxis::underGround:
                tileMap = &m_map.tileMapUnderGroundObjects;
                break;
        }

        int tileNumber = tibia::getTileNumberByTileCoords(tilePosition.x, tilePosition.y);

        int checkTileId = tileMap->getTilesList()->at(tileNumber)->getId();

        int newTileId = 0;

        if (checkTileId == tibia::SpriteData::lever[0])
        {
            newTileId = tibia::SpriteData::lever[1];
        }
        else if (checkTileId == tibia::SpriteData::lever[1])
        {
            newTileId = tibia::SpriteData::lever[0];
        }

        if (newTileId == 0)
        {
            return false;
        }

        tileMap->updateTile(tileNumber, newTileId);

        return true;
    }

    bool doCreatureUseLadder(tibia::Creature* creature, sf::Vector2u tilePosition)
    {
        int creatureDistanceFromTile = calculateDistanceByTile(creature->getTileX(), creature->getTileY(), tilePosition.x, tilePosition.y);

        //std::cout << "creatureDistanceFromTile: " << creatureDistanceFromTile << std::endl;

        if (creatureDistanceFromTile > 1)
        {
            return false;
        }

        if (checkTileIsLadder(tilePosition, creature->getZ()) == true)
        {
            doCreatureMoveAbove(creature, tilePosition);

            return true;
        }

        return false;
    }

    void doCreatureMoveAbove(tibia::Creature* creature, sf::Vector2u tilePosition)
    {
        int moveTileX = tilePosition.x - tibia::TILE_SIZE;
        int moveTileY = tilePosition.y - (tibia::TILE_SIZE * 2);

        int moveZ = creature->getZ() + 1;

        int moveDirection = tibia::Directions::up;

        if (checkTileIsNull(sf::Vector2u(moveTileX, moveTileY), moveZ) == true)
        {
            return;
        }

        if (checkTileIsSolid(sf::Vector2u(moveTileX, moveTileY), moveZ) == true)
        {
            moveTileY = tilePosition.y;

            moveDirection = tibia::Directions::down;

            if (checkTileIsSolid(sf::Vector2u(moveTileX, moveTileY), moveZ) == true)
            {
                return;
            }
        }

        creature->setTileCoords(moveTileX, moveTileY);

        creature->setZ(moveZ);

        creature->setDirection(moveDirection);
    }

    void doCreatureMoveBelow(tibia::Creature* creature, sf::Vector2u tilePosition)
    {
        int moveTileX = tilePosition.x + tibia::TILE_SIZE;
        int moveTileY = tilePosition.y + (tibia::TILE_SIZE * 2);

        int moveZ = creature->getZ() - 1;

        if (checkTileIsNull(sf::Vector2u(moveTileX, moveTileY), moveZ) == true)
        {
            return;
        }

        if (checkTileIsSolid(sf::Vector2u(moveTileX, moveTileY), moveZ) == true)
        {
            return;
        }

        creature->setTileCoords(moveTileX, moveTileY);

        creature->setZ(moveZ);

        creature->setDirection(tibia::Directions::down);
    }

    /*
    bool checkTileHasObject(int id, sf::Vector2u tilePosition, int tileZ)
    {
        int tileNumber = tibia::getTileNumberByTileCoords(tilePosition.x, tilePosition.y);

        tibia::TileMap* tileMap;

        switch (tileZ)
        {
        case tibia::ZAxis::aboveGround:
            tileMap = &m_map.tileMapAboveGroundObjects;
            break;

        case tibia::ZAxis::ground:
            tileMap = &m_map.tileMapGroundObjects;
            break;

        case tibia::ZAxis::underGround:
            tileMap = &m_map.tileMapUnderGroundObjects;
            break;
        }

        int checkTileId = tileMap->getTilesList()->at(tileNumber)->getId();

        if (checkTileId == id)
        {
            return true;
        }

        return false;
    }
    */

    int getCreatureTileNumber(tibia::Creature* creature)
    {
        return tibia::getTileNumberByTileCoords(creature->getTileX(), creature->getTileY());
    }

    float calculateDistanceBetweenThings(tibia::Thing* a, tibia::Thing* b)
    {
        //std::cout << a->getX() << "," << a->getY() << " | " << b->getX() << "," <<  b->getY() <<  std::endl;

        return tibia::calculateDistance(a->getX(), a->getY(), b->getX(), b->getY());
    }

    float calculateDistanceBetweenCreatures(tibia::Creature* a, tibia::Creature* b)
    {
        //std::cout << a->getX() << "," << a->getY() << " | " << b->getX() << "," <<  b->getY() <<  std::endl;

        return tibia::calculateDistance(a->getX(), a->getY(), b->getX(), b->getY());
    }

    std::vector<creatureSharedPtr> getCreaturesList()
    {
        return m_creaturesList;
    }

    std::vector<animationSharedPtr> getAnimationsList()
    {
        return m_animationsList;
    }

    std::vector<animationSharedPtr> getAnimatedDecalsList()
    {
        return m_animatedDecalsList;
    }

    std::vector<projectileSharedPtr> getProjectilesList()
    {
        return m_projectilesList;
    }

    std::vector<soundSharedPtr> getSoundList()
    {
        return m_soundsList;
    }

    sf::Clock* getClock()
    {
        return &m_clock;
    }

    sf::Clock* getClockAnimatedWaterAndObjects()
    {
        return &m_clockAnimatedWaterAndObjects;
    }

    sf::Clock* getClockCreatureLogic()
    {
        return &m_clockCreatureLogic;
    }

    sf::Clock* getClockMiniMap()
    {
        return &m_clockMiniMap;
    }

    sf::RenderTexture* getWindow()
    {
        return &m_window;
    }

    sf::Sprite* getWindowSprite()
    {
        return &m_windowSprite;
    }

    sf::View* getWindowView()
    {
        return &m_windowView;
    }

    sf::Font* getFont()
    {
        return &m_font;
    }

    sf::Font* getFontSmall()
    {
        return &m_fontSmall;
    }

    sf::Font* getFontMartel()
    {
        return &m_fontMartel;
    }

    tibia::Map* getMap()
    {
        return &m_map;
    }

    tibia::Creature* getPlayer()
    {
        return m_player.get();
    }

private:

    sf::Clock m_clock;
    sf::Clock m_clockAnimatedWaterAndObjects;
    sf::Clock m_clockCreatureLogic;
    sf::Clock m_clockMiniMap;

    sf::RenderTexture m_window;
    sf::Sprite m_windowSprite;
    sf::RectangleShape m_windowBorder;
    sf::View m_windowView;

    sf::RenderTexture m_miniMapWindow;
    sf::Sprite m_miniMapWindowSprite;
    sf::RectangleShape m_miniMapWindowBorder;
    sf::View m_miniMapWindowView;

    sf::RenderTexture m_rtLight;
    sf::RectangleShape m_rectLight;

    sf::Font m_font;
    sf::Font m_fontSmall;
    sf::Font m_fontMartel;

    std::vector<sf::Text> m_textList;

    sf::Clock m_clockText;

    tibia::Map m_map;

    //tibia::Player m_player;
    creatureSharedPtr m_player;

    std::vector<tibia::Thing*> m_thingsList;

    std::vector<creatureSharedPtr> m_creaturesList;
    std::vector<creatureSharedPtr> m_creaturesSpawnList;

    std::vector<animationSharedPtr> m_animationsList;
    std::vector<animationSharedPtr> m_animationsSpawnList;

    std::vector<animationSharedPtr> m_animatedDecalsList;
    std::vector<animationSharedPtr> m_animatedDecalsSpawnList;

    std::vector<projectileSharedPtr> m_projectilesList;
    std::vector<projectileSharedPtr> m_projectilesSpawnList;

    std::vector<soundSharedPtr> m_soundsList;
    std::vector<soundSharedPtr> m_soundsSpawnList;
};

}

#endif // TIBIA_GAME_HPP

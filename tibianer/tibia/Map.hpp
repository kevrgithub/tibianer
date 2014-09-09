#ifndef TIBIA_MAP_HPP
#define TIBIA_MAP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include <boost/range/algorithm/remove_if.hpp>

#include "base64.hpp"
#include "boost_zlib.hpp"

#include "pugixml.hpp"

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Object.hpp"
#include "tibia/Creature.hpp"

namespace tibia
{

class Map
{

public:

    struct Properties_t
    {
        std::string name   = "Unknown Map";
        std::string author = "Unknown Author";
        std::string description;

        unsigned int playerStartX = 0;
        unsigned int playerStartY = 0;
        unsigned int playerStartZ = tibia::ZAxis::default;

        unsigned int timeOfDay = tibia::TimeOfDay::day;
    };

    Properties_t properties;

    tibia::TileMap tileMapTiles[tibia::NUM_Z_LEVELS];
    tibia::TileMap tileMapTileEdges[tibia::NUM_Z_LEVELS];

    bool load(const std::string& filename)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result loadResult = doc.load_file(filename.c_str());

        if (loadResult == pugi::xml_parse_status::status_ok)
        {
            std::cout << "Map load error: " << loadResult.description() << std::endl;
            return false;
        }

        m_isCompressed = false;

        m_filename = filename;

        pugi::xml_node docMap = doc.first_child();
        m_width  = docMap.attribute("width").as_uint();
        m_height = docMap.attribute("height").as_uint();

        m_tileWidth  = m_width * tibia::TILE_SIZE;
        m_tileHeight = m_width * tibia::TILE_SIZE;

        m_size = m_width * m_height;

        tibia::mapWidth  = m_width;
        tibia::mapHeight = m_height;

        tibia::mapSize = m_size;

        std::cout << "Map size: " << m_width << "x" << m_height << " (" << m_size << " tiles)" << std::endl;

        auto docMapProperties = docMap.child("properties");

        if (docMapProperties == NULL)
        {
            std::cout << "Map load error: Properties not found!" << std::endl;
            return false;
        }

        for (auto docMapProperty : docMapProperties.children("property"))
        {
            std::string docMapPropertyName = docMapProperty.attribute("name").value();

            if (docMapPropertyName == "name")
            {
                properties.name = docMapProperty.attribute("value").value();
            }
            else if (docMapPropertyName == "description")
            {
                properties.description = docMapProperty.attribute("value").value();
            }
            else if (docMapPropertyName == "author")
            {
                properties.author = docMapProperty.attribute("value").value();
            }

            else if (docMapPropertyName == "player_start_x")
            {
                properties.playerStartX = docMapProperty.attribute("value").as_uint();
            }
            else if (docMapPropertyName == "player_start_y")
            {
                properties.playerStartY = docMapProperty.attribute("value").as_uint();
            }
            else if (docMapPropertyName == "player_start_z")
            {
                properties.playerStartZ = docMapProperty.attribute("value").as_uint();
            }

            else if (docMapPropertyName == "time_of_day")
            {
                properties.timeOfDay = tibia::UMaps::timeOfDay[docMapProperty.attribute("value").value()];
            }
        }

        auto docMapLayerFind = docMap.child("layer");

        if (docMapLayerFind == NULL)
        {
            std::cout << "Map load error: Layers not found!" << std::endl;
            return false;
        }

        for (auto docMapLayer : docMap.children("layer"))
        {
            std::string docMapLayerName = docMapLayer.attribute("name").value();

            //std::cout << "Layer Name: " << docMapLayerName << std::endl;

            auto docMapLayerData = docMapLayer.child("data");

            if (docMapLayerData == NULL)
            {
                std::cout << "Map load error: Layer data not found for '" << docMapLayerName << "'" << std::endl;
                return false;
            }

            std::string docMapLayerDataString = docMapLayerData.text().get();

            // remove spaces and new lines
            docMapLayerDataString.erase(boost::remove_if(docMapLayerDataString, boost::is_any_of(" \r\n")), docMapLayerDataString.end());

            // skip empty layers, huge performance boost
            if (docMapLayerDataString == "eJztwTEBAAAAwqD1T20IX6AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD4DAPAAAQ==")
            {
                continue;
            }

            //std::cout << "Decoding base64 map layer: " << docMapLayerName << std::endl;

            docMapLayerDataString = base64_decode(docMapLayerDataString);

            for (auto docMapLayerDataAttr : docMapLayerData.attributes())
            {
                std::string docMapLayerDataAttrName = docMapLayerDataAttr.name();

                if (docMapLayerDataAttrName == "compression")
                {
                    m_isCompressed = true;

                    //std::cout << "Decompressing zlib map layer: " << docMapLayerName << std::endl;

                    docMapLayerDataString = boost_zlib_decompress_string_fast(docMapLayerDataString);

                    break;
                }
            }

            //std::cout << docMapLayerDataString << std::endl;

            std::istringstream docMapLayerDataStream(docMapLayerDataString);

            std::vector<int> docMapLayerDataTiles;
            docMapLayerDataTiles.reserve(docMapLayerDataString.size() / 4);

            for (std::size_t i = 0; i < docMapLayerDataString.size(); i += 4)
            {
                int tileId;
                docMapLayerDataStream.read(reinterpret_cast<char*>(&tileId), 4);

                docMapLayerDataTiles.push_back(tileId);
            }

            int tileMapZ = tibia::ZAxis::default;

            int tileMapType = tibia::TileMapTypes::tiles;

            auto docMapLayerProperties = docMapLayer.child("properties");

            if (docMapLayerProperties == NULL)
            {
                std::cout << "Map load error: Layer properties not found for '" << docMapLayerName << "'" << std::endl;
                return false;
            }

            for (auto docMapLayerProperty : docMapLayerProperties.children("property"))
            {
                std::string docMapLayerPropertyName = docMapLayerProperty.attribute("name").value();

                int docMapLayerPropertyValue = docMapLayerProperty.attribute("value").as_int();

                //std::cout << "Layer Property: " << docMapLayerPropertyName << "=" << docMapLayerPropertyValue << std::endl;

                if (docMapLayerPropertyName == "z")
                {
                    tileMapZ = docMapLayerPropertyValue;
                }
                else if (docMapLayerPropertyName == "type")
                {
                    tileMapType = docMapLayerPropertyValue;
                }
            }

            if (tileMapType == tibia::TileMapTypes::tiles)
            {
                tileMapTiles[tileMapZ].load(m_width, m_height, docMapLayerDataTiles, docMapLayerName, tileMapType, tileMapZ);
            }
            else if (tileMapType == tibia::TileMapTypes::tileEdges)
            {
                tileMapTileEdges[tileMapZ].load(m_width, m_height, docMapLayerDataTiles, docMapLayerName, tileMapType, tileMapZ);
            }
        }

        auto docMapObjectGroupFind = docMap.child("objectgroup");

        if (docMapObjectGroupFind == NULL)
        {
            std::cout << "Map load error: Object Groups not found!" << std::endl;
            return false;
        }

        for (auto docMapObjectGroup : docMap.children("objectgroup"))
        {
            std::string docMapObjectGroupName = docMapObjectGroup.attribute("name").value();

            //std::cout << docMapObjectGroupName << std::endl;

            int docMapObjectZ = tibia::ZAxis::default;

            int docMapObjectLayerType = tibia::ObjectLayerTypes::objects;

            auto docMapObjectGroupProperties = docMapObjectGroup.child("properties");

            if (docMapObjectGroupProperties == NULL)
            {
                std::cout << "Map load error: Object Group properties not found for '" << docMapObjectGroupName << "'" << std::endl;
                return false;
            }

            for (auto docMapObjectGroupProperty : docMapObjectGroupProperties.children("property"))
            {
                std::string docMapObjectGroupPropertyName = docMapObjectGroupProperty.attribute("name").value();

                int docMapObjectGroupPropertyValue = docMapObjectGroupProperty.attribute("value").as_int();

                //std::cout << "Object Group Property: " << docMapObjectGroupPropertyName << "=" << docMapObjectGroupPropertyValue << std::endl;

                if (docMapObjectGroupPropertyName == "z")
                {
                    docMapObjectZ = docMapObjectGroupPropertyValue;
                }
                else if (docMapObjectGroupPropertyName == "type")
                {
                    docMapObjectLayerType = docMapObjectGroupPropertyValue;
                }
            }

            tibia::TileMap* tileMap = &tileMapTiles[docMapObjectZ];

            //auto docMapObjectFind = docMapObjectGroup.child("object");

            //if (docMapObjectFind == NULL)
            //{
                //std::cout << "Map load error: Object Group objects not found for '" << docMapObjectGroupName << "'" << std::endl;
                //return false;
            //}

            for (auto docMapObject : docMapObjectGroup.children("object"))
            {
                int docMapObjectId;
                int docMapObjectTileX;
                int docMapObjectTileY;

                std::string docMapObjectType = "null";

                for (auto docMapObjectAttr : docMapObject.attributes())
                {
                    std::string docMapObjectAttrName = docMapObjectAttr.name();

                    if (docMapObjectAttrName == "gid")
                    {
                        docMapObjectId = docMapObjectAttr.as_int();
                    }
                    else if (docMapObjectAttrName == "x")
                    {
                        docMapObjectTileX = docMapObjectAttr.as_int();
                    }
                    else if (docMapObjectAttrName == "y")
                    {
                        docMapObjectTileY = docMapObjectAttr.as_int() - tibia::TILE_SIZE; // y-axis bug for objects in Tiled map editor?;
                    }
                    else if (docMapObjectAttrName == "type")
                    {
                        docMapObjectType = docMapObjectAttr.value();
                    }
                }

                tibia::Tile::List* tileList = tileMap->getTileList();

                int tileNumber = getTileNumberByTileCoords(sf::Vector2u(docMapObjectTileX, docMapObjectTileY));

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

                int objectType = tibia::UMaps::objectTypes[docMapObjectType];

                int objectCount = 0;

                if (docMapObjectLayerType == tibia::ObjectLayerTypes::objects)
                {
                    tibia::Object::Ptr object = std::make_shared<tibia::Object>(sf::Vector2u(docMapObjectTileX, docMapObjectTileY), docMapObjectZ, docMapObjectId);

                    object->setType(objectType);

                    auto docMapObjectProperties = docMapObject.child("properties");

                    if (docMapObjectProperties != NULL)
                    {
                        for (auto docMapObjectProperty : docMapObjectProperties.children("property"))
                        {
                            std::string docMapObjectPropertyName = docMapObjectProperty.attribute("name").value();

                            if (docMapObjectPropertyName == "count")
                            {
                                objectCount = docMapObjectProperty.attribute("value").as_int();
                            }

                            if (docMapObjectPropertyName == "script_on_interact")
                            {
                                object->properties.onInteractScriptFilename = docMapObjectProperty.attribute("value").value();
                            }

                            if (objectType == tibia::ObjectTypes::sign)
                            {
                                if (docMapObjectPropertyName == "name")
                                {
                                    std::string objectSignName = docMapObjectProperty.attribute("value").value();

                                    object->properties.signName = objectSignName;
                                }
                                if (docMapObjectPropertyName == "text")
                                {
                                    std::string objectSignText = docMapObjectProperty.attribute("value").value();

                                    boost::replace_all(objectSignText, "\\n", "\n");

                                    object->properties.signText = objectSignText;
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::book)
                            {
                                if (docMapObjectPropertyName == "name")
                                {
                                    std::string objectBookName = docMapObjectProperty.attribute("value").value();

                                    object->properties.bookName = objectBookName;
                                }
                                if (docMapObjectPropertyName == "text")
                                {
                                    std::string objectBookText = docMapObjectProperty.attribute("value").value();

                                    boost::replace_all(objectBookText, "\\n", "\n");

                                    object->properties.bookText = objectBookText;
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::lever)
                            {
                                if (docMapObjectPropertyName == "script_on")
                                {
                                    std::string objectLeverScriptOn = docMapObjectProperty.attribute("value").value();

                                    object->properties.leverOnScriptFilename = objectLeverScriptOn;
                                }
                                if (docMapObjectPropertyName == "script_off")
                                {
                                    std::string objectLeverScriptOff = docMapObjectProperty.attribute("value").value();

                                    object->properties.leverOffScriptFilename = objectLeverScriptOff;
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::door)
                            {
                                if (docMapObjectPropertyName == "key")
                                {
                                    std::string objectDoorKey = docMapObjectProperty.attribute("value").value();

                                    object->properties.doorKey = tibia::UMaps::keyTypes[objectDoorKey];
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::teleporter)
                            {
                                if (docMapObjectPropertyName == "x")
                                {
                                    object->properties.teleporterX = docMapObjectProperty.attribute("value").as_int();
                                }
                                else if (docMapObjectPropertyName == "y")
                                {
                                    object->properties.teleporterY = docMapObjectProperty.attribute("value").as_int();
                                }
                                else if (docMapObjectPropertyName == "z")
                                {
                                    object->properties.teleporterZ = docMapObjectProperty.attribute("value").as_int();
                                }
                            }
                        }
                    }

                    tile->addObject(object);

                    if (objectCount > 0)
                    {
                        for (unsigned int i = 1; i < objectCount; i++)
                        {
                            tibia::Object::Ptr copyObject = std::make_shared<tibia::Object>(*object);

                            tile->addObject(copyObject);
                        }
                    }
                }
                else if (docMapObjectLayerType == tibia::ObjectLayerTypes::creatures)
                {
                    tibia::Creature::Ptr creature = std::make_shared<tibia::Creature>(docMapObjectTileX, docMapObjectTileY, docMapObjectZ);

                    creature->setOutfitRandom();

                    auto docMapObjectProperties = docMapObject.child("properties");

                    if (docMapObjectProperties != NULL)
                    {
                        for (auto docMapObjectProperty : docMapObjectProperties.children("property"))
                        {
                            std::string docMapObjectPropertyName = docMapObjectProperty.attribute("name").value();

                            if (docMapObjectPropertyName == "name")
                            {
                                std::string creatureName = docMapObjectProperty.attribute("value").value();;

                                creature->setName(creatureName);

                                creature->setHasCustomName(true);
                            }
                            else if (docMapObjectPropertyName == "type")
                            {
                                std::string creatureTypeString = docMapObjectProperty.attribute("value").value();;

                                int creatureType = tibia::UMaps::creatureTypes[creatureTypeString];

                                creature->setType(creatureType);
                            }
                            else if (docMapObjectPropertyName == "team")
                            {
                                std::string creatureTeamString = docMapObjectProperty.attribute("value").value();;

                                int creatureTeam = tibia::UMaps::teams[creatureTeamString];

                                creature->setTeam(creatureTeam);
                            }
                        }
                    }

                    tile->addCreature(creature);
                }
                else if (docMapObjectLayerType == tibia::ObjectLayerTypes::entities)
                {
                    tibia::Object::Ptr object = std::make_shared<tibia::Object>(sf::Vector2u(docMapObjectTileX, docMapObjectTileY), docMapObjectZ, docMapObjectId);

                    object->setType(objectType);

                    auto docMapObjectProperties = docMapObject.child("properties");

                    if (docMapObjectProperties != NULL)
                    {
                        for (auto docMapObjectProperty : docMapObjectProperties.children("property"))
                        {
                            std::string docMapObjectPropertyName = docMapObjectProperty.attribute("name").value();

                            if (objectType == tibia::ObjectTypes::doScript)
                            {
                                if (docMapObjectPropertyName == "script")
                                {
                                    object->properties.doScriptFilename = docMapObjectProperty.attribute("value").value();
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::stepTile)
                            {
                                if (docMapObjectPropertyName == "script_on_start_touch")
                                {
                                    object->properties.stepTileOnStartTouchScriptFilename = docMapObjectProperty.attribute("value").value();
                                }
                                else if (docMapObjectPropertyName == "script_on_stop_touch")
                                {
                                    object->properties.stepTileOnStopTouchScriptFilename = docMapObjectProperty.attribute("value").value();
                                }
                            }
                        }
                    }

                    tile->addEntity(object);
                }
            }
        }

        return true;
    }

    template <class T>
    int getTileNumberByTileCoords(T tileCoords)
    {
        int x = tileCoords.x / tibia::TILE_SIZE;
        int y = tileCoords.y / tibia::TILE_SIZE;

        return x + (y * m_width);
    }

    bool isTileNumberOutOfBounds(int tileNumber)
    {
        if (tileNumber < 0 || tileNumber > m_size - 1)
        {
            return true;
        }

        return false;
    }

    bool isTilePositionOutOfBounds(const sf::Vector2u& tilePosition)
    {
        if
        (
            tilePosition.x < 0 ||
            tilePosition.y < 0 ||
            tilePosition.x > m_tileWidth  - tibia::TILE_SIZE ||
            tilePosition.y > m_tileHeight - tibia::TILE_SIZE
        )
        {
            return true;
        }

        return false;
    }

    bool isCompressed()
    {
        return m_isCompressed;
    }

    std::string getFilename()
    {
        return m_filename;
    }

    unsigned int getWidth()
    {
        return m_width;
    }

    unsigned int getHeight()
    {
        return m_height;
    }

    unsigned int getTileWidth()
    {
        return m_tileWidth;
    }

    unsigned int getTileHeight()
    {
        return m_tileHeight;
    }

    unsigned int getSize()
    {
        return m_size;
    }

private:

    bool m_isCompressed;

    std::string m_filename;

    unsigned int m_width;
    unsigned int m_height;

    unsigned int m_tileWidth;
    unsigned int m_tileHeight;

    unsigned int m_size;

}; // class Map

} // namespace tibia

#endif // TIBIA_MAP_HPP

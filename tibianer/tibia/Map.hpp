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

#include "tinyxml2.h"

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
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());

        if (doc.ErrorID() != tinyxml2::XML_NO_ERROR)
        {
            std::cout << "Map Document LoadFile Error: " << filename << std::endl;
            return false;
        }

        m_isCompressed = false;

        m_filename = filename;

        tinyxml2::XMLElement* docMap = doc.FirstChildElement();

        m_width  = docMap->UnsignedAttribute("width");
        m_height = docMap->UnsignedAttribute("height");

        m_tileWidth  = m_width * tibia::TILE_SIZE;
        m_tileHeight = m_width * tibia::TILE_SIZE;

        m_size = m_width * m_height;

        tibia::mapWidth  = m_width;
        tibia::mapHeight = m_height;

        tibia::mapSize = m_size;

        std::cout << "Map size: " << m_width << "x" << m_height << " (" << m_size << " tiles)" << std::endl;

        tinyxml2::XMLElement* docMapProperties = docMap->FirstChildElement("properties");

        if (docMapProperties != NULL)
        {
            for (tinyxml2::XMLElement* docMapProperty = docMapProperties->FirstChildElement("property"); docMapProperty != NULL; docMapProperty = docMapProperty->NextSiblingElement("property"))
            {
                std::string docMapPropertyName = docMapProperty->Attribute("name");

                if (docMapPropertyName == "name")
                {
                    properties.name = docMapProperty->Attribute("value");
                }
                else if (docMapPropertyName == "description")
                {
                    properties.description = docMapProperty->Attribute("value");
                }
                else if (docMapPropertyName == "author")
                {
                    properties.author = docMapProperty->Attribute("value");
                }

                else if (docMapPropertyName == "player_start_x")
                {
                    properties.playerStartX = docMapProperty->IntAttribute("value");
                }
                else if (docMapPropertyName == "player_start_y")
                {
                    properties.playerStartY = docMapProperty->IntAttribute("value");
                }
                else if (docMapPropertyName == "player_start_z")
                {
                    properties.playerStartZ = docMapProperty->IntAttribute("value");
                }

                else if (docMapPropertyName == "time_of_day")
                {
                    properties.timeOfDay = tibia::UMaps::timeOfDay[docMapProperty->Attribute("value")];
                }
            }
        }

        for (tinyxml2::XMLElement* docMapLayer = docMap->FirstChildElement("layer"); docMapLayer != NULL; docMapLayer = docMapLayer->NextSiblingElement("layer"))
        {
            std::string docMapLayerName = docMapLayer->Attribute("name");

            //std::cout << "Layer Name: " << docMapLayerName << std::endl;

            tinyxml2::XMLElement* docMapLayerData = docMapLayer->FirstChildElement("data");

            std::string docMapLayerDataString = docMapLayerData->GetText();

            docMapLayerDataString.erase(boost::remove_if(docMapLayerDataString, boost::is_any_of(" \r\n")), docMapLayerDataString.end());

            // skip empty layers, huge performance boost
            if (docMapLayerDataString == "eJztwTEBAAAAwqD1T20IX6AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD4DAPAAAQ==")
            {
                continue;
            }

            //std::cout << "Decoding base64 map layer: " << docMapLayerName << std::endl;

            docMapLayerDataString = base64_decode(docMapLayerDataString);

            if (docMapLayerData->Attribute("compression") != NULL)
            {
                m_isCompressed = true;

                //std::cout << "Decompressing zlib map layer: " << docMapLayerName << std::endl;

                docMapLayerDataString = boost_zlib_decompress_string_fast(docMapLayerDataString);
            }

            //std::cout << docMapLayerDataString << std::endl;

            std::istringstream docMapLayerDataStream(docMapLayerDataString);
            //docMapLayerDataStream.seekg(0, std::ios::beg);

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

            tinyxml2::XMLElement* docMapLayerProperties = docMapLayer->FirstChildElement("properties");

            if (docMapLayerProperties != NULL)
            {
                for (tinyxml2::XMLElement* docMapLayerProperty = docMapLayerProperties->FirstChildElement("property"); docMapLayerProperty != NULL; docMapLayerProperty = docMapLayerProperty->NextSiblingElement("property"))
                {
                    std::string docMapLayerPropertyName = docMapLayerProperty->Attribute("name");

                    int docMapLayerPropertyValue = docMapLayerProperty->IntAttribute("value");

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

        for (tinyxml2::XMLElement* docMapObjectGroup = docMap->FirstChildElement("objectgroup"); docMapObjectGroup != NULL; docMapObjectGroup = docMapObjectGroup->NextSiblingElement("objectgroup"))
        {
            std::string docMapObjectGroupName = docMapObjectGroup->Attribute("name");

            //std::cout << docMapObjectGroupName << std::endl;

            int docMapObjectZ = tibia::ZAxis::default;

            int docMapObjectLayerType = tibia::ObjectLayerTypes::objects;

            tinyxml2::XMLElement* docMapObjectGroupProperties = docMapObjectGroup->FirstChildElement("properties");

            if (docMapObjectGroupProperties != NULL)
            {
                for (tinyxml2::XMLElement* docMapObjectGroupProperty = docMapObjectGroupProperties->FirstChildElement("property"); docMapObjectGroupProperty != NULL; docMapObjectGroupProperty = docMapObjectGroupProperty->NextSiblingElement("property"))
                {
                    std::string docMapObjectGroupPropertyName = docMapObjectGroupProperty->Attribute("name");

                    int docMapObjectGroupPropertyValue = docMapObjectGroupProperty->IntAttribute("value");

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
            }

            tibia::TileMap* tileMap = &tileMapTiles[docMapObjectZ];

            for (tinyxml2::XMLElement* docMapObject = docMapObjectGroup->FirstChildElement("object"); docMapObject != NULL; docMapObject = docMapObject->NextSiblingElement("object"))
            {
                int docMapObjectId = docMapObject->IntAttribute("gid");

                //std::cout << docMapObjectId << std::endl;

                int docMapObjectTileX = docMapObject->IntAttribute("x");
                int docMapObjectTileY = docMapObject->IntAttribute("y") - tibia::TILE_SIZE; // y-axis bug for objects in Tiled editor?

                tibia::Tile::List* tileList = tileMap->getTileList();

                int tileNumber = getTileNumberByTileCoords(sf::Vector2u(docMapObjectTileX, docMapObjectTileY));

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

                std::string docMapObjectType = "null";

                if (docMapObject->Attribute("type") != NULL)
                {
                    docMapObjectType = docMapObject->Attribute("type");
                }

                int objectType = tibia::UMaps::objectTypes[docMapObjectType];

                int objectCount = 0;

                if (docMapObjectLayerType == tibia::ObjectLayerTypes::objects)
                {
                    tibia::Object::Ptr object = std::make_shared<tibia::Object>(sf::Vector2u(docMapObjectTileX, docMapObjectTileY), docMapObjectZ, docMapObjectId);

                    object->setType(objectType);

                    tinyxml2::XMLElement* docMapObjectProperties = docMapObject->FirstChildElement("properties");

                    if (docMapObjectProperties != NULL)
                    {
                        for (tinyxml2::XMLElement* docMapObjectProperty = docMapObjectProperties->FirstChildElement("property"); docMapObjectProperty != NULL; docMapObjectProperty = docMapObjectProperty->NextSiblingElement("property"))
                        {
                            std::string docMapObjectPropertyName = docMapObjectProperty->Attribute("name");

                            if (docMapObjectPropertyName == "count")
                            {
                                objectCount = docMapObjectProperty->IntAttribute("value");
                            }

                            if (objectType == tibia::ObjectTypes::sign)
                            {
                                if (docMapObjectPropertyName == "name")
                                {
                                    std::string objectSignName = docMapObjectProperty->Attribute("value");

                                    object->properties.signName = objectSignName;
                                }
                                if (docMapObjectPropertyName == "text")
                                {
                                    std::string objectSignText = docMapObjectProperty->Attribute("value");

                                    boost::replace_all(objectSignText, "\\n", "\n");

                                    object->properties.signText = objectSignText;
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::book)
                            {
                                if (docMapObjectPropertyName == "name")
                                {
                                    std::string objectBookName = docMapObjectProperty->Attribute("value");

                                    object->properties.bookName = objectBookName;
                                }
                                if (docMapObjectPropertyName == "text")
                                {
                                    std::string objectBookText = docMapObjectProperty->Attribute("value");

                                    boost::replace_all(objectBookText, "\\n", "\n");

                                    object->properties.bookText = objectBookText;
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::door)
                            {
                                if (docMapObjectPropertyName == "key")
                                {
                                    std::string objectDoorKey = docMapObjectProperty->Attribute("value");

                                    object->properties.doorKey = tibia::UMaps::keyTypes[objectDoorKey];
                                }
                            }
                            else if (objectType == tibia::ObjectTypes::teleporter)
                            {
                                if (docMapObjectPropertyName == "x")
                                {
                                    object->properties.teleporterX = docMapObjectProperty->IntAttribute("value");
                                }
                                else if (docMapObjectPropertyName == "y")
                                {
                                    object->properties.teleporterY = docMapObjectProperty->IntAttribute("value");
                                }
                                else if (docMapObjectPropertyName == "z")
                                {
                                    object->properties.teleporterZ = docMapObjectProperty->IntAttribute("value");
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

                    tinyxml2::XMLElement* docMapObjectProperties = docMapObject->FirstChildElement("properties");

                    if (docMapObjectProperties != NULL)
                    {
                        for (tinyxml2::XMLElement* docMapObjectProperty = docMapObjectProperties->FirstChildElement("property"); docMapObjectProperty != NULL; docMapObjectProperty = docMapObjectProperty->NextSiblingElement("property"))
                        {
                            std::string docMapObjectPropertyName = docMapObjectProperty->Attribute("name");

                            if (docMapObjectPropertyName == "name")
                            {
                                std::string creatureName = docMapObjectProperty->Attribute("value");

                                creature->setName(creatureName);

                                creature->setHasCustomName(true);
                            }
                            else if (docMapObjectPropertyName == "type")
                            {
                                std::string creatureTypeString = docMapObjectProperty->Attribute("value");

                                int creatureType = tibia::UMaps::creatureTypes[creatureTypeString];

                                creature->setType(creatureType);
                            }
                            else if (docMapObjectPropertyName == "team")
                            {
                                std::string creatureTeamString = docMapObjectProperty->Attribute("value");

                                int creatureTeam = tibia::UMaps::teams[creatureTeamString];

                                creature->setTeam(creatureTeam);
                            }
                        }
                    }

                    tile->addCreature(creature);
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

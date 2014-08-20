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
        std::string name        = "Unknown Map";
        std::string description = "";
        std::string author      = "Unknown Author";

        unsigned int playerStartX = 0;
        unsigned int playerStartY = 0;
        unsigned int playerStartZ = tibia::ZAxis::ground;

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

        m_filename = filename;

        tinyxml2::XMLElement* docMap = doc.FirstChildElement();

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
                    properties.timeOfDay = tibia::umapTimeOfDay[docMapProperty->Attribute("value")];
                }
            }
        }

        for (tinyxml2::XMLElement* docMapLayer = docMap->FirstChildElement("layer"); docMapLayer != NULL; docMapLayer = docMapLayer->NextSiblingElement("layer"))
        {
            std::string docMapLayerName = docMapLayer->Attribute("name");

            //std::cout << "Layer Name: " << docMapLayerName << std::endl;

            std::string docMapLayerData = docMapLayer->FirstChildElement("data")->GetText();

            docMapLayerData.erase(boost::remove_if(docMapLayerData, boost::is_any_of(" \r\n")), docMapLayerData.end());

            docMapLayerData = base64_decode(docMapLayerData);
            docMapLayerData = boost_zlib_decompress_string_fast(docMapLayerData);

            //std::cout << docMapLayerData << std::endl;

            std::istringstream docMapLayerDataStream(docMapLayerData);
            //docMapLayerDataStream.seekg(0, std::ios::beg);

            std::vector<int> docMapLayerDataTiles;
            docMapLayerDataTiles.reserve(docMapLayerData.size() / 4);

            for (unsigned int i = 0; i < docMapLayerData.size(); i += 4)
            {
                int tileId;
                docMapLayerDataStream.read(reinterpret_cast<char*>(&tileId), 4);

                docMapLayerDataTiles.push_back(tileId);
            }

            int tileMapZ = tibia::ZAxis::ground;

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
                tileMapTiles[tileMapZ].load(docMapLayerDataTiles, docMapLayerName, tileMapType, tileMapZ);
            }
            else if (tileMapType == tibia::TileMapTypes::tileEdges)
            {
                tileMapTileEdges[tileMapZ].load(docMapLayerDataTiles, docMapLayerName, tileMapType, tileMapZ);
            }
        }

        for (tinyxml2::XMLElement* docMapObjectGroup = docMap->FirstChildElement("objectgroup"); docMapObjectGroup != NULL; docMapObjectGroup = docMapObjectGroup->NextSiblingElement("objectgroup"))
        {
            std::string docMapObjectGroupName = docMapObjectGroup->Attribute("name");

            //std::cout << docMapObjectGroupName << std::endl;

            int docMapObjectZ = tibia::ZAxis::ground;

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

                int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(docMapObjectTileX, docMapObjectTileY));

                tibia::Tile::Ptr tile = tileList->at(tileNumber);

                std::string docMapObjectType = "null";

                if (docMapObject->Attribute("type") != NULL)
                {
                    docMapObjectType = docMapObject->Attribute("type");
                }

                int objectType = tibia::umapObjectTypes[docMapObjectType];

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
                        for (int i = 1; i < objectCount; i++)
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

                                int creatureType = tibia::umapCreatureTypes[creatureTypeString];

                                creature->setType(creatureType);
                            }
                            else if (docMapObjectPropertyName == "team")
                            {
                                std::string creatureTeamString = docMapObjectProperty->Attribute("value");

                                int creatureTeam = tibia::umapTeams[creatureTeamString];

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

    std::string getFilename()
    {
        return m_filename;
    }

private:

    std::string m_filename;

}; // class Map

} // namespace tibia

#endif // TIBIA_MAP_HPP

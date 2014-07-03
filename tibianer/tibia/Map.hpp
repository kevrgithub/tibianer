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

namespace tibia
{

class Map
{

public:

    tibia::TileMap tileMapTiles[5];
    tibia::TileMap tileMapTileEdges[5];

    bool load(const std::string& filename)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());

        tinyxml2::XMLElement* docMap = doc.FirstChildElement();

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

            std::size_t findNameWithObjects = docMapObjectGroupName.find("Objects");

            if (findNameWithObjects == std::string::npos)
            {
                continue;
            }

            int docMapObjectZ = tibia::ZAxis::ground;

            tinyxml2::XMLElement* docMapObjectGroupProperties = docMapObjectGroup->FirstChildElement("properties");

            for (tinyxml2::XMLElement* docMapObjectGroupProperty = docMapObjectGroupProperties->FirstChildElement("property"); docMapObjectGroupProperty != NULL; docMapObjectGroupProperty = docMapObjectGroupProperty->NextSiblingElement("property"))
            {
                std::string docMapObjectGroupPropertyName = docMapObjectGroupProperty->Attribute("name");

                int docMapObjectGroupPropertyValue = docMapObjectGroupProperty->IntAttribute("value");

                //std::cout << "Object Group Property: " << docMapObjectGroupPropertyName << "=" << docMapObjectGroupPropertyValue << std::endl;

                if (docMapObjectGroupPropertyName == "z")
                {
                    docMapObjectZ = docMapObjectGroupPropertyValue;
                }
            }

            tibia::TileMap* tileMap = &tileMapTiles[docMapObjectZ];

            for (tinyxml2::XMLElement* docMapObject = docMapObjectGroup->FirstChildElement("object"); docMapObject != NULL; docMapObject = docMapObject->NextSiblingElement("object"))
            {
                int docMapObjectId = docMapObject->IntAttribute("gid");

                //std::cout << docMapObjectId << std::endl;

                int docMapObjectTileX = docMapObject->IntAttribute("x");
                int docMapObjectTileY = docMapObject->IntAttribute("y") - tibia::TILE_SIZE; // y-axis bug for objects in Tiled editor?

                tibia::ObjectPtr object = std::make_shared<tibia::Object>(docMapObjectTileX, docMapObjectTileY, docMapObjectZ, docMapObjectId);

                tibia::TileList* tileList = tileMap->getTileList();

                int tileNumber = tibia::Utility::getTileNumberByTileCoords(sf::Vector2u(docMapObjectTileX, docMapObjectTileY));

                tibia::TilePtr tile = tileList->at(tileNumber);

                tile->addObject(object);

                //std::cout << "addObject to tileNumber: " << tileNumber << std::endl;
            }
        }

        return true;
    }

};

} // tibia

#endif // TIBIA_MAP_HPP

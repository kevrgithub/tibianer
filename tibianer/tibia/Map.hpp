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
#include "tibia/Object.hpp"

namespace tibia
{

class Map
{

public:

    typedef std::shared_ptr<tibia::Object> objectSharedPtr;
    typedef std::vector<objectSharedPtr>::iterator objectsListIterator;

    typedef std::vector<tibia::TileMap*>::iterator tileMapListIterator;

    std::vector<tibia::TileMap*> tileMapList;
    std::vector<tibia::TileMap*>::iterator tileMapList_it;

    tibia::TileMap tileMapUnderGroundTiles;
    tibia::TileMap tileMapUnderGroundEdges;
    tibia::TileMap tileMapUnderGroundObjects;

    tibia::TileMap tileMapGroundTiles;
    tibia::TileMap tileMapGroundEdges;
    tibia::TileMap tileMapGroundObjects;

    tibia::TileMap tileMapAboveGroundTiles;
    tibia::TileMap tileMapAboveGroundEdges;
    tibia::TileMap tileMapAboveGroundObjects;

    bool load(std::string filename)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());

        tinyxml2::XMLElement* docMap = doc.FirstChildElement();

        for (tinyxml2::XMLElement* docMapLayer = docMap->FirstChildElement("layer"); docMapLayer != NULL; docMapLayer = docMapLayer->NextSiblingElement("layer"))
        {
            std::string docMapLayerName = docMapLayer->Attribute("name");

            //std::cout << docMapLayerName << std::endl;

            std::string docMapLayerData = docMapLayer->FirstChildElement("data")->GetText();

            docMapLayerData.erase(boost::remove_if(docMapLayerData, boost::is_any_of(" \r\n")), docMapLayerData.end());

            docMapLayerData = base64_decode(docMapLayerData);

            docMapLayerData = boost_zlib_decompress_string_fast(docMapLayerData);

            //std::cout << docMapLayerData << std::endl;

            std::istringstream docMapLayerDataStream(docMapLayerData);
            docMapLayerDataStream.seekg(0, std::ios::beg);

            std::vector<int> docMapLayerDataTiles;

            for (unsigned int i = 0; i < docMapLayerData.size(); i += 4)
            {
                int tileId;
                docMapLayerDataStream.read(reinterpret_cast<char*>(&tileId), 4);

                docMapLayerDataTiles.push_back(tileId);
            }

            if (docMapLayerName == "underground tiles")
            {
                tileMapUnderGroundTiles.load(docMapLayerDataTiles, tibia::TileMapTypes::tiles);
            }
            else if (docMapLayerName == "underground tile edges")
            {
                tileMapUnderGroundEdges.load(docMapLayerDataTiles, tibia::TileMapTypes::edges);
            }
            else if (docMapLayerName == "underground tile objects")
            {
                tileMapUnderGroundObjects.load(docMapLayerDataTiles, tibia::TileMapTypes::objects);
            }

            else if (docMapLayerName == "ground tiles")
            {
                tileMapGroundTiles.load(docMapLayerDataTiles, tibia::TileMapTypes::tiles);
            }
            else if (docMapLayerName == "ground tile edges")
            {
                tileMapGroundEdges.load(docMapLayerDataTiles, tibia::TileMapTypes::edges);
            }
            else if (docMapLayerName == "ground tile objects")
            {
                tileMapGroundObjects.load(docMapLayerDataTiles, tibia::TileMapTypes::objects);
            }

            else if (docMapLayerName == "aboveground tiles")
            {
                tileMapAboveGroundTiles.load(docMapLayerDataTiles, tibia::TileMapTypes::tiles);
            }
            else if (docMapLayerName == "aboveground tile edges")
            {
                tileMapAboveGroundEdges.load(docMapLayerDataTiles, tibia::TileMapTypes::edges);
            }
            else if (docMapLayerName == "aboveground tile objects")
            {
                tileMapAboveGroundObjects.load(docMapLayerDataTiles, tibia::TileMapTypes::objects);
            }
        }

        tileMapUnderGroundTiles.setName("underground tiles");
        tileMapUnderGroundEdges.setName("underground tile edges");
        tileMapUnderGroundObjects.setName("underground tile objects");

        tileMapGroundTiles.setName("ground tiles");
        tileMapGroundEdges.setName("ground tile edges");
        tileMapGroundObjects.setName("ground tile objects");

        tileMapAboveGroundTiles.setName("aboveground tiles");
        tileMapAboveGroundEdges.setName("aboveground tile edges");
        tileMapAboveGroundObjects.setName("aboveground tile objects");

        tileMapUnderGroundTiles.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundEdges.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundObjects.setZ(tibia::ZAxis::underGround);

        tileMapGroundTiles.setZ(tibia::ZAxis::ground);
        tileMapGroundEdges.setZ(tibia::ZAxis::ground);
        tileMapGroundObjects.setZ(tibia::ZAxis::ground);

        tileMapAboveGroundTiles.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundEdges.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundObjects.setZ(tibia::ZAxis::aboveGround);

        tileMapList.push_back(&tileMapUnderGroundTiles);
        tileMapList.push_back(&tileMapUnderGroundEdges);
        tileMapList.push_back(&tileMapUnderGroundObjects);

        tileMapList.push_back(&tileMapGroundTiles);
        tileMapList.push_back(&tileMapGroundEdges);
        tileMapList.push_back(&tileMapGroundObjects);

        tileMapList.push_back(&tileMapAboveGroundTiles);
        tileMapList.push_back(&tileMapAboveGroundEdges);
        tileMapList.push_back(&tileMapAboveGroundObjects);

        for (tinyxml2::XMLElement* docMapObjectGroup = docMap->FirstChildElement("objectgroup"); docMapObjectGroup != NULL; docMapObjectGroup = docMapObjectGroup->NextSiblingElement("objectgroup"))
        {
            std::string docMapObjectGroupName = docMapObjectGroup->Attribute("name");

            //std::cout << docMapObjectGroupName << std::endl;

            std::size_t findObjectsClass = docMapObjectGroupName.find("objects");

            if (findObjectsClass == std::string::npos)
            {
                continue;
            }

            int docMapObjectZ = tibia::ZAxis::ground;

            if (docMapObjectGroupName == "ground objects")
            {
                docMapObjectZ = tibia::ZAxis::ground;
            }
            else if (docMapObjectGroupName == "underground objects")
            {
                docMapObjectZ = tibia::ZAxis::underGround;
            }
            else if (docMapObjectGroupName == "aboveground objects")
            {
                docMapObjectZ = tibia::ZAxis::aboveGround;
            }

            for (tinyxml2::XMLElement* docMapObject = docMapObjectGroup->FirstChildElement("object"); docMapObject != NULL; docMapObject = docMapObject->NextSiblingElement("object"))
            {
                int docMapObjectId = docMapObject->IntAttribute("gid");

                //std::cout << docMapObjectId << std::endl;

                int docMapObjectTileX = docMapObject->IntAttribute("x");
                int docMapObjectTileY = docMapObject->IntAttribute("y") - tibia::TILE_SIZE; // y-axis bug for objects in Tiled editor?

                objectSharedPtr object = std::make_shared<tibia::Object>(docMapObjectTileX, docMapObjectTileY, docMapObjectZ, docMapObjectId);
                m_objectsList.push_back(object);
            }
        }

        return true;
    }

    std::vector<objectSharedPtr>* getObjectsList()
    {
        return &m_objectsList;
    }

private:

    std::vector<objectSharedPtr> m_objectsList;

};

}

#endif // TIBIA_MAP_HPP

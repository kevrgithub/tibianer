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

namespace tibia
{

class Map
{

public:

    std::vector<tibia::TileMap*> tileMapList;
    std::vector<tibia::TileMap*>::iterator tileMapList_it;

    std::vector<tibia::TileMap*> tileMapObjectsList;
    std::vector<tibia::TileMap*>::iterator tileMapObjectsList_it;

    tibia::TileMap tileMapUnderGroundTiles;
    tibia::TileMap tileMapUnderGroundEdges;
    tibia::TileMap tileMapUnderGroundWalls;
    tibia::TileMap tileMapUnderGroundWallsEx;
    tibia::TileMap tileMapUnderGroundWalls2;
    tibia::TileMap tileMapUnderGroundObjects;
    tibia::TileMap tileMapUnderGroundObjects2;
    tibia::TileMap tileMapUnderGroundObjectsOffset;
    tibia::TileMap tileMapUnderGroundObjectsDrawLast;
    tibia::TileMap tileMapUnderGroundObjectsDrawLast2;

    tibia::TileMap tileMapGroundTiles;
    tibia::TileMap tileMapGroundEdges;
    tibia::TileMap tileMapGroundWalls;
    tibia::TileMap tileMapGroundWallsEx;
    tibia::TileMap tileMapGroundWalls2;
    tibia::TileMap tileMapGroundObjects;
    tibia::TileMap tileMapGroundObjects2;
    tibia::TileMap tileMapGroundObjectsOffset;
    tibia::TileMap tileMapGroundObjectsDrawLast;
    tibia::TileMap tileMapGroundObjectsDrawLast2;

    tibia::TileMap tileMapAboveGroundTiles;
    tibia::TileMap tileMapAboveGroundEdges;
    tibia::TileMap tileMapAboveGroundWalls;
    tibia::TileMap tileMapAboveGroundWallsEx;
    tibia::TileMap tileMapAboveGroundWalls2;
    tibia::TileMap tileMapAboveGroundObjects;
    tibia::TileMap tileMapAboveGroundObjects2;
    tibia::TileMap tileMapAboveGroundObjectsOffset;
    tibia::TileMap tileMapAboveGroundObjectsDrawLast;
    tibia::TileMap tileMapAboveGroundObjectsDrawLast2;

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

            std::string docMapLayerDataDecoded = base64_decode(docMapLayerData);

            std::string docMapLayerDataDecompressed = boost_zlib_decompress_string(docMapLayerDataDecoded);

            //std::cout << docMapLayerData << std::endl;

            std::istringstream docMapLayerDataStream(docMapLayerDataDecompressed);
            docMapLayerDataStream.seekg(0, std::ios::beg);

            std::vector<int> docMapLayerDataTiles;

            for (unsigned int i = 0; i < docMapLayerDataDecompressed.size(); i += 4)
            {
                int tileId;
                docMapLayerDataStream.read(reinterpret_cast<char*>(&tileId), 4);

                docMapLayerDataTiles.push_back(tileId);
            }

            if (docMapLayerName == "underground tiles")
            {
                tileMapUnderGroundTiles.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground edges")
            {
                tileMapUnderGroundEdges.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground walls")
            {
                tileMapUnderGroundWalls.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground walls ex")
            {
                tileMapUnderGroundWallsEx.load(docMapLayerDataTiles);
            }            
            else if (docMapLayerName == "underground walls 2")
            {
                tileMapUnderGroundWalls2.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground objects")
            {
                tileMapUnderGroundObjects.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground objects 2")
            {
                tileMapUnderGroundObjects2.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground objects offset")
            {
                tileMapUnderGroundObjectsOffset.load(docMapLayerDataTiles, true);
            }
            else if (docMapLayerName == "underground objects draw last")
            {
                tileMapUnderGroundObjectsDrawLast.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "underground objects draw last 2")
            {
                tileMapUnderGroundObjectsDrawLast2.load(docMapLayerDataTiles);
            }

            else if (docMapLayerName == "ground tiles")
            {
                tileMapGroundTiles.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground edges")
            {
                tileMapGroundEdges.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground walls")
            {
                tileMapGroundWalls.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground walls ex")
            {
                tileMapGroundWallsEx.load(docMapLayerDataTiles);
            }            
            else if (docMapLayerName == "ground walls 2")
            {
                tileMapGroundWalls2.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground objects")
            {
                tileMapGroundObjects.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground objects 2")
            {
                tileMapGroundObjects2.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground objects offset")
            {
                tileMapGroundObjectsOffset.load(docMapLayerDataTiles, true);
            }
            else if (docMapLayerName == "ground objects draw last")
            {
                tileMapGroundObjectsDrawLast.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground objects draw last 2")
            {
                tileMapGroundObjectsDrawLast2.load(docMapLayerDataTiles);
            }

            else if (docMapLayerName == "aboveground tiles")
            {
                tileMapAboveGroundTiles.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground edges")
            {
                tileMapAboveGroundEdges.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground walls")
            {
                tileMapAboveGroundWalls.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground walls ex")
            {
                tileMapAboveGroundWallsEx.load(docMapLayerDataTiles);
            }            
            else if (docMapLayerName == "aboveground walls 2")
            {
                tileMapAboveGroundWalls2.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground objects")
            {
                tileMapAboveGroundObjects.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground objects 2")
            {
                tileMapAboveGroundObjects2.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground objects offset")
            {
                tileMapAboveGroundObjectsOffset.load(docMapLayerDataTiles, true);
            }
            else if (docMapLayerName == "aboveground objects draw last")
            {
                tileMapAboveGroundObjectsDrawLast.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "aboveground objects draw last 2")
            {
                tileMapAboveGroundObjectsDrawLast2.load(docMapLayerDataTiles);
            }
        }

        tileMapUnderGroundTiles.setName("underground tiles");
        tileMapUnderGroundEdges.setName("underground edges");
        tileMapUnderGroundWalls.setName("underground walls");
        tileMapUnderGroundWallsEx.setName("underground walls ex");
        tileMapUnderGroundWalls2.setName("underground walls 2");
        tileMapUnderGroundObjects.setName("underground objects");
        tileMapUnderGroundObjects2.setName("underground objects 2");
        tileMapUnderGroundObjectsOffset.setName("underground objects offset");
        tileMapUnderGroundObjectsDrawLast.setName("underground objects draw last");
        tileMapUnderGroundObjectsDrawLast2.setName("underground objects draw last 2");

        tileMapGroundTiles.setName("ground tiles");
        tileMapGroundEdges.setName("ground edges");
        tileMapGroundWalls.setName("ground walls");
        tileMapGroundWallsEx.setName("ground walls ex");
        tileMapGroundWalls2.setName("ground walls 2");
        tileMapGroundObjects.setName("ground objects");
        tileMapGroundObjects2.setName("ground objects 2");
        tileMapGroundObjectsOffset.setName("ground objects offset");
        tileMapGroundObjectsDrawLast.setName("ground objects draw last");
        tileMapGroundObjectsDrawLast2.setName("ground objects draw last 2");

        tileMapAboveGroundTiles.setName("aboveground tiles");
        tileMapAboveGroundEdges.setName("aboveground edges");
        tileMapAboveGroundWalls.setName("aboveground walls");
        tileMapAboveGroundWallsEx.setName("aboveground walls ex");
        tileMapAboveGroundWalls2.setName("aboveground walls 2");
        tileMapAboveGroundObjects.setName("aboveground objects");
        tileMapAboveGroundObjects2.setName("aboveground objects 2");
        tileMapAboveGroundObjectsOffset.setName("aboveground objects offset");
        tileMapAboveGroundObjectsDrawLast.setName("aboveground objects draw last");
        tileMapAboveGroundObjectsDrawLast2.setName("aboveground objects draw last 2");

        tileMapUnderGroundTiles.setType("tiles");
        tileMapUnderGroundEdges.setType("edges");
        tileMapUnderGroundWalls.setType("walls");
        tileMapUnderGroundWallsEx.setType("walls");
        tileMapUnderGroundWalls2.setType("walls");
        tileMapUnderGroundObjects.setType("objects");
        tileMapUnderGroundObjects2.setType("objects");
        tileMapUnderGroundObjectsOffset.setType("objects");
        tileMapUnderGroundObjectsDrawLast.setType("objects draw last");
        tileMapUnderGroundObjectsDrawLast2.setType("objects draw last");

        tileMapGroundTiles.setType("tiles");
        tileMapGroundEdges.setType("edges");
        tileMapGroundWalls.setType("walls");
        tileMapGroundWallsEx.setType("walls");
        tileMapGroundWalls2.setType("walls");
        tileMapGroundObjects.setType("objects");
        tileMapGroundObjects2.setType("objects");
        tileMapGroundObjectsOffset.setType("objects");
        tileMapGroundObjectsDrawLast.setType("objects draw last");
        tileMapGroundObjectsDrawLast2.setType("objects draw last");

        tileMapAboveGroundTiles.setType("tiles");
        tileMapAboveGroundEdges.setType("edges");
        tileMapAboveGroundWalls.setType("walls");
        tileMapAboveGroundWallsEx.setType("walls");
        tileMapAboveGroundWalls2.setType("walls");
        tileMapAboveGroundObjects.setType("objects");
        tileMapAboveGroundObjects2.setType("objects");
        tileMapAboveGroundObjectsOffset.setType("objects");
        tileMapAboveGroundObjectsDrawLast.setType("objects draw last");
        tileMapAboveGroundObjectsDrawLast2.setType("objects draw last");

        tileMapUnderGroundTiles.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundEdges.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundWalls.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundWallsEx.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundWalls2.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundObjects.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundObjects2.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundObjectsOffset.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundObjectsDrawLast.setZ(tibia::ZAxis::underGround);
        tileMapUnderGroundObjectsDrawLast2.setZ(tibia::ZAxis::underGround);

        tileMapGroundTiles.setZ(tibia::ZAxis::ground);
        tileMapGroundEdges.setZ(tibia::ZAxis::ground);
        tileMapGroundWalls.setZ(tibia::ZAxis::ground);
        tileMapGroundWallsEx.setZ(tibia::ZAxis::ground);
        tileMapGroundWalls2.setZ(tibia::ZAxis::ground);
        tileMapGroundObjects.setZ(tibia::ZAxis::ground);
        tileMapGroundObjects2.setZ(tibia::ZAxis::ground);
        tileMapGroundObjectsOffset.setZ(tibia::ZAxis::ground);
        tileMapGroundObjectsDrawLast.setZ(tibia::ZAxis::ground);
        tileMapGroundObjectsDrawLast2.setZ(tibia::ZAxis::ground);

        tileMapAboveGroundTiles.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundEdges.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundWalls.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundWallsEx.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundWalls2.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundObjects.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundObjects2.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundObjectsOffset.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundObjectsDrawLast.setZ(tibia::ZAxis::aboveGround);
        tileMapAboveGroundObjectsDrawLast2.setZ(tibia::ZAxis::aboveGround);

        tileMapList.push_back(&tileMapUnderGroundTiles);
        tileMapList.push_back(&tileMapUnderGroundEdges);
        tileMapList.push_back(&tileMapUnderGroundWalls);
        tileMapList.push_back(&tileMapUnderGroundWallsEx);
        tileMapList.push_back(&tileMapUnderGroundWalls2);
        tileMapList.push_back(&tileMapUnderGroundObjects);
        tileMapList.push_back(&tileMapUnderGroundObjects2);
        tileMapList.push_back(&tileMapUnderGroundObjectsOffset);
        tileMapList.push_back(&tileMapUnderGroundObjectsDrawLast);
        tileMapList.push_back(&tileMapUnderGroundObjectsDrawLast2);

        tileMapList.push_back(&tileMapGroundTiles);
        tileMapList.push_back(&tileMapGroundEdges);
        tileMapList.push_back(&tileMapGroundWalls);
        tileMapList.push_back(&tileMapGroundWallsEx);
        tileMapList.push_back(&tileMapGroundWalls2);
        tileMapList.push_back(&tileMapGroundObjects);
        tileMapList.push_back(&tileMapGroundObjects2);
        tileMapList.push_back(&tileMapGroundObjectsOffset);
        tileMapList.push_back(&tileMapGroundObjectsDrawLast);
        tileMapList.push_back(&tileMapGroundObjectsDrawLast2);

        tileMapList.push_back(&tileMapAboveGroundTiles);
        tileMapList.push_back(&tileMapAboveGroundEdges);
        tileMapList.push_back(&tileMapAboveGroundWalls);
        tileMapList.push_back(&tileMapAboveGroundWallsEx);
        tileMapList.push_back(&tileMapAboveGroundWalls2);
        tileMapList.push_back(&tileMapAboveGroundObjects);
        tileMapList.push_back(&tileMapAboveGroundObjects2);
        tileMapList.push_back(&tileMapAboveGroundObjectsOffset);
        tileMapList.push_back(&tileMapAboveGroundObjectsDrawLast);
        tileMapList.push_back(&tileMapAboveGroundObjectsDrawLast2);

        tileMapObjectsList.push_back(&tileMapUnderGroundObjects);
        tileMapObjectsList.push_back(&tileMapUnderGroundObjects2);
        tileMapObjectsList.push_back(&tileMapUnderGroundObjectsOffset);
        tileMapObjectsList.push_back(&tileMapUnderGroundObjectsDrawLast);
        tileMapObjectsList.push_back(&tileMapUnderGroundObjectsDrawLast2);

        tileMapObjectsList.push_back(&tileMapGroundObjects);
        tileMapObjectsList.push_back(&tileMapGroundObjects2);
        tileMapObjectsList.push_back(&tileMapGroundObjectsOffset);
        tileMapObjectsList.push_back(&tileMapGroundObjectsDrawLast);
        tileMapObjectsList.push_back(&tileMapGroundObjectsDrawLast2);

        tileMapObjectsList.push_back(&tileMapAboveGroundObjects);
        tileMapObjectsList.push_back(&tileMapAboveGroundObjects2);
        tileMapObjectsList.push_back(&tileMapAboveGroundObjectsOffset);
        tileMapObjectsList.push_back(&tileMapAboveGroundObjectsDrawLast);
        tileMapObjectsList.push_back(&tileMapAboveGroundObjectsDrawLast2);

        return true;
    }

    void doAnimatedObjects()
    {
        for (tileMapObjectsList_it = tileMapObjectsList.begin(); tileMapObjectsList_it != tileMapObjectsList.end(); tileMapObjectsList_it++)
        {
            (*tileMapObjectsList_it)->doAnimatedObjects();
        }
    }

};

}

#endif // TIBIA_MAP_HPP

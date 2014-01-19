#ifndef TIBIA_MAP_HPP
#define TIBIA_MAP_HPP

#include <string>
#include <vector>

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

    std::vector<tibia::TileMap*> tileMapTypeTiles;
    std::vector<tibia::TileMap*>::iterator tileMapTypeTiles_it;

    std::vector<tibia::TileMap*> tileMapTypeEdges;
    std::vector<tibia::TileMap*>::iterator tileMapTypeEdges_it;

    std::vector<tibia::TileMap*> tileMapTypeWalls;
    std::vector<tibia::TileMap*>::iterator tileMapTypeWalls_it;

    std::vector<tibia::TileMap*> tileMapTypeObjects;
    std::vector<tibia::TileMap*>::iterator tileMapTypeObjects_it;

    std::vector<tibia::TileMap*> tileMapTypeObjectsDrawLast;
    std::vector<tibia::TileMap*>::iterator tileMapTypeObjectsDrawLast_it;

    std::vector<tibia::TileMap*> tileMapTypeObjectsAll;
    std::vector<tibia::TileMap*>::iterator tileMapTypeObjectsAll_it;

    tibia::TileMap tileMapGroundTiles;
    tibia::TileMap tileMapGroundEdges;
    tibia::TileMap tileMapGroundWalls;
    tibia::TileMap tileMapGroundWallsEx;
    tibia::TileMap tileMapGroundWalls2;
    tibia::TileMap tileMapGroundObjects;
    tibia::TileMap tileMapGroundObjects2;
    tibia::TileMap tileMapGroundObjectsDrawLast;
    tibia::TileMap tileMapGroundObjectsDrawLast2;

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

            if (docMapLayerName == "ground tiles")
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
            else if (docMapLayerName == "ground objects draw last")
            {
                tileMapGroundObjectsDrawLast.load(docMapLayerDataTiles);
            }
            else if (docMapLayerName == "ground objects draw last 2")
            {
                tileMapGroundObjectsDrawLast2.load(docMapLayerDataTiles);
            }
        }

        tileMapTypeTiles.push_back(&tileMapGroundTiles);
        tileMapTypeEdges.push_back(&tileMapGroundEdges);
        tileMapTypeWalls.push_back(&tileMapGroundWalls);
        tileMapTypeWalls.push_back(&tileMapGroundWallsEx);
        tileMapTypeWalls.push_back(&tileMapGroundWalls2);
        tileMapTypeObjects.push_back(&tileMapGroundObjects);
        tileMapTypeObjects.push_back(&tileMapGroundObjects2);
        tileMapTypeObjectsDrawLast.push_back(&tileMapGroundObjectsDrawLast);
        tileMapTypeObjectsDrawLast.push_back(&tileMapGroundObjectsDrawLast2);
        tileMapTypeObjectsAll.push_back(&tileMapGroundObjects);
        tileMapTypeObjectsAll.push_back(&tileMapGroundObjects2);
        tileMapTypeObjectsAll.push_back(&tileMapGroundObjectsDrawLast);
        tileMapTypeObjectsAll.push_back(&tileMapGroundObjectsDrawLast2);

        return true;
    }

    void doAnimatedObjects()
    {
        for (tileMapTypeObjectsAll_it = tileMapTypeObjectsAll.begin(); tileMapTypeObjectsAll_it != tileMapTypeObjectsAll.end(); tileMapTypeObjectsAll_it++)
        {
            //std::cout << "Map doAnimatedObjects()" << std::endl;

            (*tileMapTypeObjectsAll_it)->doAnimatedObjects();
        }
    }
};

}

#endif // TIBIA_MAP_HPP

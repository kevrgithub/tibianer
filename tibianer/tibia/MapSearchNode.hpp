#ifndef TIBIA_MAPSEARCHNODE_HPP
#define TIBIA_MAPSEARCHNODE_HPP

#include "stlastar.h"

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Map.hpp"
#include "tibia/Tile.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Object.hpp"

namespace tibia
{

tibia::TileMap* tileMap_MapSearchNode;

class MapSearchNode
{

public:

    int x;
    int y;

    MapSearchNode::MapSearchNode()
    {
        x = 0;
        y = 0;
    }

    MapSearchNode::MapSearchNode(int px, int py)
    {
        x = px;
        y = py;
    }

    bool IsSameState(MapSearchNode &rhs)
    {
        if (x == rhs.x && y == rhs.y)
        {
            return true;
        }

        return false;
    }

    float GoalDistanceEstimate(MapSearchNode &nodeGoal)
    {
        float xd = float(((float)x - (float)nodeGoal.x));
        float yd = float(((float)y - (float)nodeGoal.y));

        return xd + yd;
    }

    bool IsGoal(MapSearchNode &nodeGoal)
    {
        if (x == nodeGoal.x && y == nodeGoal.y)
        {
            return true;
        }

        return false;
    }

    tibia::Tile::Ptr GetTile(int x, int y)
    {
        if (x < 0 || y < 0)
        {
            return nullptr;
        }

        tibia::Tile::List* tileList = tibia::tileMap_MapSearchNode->getTileList();

        int tileNumber = x + (y * tibia::mapWidth);

        if (tileNumber < 0|| tileNumber > tileList->size())
        {
            return nullptr;
        }

        tibia::Tile::Ptr tile = tileList->at(tileNumber);

        return tile;
    }

    bool GetSuccessors(AStarSearch<MapSearchNode>* aStarSearch, MapSearchNode* parentNode)
    {
        int parentX = -1;
        int parentY = -1;

        if (parentNode)
        {
            parentX = parentNode->x;
            parentY = parentNode->y;
        }

        MapSearchNode newNode;

        tibia::Tile::Ptr tile1 = GetTile(x - 1, y);

        if (tile1 != nullptr)
        {
            tibia::Tile::Properties_t tileProperties1 = tile1->getProperties();

            if
            (
                //(tileProperties1.hasSolidObject == false && tileProperties1.hasSolidCreature == false)
                //&&
                !(
                    (parentX == x - 1) && (parentY == y)
                )
            ) 
            {
                newNode = MapSearchNode(x - 1, y);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        tibia::Tile::Ptr tile2 = GetTile(x, y - 1);

        if (tile2 != nullptr)
        {
            tibia::Tile::Properties_t tileProperties2 = tile2->getProperties();

            if
            (
                //(tileProperties2.hasSolidObject == false && tileProperties2.hasSolidCreature == false)
                //&&
                !(
                    (parentX == x) && (parentY == y - 1)
                )
            ) 
            {
                newNode = MapSearchNode(x, y - 1);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        tibia::Tile::Ptr tile3 = GetTile(x + 1, y);

        if (tile3 != nullptr)
        {
            tibia::Tile::Properties_t tileProperties3 = tile3->getProperties();

            if
            (
                //(tileProperties3.hasSolidObject == false && tileProperties3.hasSolidCreature == false)
                //&&
                !(
                    (parentX == x + 1) && (parentY == y)
                )
            ) 
            {
                newNode = MapSearchNode(x + 1, y);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        tibia::Tile::Ptr tile4 = GetTile(x, y + 1);

        if (tile4 != nullptr)
        {
            tibia::Tile::Properties_t tileProperties4 = tile4->getProperties();

            if
            (
                //(tileProperties4.hasSolidObject == false && tileProperties4.hasSolidCreature == false)
                //&&
                !(
                    (parentX == x) && (parentY == y + 1)
                )
            ) 
            {
                newNode = MapSearchNode(x, y + 1);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        return true;
    }

    float GetCost(MapSearchNode &successor)
    {
        tibia::Tile::Ptr tile = GetTile(x, y);
        tibia::Tile::Properties_t tileProperties = tile->getProperties();

        if (tileProperties.hasSolidObject == true || tileProperties.hasSolidCreature == true)
        {
            return 9.0f;
        }

        if (tileProperties.hasModifyHpOnTouchObject == true || tileProperties.hasTeleporterObject == true)
        {
            return 6.0f;
        }

        if (tileProperties.isMoveBelow == true || tileProperties.hasMoveBelowObject == true)
        {
            return 3.0f;
        }

        return 1.0f;
    }

}; // class MapSearchNode

} // namespace tibia

#endif // TIBIA_MAPSEARCHNODE_HPP

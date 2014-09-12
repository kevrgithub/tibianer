#ifndef TIBIA_MAPSEARCHNODE_HPP
#define TIBIA_MAPSEARCHNODE_HPP

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"
#include "tibia/Map.hpp"
#include "tibia/Tile.hpp"
#include "tibia/TileMap.hpp"
#include "tibia/Creature.hpp"
#include "tibia/Object.hpp"

namespace tibia
{

tibia::TileMap* MapSearchNode_tileMap;

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

    bool IsSameState(MapSearchNode& rhs)
    {
        if ((x == rhs.x) && (y == rhs.y))
        {
            return true;
        }

        return false;
    }

    float GoalDistanceEstimate(MapSearchNode& nodeGoal)
    {
        float xd = float(((float)x - (float)nodeGoal.x));
        float yd = float(((float)y - (float)nodeGoal.y));

        return xd + yd;
    }

    bool IsGoal(MapSearchNode& nodeGoal)
    {
        if ((x == nodeGoal.x) && (y == nodeGoal.y))
        {
            return true;
        }

        return false;
    }

    tibia::Tile::Ptr GetTile(int px, int py)
    {
        if (px < 0 || py < 0 || px > tibia::mapSize - 1 || py > tibia::mapSize - 1)
        {
            //std::cout << "px,py: " << px << "," << py << std::endl;

            return nullptr;
        }

        tibia::Tile::List* tileList = tibia::MapSearchNode_tileMap->getTileList();

        if (tileList->size() == 0)
        {
            return nullptr;
        }

        int tileNumber = px + (py * tibia::mapWidth);

        if (tileNumber < 0 || tileNumber > tibia::mapSize - 1)
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
                (tile1->getId() != tibia::TILE_NULL) &&
                (tileProperties1.isSolid        == false) &&
                (tileProperties1.hasSolidObject == false) &&
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
                (tile2->getId() != tibia::TILE_NULL) &&
                (tileProperties2.isSolid        == false) &&
                (tileProperties2.hasSolidObject == false) &&
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
                (tile3->getId() != tibia::TILE_NULL) &&
                (tileProperties3.isSolid        == false) &&
                (tileProperties3.hasSolidObject == false) &&
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
                (tile4->getId() != tibia::TILE_NULL) &&
                (tileProperties4.isSolid        == false) &&
                (tileProperties4.hasSolidObject == false) &&
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

    float GetCost(MapSearchNode& successor)
    {
        tibia::Tile::Ptr tile = GetTile(x, y);

        if (tile == nullptr)
        {
            return 10.0f;
        }

        tibia::Tile::Properties_t tileProperties = tile->getProperties();

        if
        (
            tile->getId() == tibia::TILE_NULL ||

            tileProperties.isSolid          == true ||
            tileProperties.hasSolidObject   == true ||
            tileProperties.hasSolidCreature == true
        )
        {
            return 8.0f;
        }

        if (tileProperties.hasModifyHpOnTouchObject == true || tileProperties.hasTeleporterObject == true)
        {
            return 6.0f;
        }

        if (tile->getHeight() > 1)
        {
            return 6.0f;
        }

        if (tileProperties.isMoveBelow == true || tileProperties.hasMoveBelowObject == true)
        {
            return 4.0f;
        }

        return 1.0f;
    }

}; // class MapSearchNode

} // namespace tibia

#endif // TIBIA_MAPSEARCHNODE_HPP

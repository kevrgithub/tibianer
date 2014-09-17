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

tibia::Tile::List MapSearchNode_tileList;

class MapSearchNode
{

public:

    int x;
    int y;

    bool isDiagonal = false;

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

        if (tibia::MapSearchNode_tileList.size() == 0)
        {
            return nullptr;
        }

        //int tileNumber = px + (py * tibia::mapWidth);

        //if (tileNumber < 0 || tileNumber > tibia::mapSize - 1)
        //{
            //return nullptr;
        //}

        //tibia::Tile::Ptr tile = tibia::MapSearchNode_tileList.at(tileNumber);

        tibia::Tile::Ptr tile = nullptr;

        for (auto& findTile : tibia::MapSearchNode_tileList)
        {
            if
            (
                (findTile->getPosition().x / tibia::TILE_SIZE) == px &&
                (findTile->getPosition().y / tibia::TILE_SIZE) == py
            )
            {
                tile = findTile;
                break;
            }
        }

        if (tile != nullptr)
        {
            tibia::Tile::Properties_t tileProperties = tile->getProperties();

            if
            (
                (tile->getId() == tibia::TILE_NULL) ||

                (tileProperties.isSolid        == true) ||
                (tileProperties.hasSolidObject == true)
            )
            {
                return nullptr;
            }
        }

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

        // up
        if (GetTile(x, y - 1) != nullptr)
        {
            if
            (
                !(
                    (parentX == x) && (parentY == y - 1)
                )
            )
            {
                newNode = MapSearchNode(x, y - 1);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // right
        if (GetTile(x + 1, y) != nullptr)
        {
            if
            (
                !(
                    (parentX == x + 1) && (parentY == y)
                )
            )
            {
                newNode = MapSearchNode(x + 1, y);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // down
        if (GetTile(x, y + 1) != nullptr)
        {
            if
            (
                !(
                    (parentX == x) && (parentY == y + 1)
                )
            )
            {
                newNode = MapSearchNode(x, y + 1);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // left
        if (GetTile(x - 1, y) != nullptr)
        {
            if
            (
                !(
                    (parentX == x - 1) && (parentY == y)
                )
            )
            {
                newNode = MapSearchNode(x - 1, y);
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // up left
        if (GetTile(x - 1, y - 1) != nullptr)
        {
            if
            (
                !(
                    (parentX == x - 1) && (parentY == y - 1)
                )
            )
            {
                newNode = MapSearchNode(x - 1, y - 1);
                newNode.isDiagonal = true;
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // up right
        if (GetTile(x + 1, y - 1) != nullptr)
        {
            if
            (
                !(
                    (parentX == x + 1) && (parentY == y - 1)
                )
            )
            {
                newNode = MapSearchNode(x + 1, y - 1);
                newNode.isDiagonal = true;
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // down right
        if (GetTile(x + 1, y + 1) != nullptr)
        {
            if
            (
                !(
                    (parentX == x + 1) && (parentY == y + 1)
                )
            )
            {
                newNode = MapSearchNode(x + 1, y + 1);
                newNode.isDiagonal = true;
                aStarSearch->AddSuccessor(newNode);
            }
        }

        // down left
        if (GetTile(x - 1, y + 1) != nullptr)
        {
            if
            (
                !(
                    (parentX == x - 1) && (parentY == y + 1)
                )
            )
            {
                newNode = MapSearchNode(x - 1, y + 1);
                newNode.isDiagonal = true;
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

        if (successor.isDiagonal == true)
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

        if ((tile->getHeight() > 1) || tileProperties.hasTeleporterObject == true)
        {
            return 6.0f;
        }

        if (tileProperties.isMoveBelow == true || tileProperties.hasMoveBelowObject == true || tileProperties.hasMoveAboveObject == true)
        {
            return 4.0f;
        }

        if (tileProperties.hasModifyHpOnTouchObject == true || tileProperties.hasBearTrapOpenObject == true)
        {
            return 2.0f;
        }

        return 1.0f;
    }

}; // class MapSearchNode

} // namespace tibia

#endif // TIBIA_MAPSEARCHNODE_HPP

#include <pch.h>
#include <cmath>
#include <limits>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

const float sqrt2 = 1.414213562373f;

bool AStarPather::initialize()
{

    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */
    Callback cb = std::bind(&AStarPather::mapChange, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);
    
    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */
    
    // WRITE YOUR CODE HERE
    if (switchingMap)
    {
        openList.reserve(terrain->get_map_height() * terrain->get_map_width());
        closeList.reserve(terrain->get_map_height() * terrain->get_map_width());
        switchingMap = false;
    }
    // push the start node
    GridPos start = terrain->get_grid_position(request.start),
        end = terrain->get_grid_position(request.goal);
    if (request.newRequest)
    {
        if (start == end)
        {
            return PathResult::COMPLETE;
        }
        openList.clear();
        closeList.clear();
        openList.push_back(NodeInfo(start, start, 0, heuristicsCalculation(request, start, end)));
        terrain->set_color(start, Colors::Blue);

    }
    
    
   
    GridPos neighborNode;

    

    int cheapestNode = -1;
    char neighbors = NULL;
    while (openList.size() != 0)
    {
        cheapestNode = findTheCheapestIndex(openList);
        if (openList[cheapestNode].current == end)
        {
            // TODO: reconstruct the path
            int index = findNodeWithValue(closeList,openList[cheapestNode].parent);
            request.path.push_front(request.goal);
            while (closeList[index].current != start)
            {
                request.path.push_front(terrain->get_world_position(closeList[index].current));
                index = findNodeWithValue(closeList, closeList[index].parent);
            }
            request.path.push_front(request.start);
            terrain->set_color(openList[cheapestNode].current, Colors::Yellow);
            return PathResult::COMPLETE;
        }
        
        //compute all neighbors
        NodeInfo tempNode = openList[cheapestNode];
        neighbors = checkValidNeighbors(openList[cheapestNode].current);

        // pop this node
        openList[cheapestNode] = openList[openList.size() - 1];
        terrain->set_color(tempNode.current, Colors::Yellow);
        openList.pop_back();

        for (int i = 7; i >= 0; --i) 
        {
            char boolean = static_cast<unsigned char>(1 << (7-i)) & neighbors;
            if (static_cast<bool>(boolean))
            {
                neighborNode = tempNode.current;
                float addCost = getNeighbor(i, neighborNode);
                
                // if not wall then add the child
                if (!terrain->is_wall(neighborNode))
                {
                    float Fcost = tempNode.Gcost + addCost + heuristicsCalculation(request, neighborNode, end);
                    int a, b;
                    // if on openlist
                    a = findNodeWithValue(openList, neighborNode);
                    // if on the openlist
                    if (a != -1)
                    {
                        // if new cost is smaller
                        if (Fcost < openList[a].Fcost)
                        {
                            // replace the node on the openlist
                            openList[a].parent = tempNode.current;
                            openList[a].Gcost = tempNode.Gcost + addCost;
                            openList[a].Fcost = Fcost;
                        }
                    }
                    else
                    {
                        b = findNodeWithValue(closeList, neighborNode);
                        // if on close list
                        if (b != -1)
                        {
                            if (Fcost < closeList[b].Fcost)
                            {
                                // remove the node on close list
                                closeList[b] = closeList[closeList.size() - 1];
                                closeList.pop_back();
                                // push the node onto the openlist
                                openList.push_back(NodeInfo(neighborNode, tempNode.current, tempNode.Gcost + addCost, Fcost));
                                terrain->set_color(neighborNode, Colors::Blue);
                            }
                        }
                        // if not on both list
                        else
                        {
                            // push the node onto the openlist
                            openList.push_back(NodeInfo(neighborNode, tempNode.current, tempNode.Gcost + addCost, Fcost));
                            terrain->set_color(neighborNode, Colors::Blue);
                        }
                        
                    }
                }

            }
        }
        terrain->set_color(tempNode.current, Colors::Yellow);
        closeList.push_back(tempNode);
        
        // TODO: if take too long return imporgress
        return PathResult::PROCESSING;
    }

    // Just sample code, safe to delete
    
    return PathResult::IMPOSSIBLE;
}

float AStarPather::heuristicsCalculation(const PathRequest& request, GridPos& currentPos, GridPos & goal)
{
    switch (request.settings.heuristic)
    {
    case Heuristic::CHEBYSHEV:
        return Chebyshev(currentPos, goal) * request.settings.weight;
        break;
    case Heuristic::EUCLIDEAN:
        return Euclidean(currentPos, goal) * request.settings.weight;
        break;
    case Heuristic::OCTILE:
        return Octile(currentPos, goal) * request.settings.weight;
        break;
    case Heuristic::MANHATTAN:
        return Manhattan(currentPos, goal) * request.settings.weight;
        break;
    default:
        return -1;
        break;
    }
}

int AStarPather::findTheCheapestIndex(const std::vector<NodeInfo> & list)
{
    float min = std::numeric_limits<float>::max();
    int index = -1;

    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].Fcost < min)
        {
            min = list[i].Fcost;
            index = i;
        }
    }

    return index;
}

int AStarPather::findNodeWithValue(const std::vector<NodeInfo>& list, const GridPos& target)
{

    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].current == target)
        {
            return i;
        }
    }
    return -1;
}

char AStarPather::checkValidNeighbors(GridPos target)
{
    char result = static_cast<char>(255);

    // if left one is invalid
    --target.col;
    if (!terrain->is_valid_grid_position(target) || terrain->is_wall(target))
    {
        result &= static_cast<char>(62);
    }


    // if right one is invalid
    target.col += 2;
    if (!terrain->is_valid_grid_position(target) || terrain->is_wall(target))
    {
        result &= static_cast<char>(227);
    }

    // if up one is invalid
    --target.col;
    ++target.row;
    if (!terrain->is_valid_grid_position(target) || terrain->is_wall(target))
    {
        result &= static_cast<char>(143);
    }

    // if down one is invalid
    target.row -= 2;
    if (!terrain->is_valid_grid_position(target) || terrain->is_wall(target))
    {
        result &= static_cast<char>(248);
    }

    return result;
}

float AStarPather::getNeighbor(const int& index, GridPos & currentPos)
{
    float cost = 0;
    switch (index)
    {
    // left
    case 0:
        --currentPos.col;
        cost = 1.f;
        break;
    // up left
    case 1:
        --currentPos.col;
        ++currentPos.row;
        cost = sqrt2;
        break;
    // up 
    case 2:
        ++currentPos.row;
        cost = 1.f;
        break;
    // up right
    case 3:
        ++currentPos.col;
        ++currentPos.row;
        cost = sqrt2;
        break;
    // right
    case 4:
        ++currentPos.col;
        cost = 1.f;
        break;
    // down right
    case 5:
        ++currentPos.col;
        --currentPos.row;
        cost = sqrt2;
        break;
    // down
    case 6:
        --currentPos.row;
        cost = 1.f;
        break;
    // down left
    case 7:
        --currentPos.col;
        --currentPos.row;
        cost = sqrt2;
        break;
    }

    return cost;
}



float AStarPather::Euclidean(GridPos& currentPos, GridPos& targetPos)
{
    float xDiff = static_cast<float>(currentPos.row - targetPos.row),
          zDiff = static_cast<float>(currentPos.col - targetPos.col);
    return std::sqrt(xDiff * xDiff + zDiff * zDiff);
}

float AStarPather::Octile(GridPos& currentPos, GridPos& targetPos)
{
    float xDiff = static_cast<float>(std::abs(currentPos.row - targetPos.row)),
          zDiff = static_cast<float>(std::abs(currentPos.col - targetPos.col));
    return std::min(xDiff, zDiff) * sqrt2 + std::max(xDiff, zDiff) - std::min(xDiff, zDiff);
}

float AStarPather::Chebyshev(GridPos& currentPos, GridPos& targetPos)
{
    float xDiff = static_cast<float>(std::abs(currentPos.row - targetPos.row)),
          zDiff = static_cast<float>(std::abs(currentPos.col - targetPos.col));
    return std::max(xDiff, zDiff);
}

float AStarPather::Manhattan(GridPos& currentPos, GridPos& targetPos)
{
    return static_cast<float>(std::abs(currentPos.row - targetPos.row)
         + std::abs(currentPos.col - targetPos.col));
}

void AStarPather::mapChange()
{
    switchingMap = true;
}

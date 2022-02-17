#pragma once
#include "Misc/PathfindingDetails.hpp"
#include <vector>
class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
private:
    struct NodeInfo
    {
        GridPos current;
        GridPos parent;
        float Fcost;
        float Gcost;
        NodeInfo(GridPos & cur, GridPos & prev, float Gcos, float Fcos): current(cur), parent(prev), Gcost(Gcos), Fcost(Fcos){}
    };
    bool switchingMap = true;
    float heuristicsCalculation(const PathRequest& request,GridPos& currentPos,GridPos& goal);
    int findTheCheapestIndex(const std::vector<NodeInfo> & list);
    int findNodeWithValue(const std::vector<NodeInfo>& list,const GridPos & target);
    char checkValidNeighbors(GridPos target);
    float getNeighbor(const int & index, GridPos& currentPos);
    float Euclidean(GridPos& currentPos, GridPos& targetPos);
    float Octile(GridPos& currentPos, GridPos& targetPos);
    float Chebyshev(GridPos& currentPos, GridPos& targetPos);
    float Manhattan(GridPos& currentPos, GridPos& targetPos);
    bool canEliminate(const GridPos& currentPos, const GridPos& targetPos);
    void rybberBanding(PathRequest& request);
    void insertSmoothPoints(PathRequest& request, const std::list<Vec3>::iterator& p1, const std::list<Vec3>::iterator& p2, const std::list<Vec3>::iterator& p3, const std::list<Vec3>::iterator& p4);
    void catmullRomSpline(PathRequest& request);
    void mapChange();
    std::vector<NodeInfo> openList, closeList;
};
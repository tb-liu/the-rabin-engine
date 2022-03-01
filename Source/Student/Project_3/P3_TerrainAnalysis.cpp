#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

const float SmallFloatValue = 0.0001f;
const float ViewAngleCos = -0.08715574f;
const float sqrt2 = 1.414213562373f;

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float Euclidean(int currentRow, int currentCol, int targetRow, int targetCol)
{
    float xDiff = static_cast<float>(currentRow - targetRow),
        zDiff = static_cast<float>(currentCol - targetCol);
    return std::sqrt(xDiff * xDiff + zDiff * zDiff);
}

float distance_to_closest_wall(int row, int col)
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        with cells outside the map bounds treated as walls, and return the smallest
        distance.  Make use of the is_valid_grid_position and is_wall member
        functions in the global terrain to determine if a cell is within map bounds
        and a wall, respectively.
    */

    // WRITE YOUR CODE HERE
    // need to check wall exist 
    int gridHeight = terrain->get_map_height();
    bool isWall = false;
    for (int i = 0; i < gridHeight; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            if (terrain->is_wall(i,j)) 
            {
                isWall = true;
                break;
            }
        }
        if (isWall)
        {
            break;
        }
    }

    // if no wall exist
    if (!isWall)
    {
        return 0;
    }


    bool findResult = false;
    float result =  std::numeric_limits<float>::max();
    int top = row + 1, bottom = row - 1, left = col - 1, right = col + 1;
    while (!findResult)
    {
        // top row
        for (int i = left; i <= right; i++)
        {
            if (terrain->is_valid_grid_position(top, i) && terrain->is_wall(top, i))
            {
                findResult = true;
                float dis = Euclidean(row, col, top, i);
                if (result > dis)
                {
                    result = dis;
                }
            }
        }
        // bottom row
        for (int i = left; i <= right; i++)
        {
            if (terrain->is_valid_grid_position(bottom, i) && terrain->is_wall(bottom, i))
            {
                findResult = true;
                float dis = Euclidean(row, col, bottom, i);
                if (result > dis)
                {
                    result = dis;
                }
            }
        }

        // left col
        for (int i = bottom + 1; i < top; i++)
        {
            if (terrain->is_valid_grid_position(i, left) && terrain->is_wall(i, left))
            {
                findResult = true;
                float dis = Euclidean(row, col, i, left);
                if (result > dis)
                {
                    result = dis;
                }
            }
        }

        // right col
        for (int i = bottom + 1; i < top; i++)
        {
            if (terrain->is_valid_grid_position(i, right) && terrain->is_wall(i, right))
            {
                findResult = true;
                float dis = Euclidean(row, col, i, right);
                if (result > dis)
                {
                    result = dis;
                }
            }
        }

        top += 1;
        bottom -= 1;
        left -= 1;
        right += 1;
    }
    return  result;
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    float gridUnit = 100.f / terrain->get_map_height();
    float halfCell = gridUnit / 2.f;
    int xMin = std::min(col0, col1),
        yMin = std::min(row0, row1),
        xMax = std::max(col0, col1),
        yMax = std::max(row0, row1);


    Vec3 worldStart, worldEnd;
    worldStart = terrain->get_world_position(row0, col0);
    worldEnd = terrain->get_world_position(row1, col1);

    Vec2 start2V(worldStart.x, worldStart.z),
         end2V(worldEnd.x, worldEnd.z);
    // if on the same line
    if (xMin == xMax)
    {
        for (int i = yMin; i < yMax; i++)
        {
            if (terrain->is_wall(i, xMax)) 
            {
                return false;
            }
        }
    }
    // if on the same coloumn
    else if (yMin == yMax)
    {
        for (int i = xMin; i < xMax; i++)
        {
            if (terrain->is_wall(yMax, i))
            {
                return false;
            }
        }
    }
    else
    {
        for (int i = xMin; i <= xMax; ++i)
        {
            for (int j = yMin; j <= yMax; ++j)
            {
                GridPos temp;
                temp.col = i;
                temp.row = j;
                if (terrain->is_wall(temp))
                {
                    // test if the wall intersect with the line
                    Vec3 pos = terrain->get_world_position(temp);

                    Vec2 topLeft(pos.x - halfCell, pos.z + halfCell),
                        topRight(pos.x + halfCell, pos.z + halfCell),
                        bottomLeft(pos.x - halfCell, pos.z - halfCell),
                        bottomRight(pos.x + halfCell, pos.z - halfCell);
                    // if intersect with top edge of this cell 
                    if (line_intersect(start2V, end2V, topLeft - Vec2(SmallFloatValue, 0.f), topRight + Vec2(SmallFloatValue, 0.f)))
                    {
                        return false;
                    }
                    // if intersect with left edge of this cell 
                    else if (line_intersect(start2V, end2V, topLeft + Vec2(0.f, SmallFloatValue), bottomLeft - Vec2(0.f, SmallFloatValue)))
                    {
                        return false;
                    }
                    // if intersect with bottom edge of this cell 
                    else if (line_intersect(start2V, end2V, bottomLeft - Vec2(SmallFloatValue, 0.f), Vec2(SmallFloatValue, 0.f)))
                    {
                        return false;
                    }
                    // if intersect with right edge of this cell 
                    else if (line_intersect(start2V, end2V, topRight + Vec2(0.f, SmallFloatValue), bottomRight - Vec2(0.f, SmallFloatValue)))
                    {
                        return false;
                    }

                }
            }
        }
    }
    
    

    return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */


    // WRITE YOUR CODE HERE
    int gridSize = terrain->get_map_height();

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j) 
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            float dis = distance_to_closest_wall(i, j);
            dis = 1.f / (dis * dis);
            layer.set_value(i, j, dis);
        }
    }
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    int gridSize = terrain->get_map_height();

    // initial all to 0.f
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            
            layer.set_value(i, j, 0.f);

        }
    }
    
    float temp;
    // calculate all visibility 
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            int count = 0;
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            int jValue = j + 1;
            // test if this cell is visible form the other
            for (int targetI = i; targetI < gridSize; ++targetI)
            {
                
                for (int  targetJ = jValue; targetJ < gridSize; ++targetJ)
                {
                    if (terrain->is_wall(targetI, targetJ))
                    {
                        continue;
                    }
                    if (is_clear_path(i,j, targetI, targetJ))
                    {
                        ++count;
                        temp = layer.get_value(targetI, targetJ) + 1.0f;
                        layer.set_value(targetI, targetJ, temp);
                    }
                }
                jValue = 0;
            }
            // calculate the visible value
            temp = layer.get_value(i, j) + static_cast<float>(count);
            temp = std::clamp(temp / 160.f, 0.f, 1.f);
            layer.set_value(i, j, temp);
        }
    }
}
// for two diagonal nodes, if block by two wall
bool canEliminate(const GridPos& first, const GridPos& second)
{
    int xMin = std::min(first.col, second.col),
        yMin = std::min(first.row, second.row),
        xMax = std::max(first.col, second.col),
        yMax = std::max(first.row, second.row);
    for (int i = xMin; i <= xMax; ++i)
    {
        for (int j = yMin; j <= yMax; ++j)
        {
            GridPos temp;
            temp.col = i;
            temp.row = j;
            if (terrain->is_wall(temp))
            {

                return false;
            }
        }
    }

    return true;
}
bool ifNeighborVisible(const int& index, GridPos currentPos,const MapLayer<float>& layer)
{
    float cost = 0;
    GridPos temp = currentPos;
    switch (index)
    {
        // left
    case 0:
        --currentPos.col;
        break;
        // up left
    case 1:
        --currentPos.col;
        ++currentPos.row;
        break;
        // up 
    case 2:
        ++currentPos.row;
        break;
        // up right
    case 3:
        ++currentPos.col;
        ++currentPos.row;
        break;
        // right
    case 4:
        ++currentPos.col;
        break;
        // down right
    case 5:
        ++currentPos.col;
        --currentPos.row;
        break;
        // down
    case 6:
        --currentPos.row;
        break;
        // down left
    case 7:
        --currentPos.col;
        --currentPos.row;
        break;
    }
    // need to check if the neighber is from cornor and been blocked by a wall
    if (terrain->is_valid_grid_position(currentPos) && !terrain->is_wall(currentPos) && layer.get_value(currentPos) == 1.f && canEliminate(currentPos, temp))
    {
        return true;
    }

    return false;
}


void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    int gridSize = terrain->get_map_height();


    // test if this cell is visible form the other
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j) || (i == row && j == col))
            {
                continue;
            }
            if (is_clear_path(row, col, i, j))
            {
                layer.set_value(i, j, 1.f);
            }
            else
            {
                layer.set_value(i, j, 0.f);
            }
        }
    }

    // check the cell close to visible
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            GridPos temp;
            temp.row = i, temp.col = j;
            if (layer.get_value(temp) == 0.f && !terrain->is_wall(temp))
            {
                for (int x = 0; x < 8; ++x)
                {
                    if (ifNeighborVisible(x, temp, layer))
                    {
                        layer.set_value(i, j, 0.5f);
                        break;
                    }
                }
            }
        }
    }
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    Vec3 forwardVec3 = agent->get_forward_vector();
    Vec3 posVec3 = agent->get_position();
    GridPos gridPos = terrain->get_grid_position(posVec3);
    Vec2 forwardVec2(forwardVec3.x, forwardVec3.z);
    forwardVec2.Normalize();
    

    int gridSize = terrain->get_map_height();

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            Vec3 targetVec3 = terrain->get_world_position(i, j);
            Vec2 targetVec2((targetVec3 - posVec3).x, (targetVec3 - posVec3).z);
            targetVec2.Normalize();
            float dotP = forwardVec2.Dot(targetVec2);
            // if within view angle
            if (dotP > ViewAngleCos)
            {
                if (!terrain->is_wall(gridPos) && is_clear_path(i, j, gridPos.row, gridPos.col))
                {
                    layer.set_value(i, j, 1.f);
                }
            }
        }
    }
}


float getNeighborValue(const int& index,const float & decay, GridPos currentPos, const MapLayer<float>& layer)
{
    float cost = 0.f;
    GridPos temp = currentPos;
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

    if (terrain->is_valid_grid_position(currentPos) && !terrain->is_wall(currentPos) && canEliminate(currentPos, temp))
    {
        //return layer.get_value(currentPos);
        return layer.get_value(currentPos) * exp(-1.f * cost * decay);
    }

    return 0.f;
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
    
    

    // WRITE YOUR CODE HERE
    float  tempLayer[40][40] = { {0.f} };
    int gridSize = terrain->get_map_height();

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            float maxValue = -1.f;
            float neighborValue = 0.f;
            GridPos tempGrid;
            tempGrid.row = i, tempGrid.col = j;
            // get the max decayed influence value
            for (int x = 0; x < 8; ++x)
            {
                neighborValue = getNeighborValue(x, decay, tempGrid, layer);
                if (neighborValue > maxValue)
                {
                    maxValue = neighborValue;
                }
            }

            // apply linear interpolation 
            tempLayer[i][j] = lerp(layer.get_value(i, j), maxValue, growth);

            
        }
    }

    // copy temp layer back
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            layer.set_value(i, j, tempLayer[i][j]);
        }
    }
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    // WRITE YOUR CODE HERE
    float  tempLayer[40][40] = { {0.f} };
    int gridSize = terrain->get_map_height();

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            float maxValue = std::numeric_limits<float>::min();
            float Abs, maxAbs;
            float neighborValue = 0.f;
            GridPos tempGrid;
            tempGrid.row = i, tempGrid.col = j;
            // get the max decayed influence value
            for (int x = 0; x < 8; ++x)
            {
                /*float dis = 0;
                float influenceValue = getNeighborValue(x, dis, tempGrid, layer);
                neighborValue = layer.get_value(tempGrid) - influenceValue * exp(decay * -dis);*/
                neighborValue = getNeighborValue(x, decay, tempGrid, layer);
                Abs = abs(neighborValue);
                if (Abs > maxValue)
                {
                    maxValue = Abs;
                    maxAbs = neighborValue;
                }
            }

            // apply linear interpolation 
            tempLayer[i][j] = lerp(layer.get_value(tempGrid), maxAbs, growth);
        }
    }

    // copy temp layer back
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            layer.set_value(i, j, tempLayer[i][j]);
        }
    }

}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE
    int gridSize = terrain->get_map_height();
    float maxValue = std::numeric_limits<float>::min();

    // find the greatest 
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (maxValue < layer.get_value(i, j))
            {
                maxValue = layer.get_value(i, j);
            }
        }
    }
    // normalize 
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j) || layer.get_value(i, j) < 0.f)
            {
                continue;
            }
            float temp = layer.get_value(i, j);
            layer.set_value(i, j, temp / maxValue);
        }
    }
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE
    int gridSize = terrain->get_map_height();
    float maxValue = std::numeric_limits<float>::min();
    float minValue = std::numeric_limits<float>::max();

    // find the greatest 
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (maxValue < layer.get_value(i, j))
            {
                maxValue = layer.get_value(i, j);
            }
            if (minValue > layer.get_value(i, j))
            {
                minValue = layer.get_value(i, j);
            }
        }
    }
    minValue = abs(minValue);
    // normalize 
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            float temp = layer.get_value(i, j);
            if (temp > 0.f)
            {
                layer.set_value(i, j, temp / maxValue);
            }
            else
            {
                layer.set_value(i, j, temp / minValue);
            }
            
        }
    }
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    // WRITE YOUR CODE HERE
    Vec3 forwardVec3 = enemy->get_forward_vector();
    Vec3 posVec3 = enemy->get_position();
    GridPos gridPos = terrain->get_grid_position(posVec3);
    Vec2 forwardVec2(forwardVec3.x, forwardVec3.z);
    forwardVec2.Normalize();

    float viewCos = cos(fovAngle * (PI / 180.f));

    int gridSize = terrain->get_map_height();


    // clear all negative value
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (layer.get_value(i, j) < 0.f)
            {
                layer.set_value(i, j, 0.f);
            }
        }
    }

    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (terrain->is_wall(i, j))
            {
                continue;
            }
            Vec3 targetVec3 = terrain->get_world_position(i, j);
            Vec2 targetVec2((targetVec3 - posVec3).x, (targetVec3 - posVec3).y);
            targetVec2.Normalize();
            float dotP = forwardVec2.Dot(targetVec2);

            // if within view angle
            if (dotP > viewCos)
            {
                if (is_clear_path(i, j, gridPos.row, gridPos.col))
                {
                    layer.set_value(i, j, occupancyValue);
                }
            }
            // if within close distance
            else if (Euclidean(i, j, gridPos.row, gridPos.col) < closeDistance && is_clear_path(i, j, gridPos.row, gridPos.col))
            {
                layer.set_value(i, j, occupancyValue);
            }
        }
    }
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    // WRITE YOUR CODE HERE
    int gridSize = terrain->get_map_height();
    float maxValue = std::numeric_limits<float>::min(),
          closeDis = std::numeric_limits<float>::max();
    float dis;
    Vec3 posVec3 = enemy->get_position();
    GridPos gridPos = terrain->get_grid_position(posVec3);
    GridPos target;
    bool Find = false;
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            float currentValue = layer.get_value(i, j);
            if (currentValue > 0.f && currentValue >= maxValue)
            {
                Find = true;
                dis = Euclidean(i, j, gridPos.row, gridPos.col);
                if (currentValue > maxValue)
                {
                    closeDis = dis;
                    maxValue = currentValue;
                    target.row = i, target.col = j;
                }
                else if(closeDis > dis)
                {
                    closeDis = dis;
                    target.row = i, target.col = j;
                }
            }
        }
    }
    if (Find)
    {

        enemy->path_to(terrain->get_world_position(target));
    }
    return Find; // REPLACE THIS
}

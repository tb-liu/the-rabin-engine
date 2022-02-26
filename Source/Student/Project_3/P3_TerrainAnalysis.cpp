#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

const float SmallFloatValue = 0.001f;

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
    float halfCell = gridUnit / 2;
    int xMin = std::min(col0, col1),
        yMin = std::min(row0, row1),
        xMax = std::max(col0, col1),
        yMax = std::max(row0, row1);


    Vec3 worldStart, worldEnd;
    worldStart = terrain->get_world_position(row0, col0);
    worldEnd = terrain->get_world_position(row1, col1);

    Vec2 start2V(worldStart.x, worldStart.z),
         end2V(worldEnd.x, worldEnd.z);
    

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
                if (line_intersect(start2V, end2V, topLeft - Vec2(SmallFloatValue,0.f), topRight + Vec2(SmallFloatValue, 0.f)))
                {
                    return true;
                }
                // if intersect with left edge of this cell 
                else if (line_intersect(start2V, end2V, topLeft + Vec2(0.f, SmallFloatValue), bottomLeft - Vec2(0.f, SmallFloatValue)))
                {
                    return true;
                }
                // if intersect with bottom edge of this cell 
                else if (line_intersect(start2V, end2V, bottomLeft - Vec2(SmallFloatValue, 0.f), Vec2(SmallFloatValue, 0.f)))
                {
                    return true;
                }
                // if intersect with right edge of this cell 
                else if (line_intersect(start2V, end2V, topRight + Vec2(0.f, SmallFloatValue), bottomRight - Vec2(0.f, SmallFloatValue)))
                {
                    return true;
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

            // test if this cell is visible form the other
            for (int targetI = i; targetI < gridSize; ++targetI)
            {
                for (int  targetJ = j + 1; targetJ < gridSize; ++targetJ)
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
            }
            // calculate the visible value
            temp = layer.get_value(i, j) + static_cast<float>(count);
            temp = std::clamp(temp / 160.f, 0.f, 1.f);
            layer.set_value(i, j, temp);
        }
    }
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
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE
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

    return false; // REPLACE THIS
}

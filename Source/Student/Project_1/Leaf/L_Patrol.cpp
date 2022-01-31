#include <pch.h>
#include "L_Patrol.h"
#include "Agent/BehaviorAgent.h"

void L_Patrol::on_enter()
{
    if (RNG::coin_toss())
    {
        // move to center
        targetPoint = Vec3(50,0,50);
    }
    else
    {
        targetPoint = RNG::world_position();
    }

    BehaviorNode::on_leaf_enter();
}

void L_Patrol::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}


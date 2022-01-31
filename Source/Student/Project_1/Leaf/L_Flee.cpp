#include <pch.h>
#include "L_Flee.h"
#include "Agent/BehaviorAgent.h"

void L_Flee::on_enter()
{
    // set animation, speed, etc
    originSpeed = agent->get_movement_speed();
    agent->set_movement_speed(originSpeed * 1.75f);
    targetPoint = RNG::world_position();
    //agent->look_at_point(targetPoint);

	BehaviorNode::on_leaf_enter();
}

void L_Flee::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
    
}

void L_Flee::on_exit()
{
    agent->set_movement_speed(originSpeed);
}

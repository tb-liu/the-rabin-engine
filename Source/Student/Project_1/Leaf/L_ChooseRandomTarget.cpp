#include <pch.h>
#include "L_ChooseRandomTarget.h"
#include "Agent/BehaviorAgent.h"

void L_ChooseRandomTarget::on_enter()
{
    if (agent->get_active_status())
    {
        on_failure();
        return;
    }
    speed = agent->get_movement_speed();
    agent->set_movement_speed(1.2f*speed);
    counter = RNG::range(0.f, 50.f);
    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();
    target = NULL;
    if (allAgents.size() == 0)
    {
        on_failure();
        return;
    }
    if (allAgents.size() == 1)
    {
        target = allAgents[0];
    }
    else
    {

        target = allAgents[RNG::range(0, static_cast<int>(allAgents.size() - 1))];
    }

    BehaviorNode::on_leaf_enter();
}

void L_ChooseRandomTarget::on_update(float dt)
{

    if (!target)
    {
        on_failure();
    }
    else if(Vec3::Distance(agent->get_position(), target->get_position()) <= 15.f)
    {
        if (Vec3::Distance(agent->get_position(), target->get_position()) <= 5.f)
        {
            
            on_success();
        }
        else
        {
            counter -= dt;
            if (counter <= 0)
            {
                on_failure();
            }
            agent->move_toward_point(target->get_position(),dt);
        }
        
        
    }
    else
    {
        on_failure();
    }
    display_leaf_text();
}

void L_ChooseRandomTarget::on_exit()
{
    agent->set_movement_speed(speed);
}

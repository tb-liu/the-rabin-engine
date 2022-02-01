#include <pch.h>
#include "L_CommitCrime.h"
#include "Agent/BehaviorAgent.h"

void L_CommitCrime::on_enter()
{
    counter = RNG::range(50.f, 80.f);
    speed = agent->get_movement_speed();
    agent->set_movement_speed(1.75f*speed);
    // find the agent that is the furthest from this one
    float shortestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;
    target = NULL;
    
    Vec3 currPos = agent->get_position();

    const auto& allAgents = agents->get_all_agents_by_type("Civilian");
    if (allAgents.size() == 0)
    {
        on_failure();
    }
    for (const auto& a : allAgents)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            target = a;
        }
        
    }
    

	BehaviorNode::on_leaf_enter();
}

void L_CommitCrime::on_update(float dt)
{
    agent->set_active_status(false);
    if (!target)
    {
        on_failure();
    }
    else if(Vec3::Distance(agent->get_position(), target->get_position()) <= 30.f)
    {
        if (Vec3::Distance(agent->get_position(), target->get_position()) <= 10.f)
        {
            agents->destroy_agent(target);
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

void L_CommitCrime::on_exit()
{
    agent->set_movement_speed(speed);
}

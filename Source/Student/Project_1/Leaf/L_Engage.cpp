#include <pch.h>
#include "L_Engage.h"
#include "Agent/BehaviorAgent.h"

void L_Engage::on_enter()
{
    originSpeed = agent->get_movement_speed();
    agent->set_movement_speed(2.f * originSpeed);
    
    BehaviorNode::on_leaf_enter();
}

void L_Engage::on_update(float dt)
{
    float shortestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;
    target = NULL;

    Vec3 currPos = agent->get_position();

    const auto& allAgents = agents->get_all_agents_by_type("Criminal");
    if (allAgents.size() == 0)
    {
        agent->set_active_status(false);
        on_failure();
        return;
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
    if (!target)
    {
        agent->set_active_status(false);
        on_failure();
        return;
    }
    const auto result = agent->move_toward_point(target->get_position(), dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

void L_Engage::on_exit()
{
    agent->set_movement_speed(originSpeed);
}

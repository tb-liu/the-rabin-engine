#include <pch.h>
#include "D_PoliceInRange.h"

D_PoliceInRange::D_PoliceInRange():Range(5.f)
{}

void D_PoliceInRange::on_enter()
{
    // find the agent that is the furthest from this one
    float shortestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;


    Vec3 currPos = agent->get_position();

    const auto& allAgents = agents->get_all_agents_by_type("Police");
    if (allAgents.size() == 0)
    {
        BehaviorNode::on_leaf_enter();
        return;
    }
    for (const auto& a : allAgents)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < shortestDistance)
        {
            shortestDistance = distance;
        }

    }

    if (shortestDistance > Range)
    {
        BehaviorNode::on_enter();
       
        return;
    }

    
    on_failure();
}

void D_PoliceInRange::on_update(float dt)
{

    
    BehaviorNode *child = children.front();

    child->tick(dt);

    // assume same status as child
    set_status(child->get_status());
    set_result(child->get_result());
    
    display_leaf_text();
}

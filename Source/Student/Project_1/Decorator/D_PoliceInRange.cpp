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

    const auto& allAgents = agents->get_all_agents_by_type("Enemie");
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
        }

    }

    if (shortestDistance > Range)
    {
        on_failure();
    }

    BehaviorNode::on_leaf_enter();
}

//void D_IfInRange::on_update(float dt)
//{
//    delay -= dt;
//
//    if (delay < 0.0f)
//    {
//        BehaviorNode *child = children.front();
//
//        child->tick(dt);
//
//        // assume same status as child
//        set_status(child->get_status());
//        set_result(child->get_result());
//    }
//}

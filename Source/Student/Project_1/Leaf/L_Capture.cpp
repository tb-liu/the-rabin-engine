#include <pch.h>
#include "L_Capture.h"
#include "Agent/BehaviorAgent.h"

void L_Capture::on_enter()
{
    
    // find the agent that is the furthest from this one
    float shortestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;
    target = NULL;
    
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
            target = a;
        }
        
    }
    

	BehaviorNode::on_leaf_enter();
}

void L_Capture::on_update(float dt)
{
    if (!target)
    {
        on_failure();
    }
    else
    {
        agents->destroy_agent(target);
        agent->set_active_status(false);
        on_success();
    }
    
    display_leaf_text();
}

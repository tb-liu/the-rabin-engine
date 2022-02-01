#include <pch.h>
#include "L_Exam.h"
#include "Agent/BehaviorAgent.h"

void L_Exam::on_enter()
{
    
    // find the agent that is the furthest from this one
    float shortestDistance = std::numeric_limits<float>().max();
    Vec3 furthestPoint;
    target = NULL;
    
    Vec3 currPos = agent->get_position();

    const auto& allAgents = agents->get_all_agents();
    if (allAgents.size() == 0)
    {
        agent->set_active_status(false);
        on_failure();
    }
    for (const auto& a : allAgents)
    {
        if (a == agent)
        {
            continue;
        }
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            target = a;
        }
        
    }
    shortestDistance = std::numeric_limits<float>().max();
    const auto& allCriminals = agents->get_all_agents_by_type("Criminal");
    if (allCriminals.size() == 0)
    {
        agent->set_active_status(false);
        on_failure();
    }
    for (const auto& a : allCriminals)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            cloestCriminal = a;
        }

    }

	BehaviorNode::on_leaf_enter();
}

void L_Exam::on_update(float dt)
{
    if (!target)
    {
        on_failure();
        return;
    }
    else
    {
        if (target != cloestCriminal)
        {
            on_failure();
        }
        else 
        { 
            on_success(); 
        }
    }
    
    display_leaf_text();
}

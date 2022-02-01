#include <pch.h>
#include "L_Alarm.h"
#include "Agent/BehaviorAgent.h"

void L_Alarm::on_enter()
{
    targets.clear();
    // find the agent that is the furthest from this one
    float alarmRange = 15.f;


    
    Vec3 currPos = agent->get_position();

    const auto& allPolice = agents->get_all_agents_by_type("Police");
    const auto& allCivilian = agents->get_all_agents_by_type("Civilian");

    if (/*allPolice.size() == 0 &&*/ allCivilian.size() == 0)
    {
        on_failure();
    }
    for (const auto& a : allPolice)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < alarmRange * 1.5f)
        {
            targets.push_back(a);
        }
        
    }
    for (const auto& a : allCivilian)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < alarmRange)
        {
            targets.push_back(a);
        }

    }

	BehaviorNode::on_leaf_enter();
}

void L_Alarm::on_update(float dt)
{
    if (targets.size() == 0)
    {
        on_failure();
    }
    else
    {
        for (const auto & a : targets)
        {
            a->set_active_status(true);
        }
        on_success();
    }
    
    display_leaf_text();
}

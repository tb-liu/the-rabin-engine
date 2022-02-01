#include <pch.h>
#include "L_MoveToPolice.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToPolice::on_enter()
{
    
    Vec3 currPos = agent->get_position();
    std::vector<Agent*> validTargets;
    const auto& allAgents = agents->get_all_agents_by_type("Police");
    if (allAgents.size() == 0)
    {
        on_failure();
    }
    for (const auto& a : allAgents)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < 40.f)
        {
            validTargets.push_back(a);
        }

    }

    if (validTargets.size() == 0)
    {

        on_failure();
        return;
    }

    int targetIndex = 0;
    if (validTargets.size() > 1)
    {
        targetIndex = RNG::range(0, static_cast<int>(validTargets.size() - 1));
    }
    target = allAgents[targetIndex];
	BehaviorNode::on_leaf_enter();
}

void L_MoveToPolice::on_update(float dt)
{
    originSpeed = agent->get_movement_speed();
    agent->set_movement_speed(1.5f * originSpeed);
    const auto result = agent->move_toward_point(target->get_position(), dt);

    // check the distance between civilian and police
    if (Vec3::Distance(agent->get_position(), target->get_position()) <= 10.f)
    {
        on_success();
    }
    /*else
    {
        on_failure();
    }*/
    agent->set_movement_speed(originSpeed);
    display_leaf_text();
}



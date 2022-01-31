#include <pch.h>
#include "L_Engage.h"
#include "Agent/BehaviorAgent.h"

void L_Engage::on_enter()
{
    Vec3 currPos = agent->get_position();
    std::vector<Agent*> validTargets;
    const auto& allAgents = agents->get_all_agents_by_type("Enemie");
    if (allAgents.size() == 0)
    {
        on_failure();
    }
    for (const auto& a : allAgents)
    {
        const auto& agentPos = a->get_position();
        const float distance = Vec3::Distance(currPos, agentPos);

        if (distance < 20.f)
        {
            validTargets.push_back(a);
        }

    }

    if (validTargets.size() == 0)
    {
        on_failure();
    }
    // set animation, speed, etc
    this->agent->get_behavior_tree().get_tree_name();
    //agent->look_at_point(targetPoint);
    int targetIndex = RNG::range(0, static_cast<int>(validTargets.size() - 1));
    target = allAgents[targetIndex];
	BehaviorNode::on_leaf_enter();
}

void L_Engage::on_update(float dt)
{

    const auto result = agent->move_toward_point(target->get_position(), dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

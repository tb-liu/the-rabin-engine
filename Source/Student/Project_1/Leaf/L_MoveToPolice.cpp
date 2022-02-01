#include <pch.h>
#include "L_MoveToPolice.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToPolice::on_enter()
{
    originSpeed = agent->get_movement_speed();
    agent->set_movement_speed(1.5f * originSpeed);
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

        if (distance < 20.f)
        {
            validTargets.push_back(a);
        }

    }

    if (validTargets.size() == 0)
    {

        on_failure();
        return;
    }
    // set animation, speed, etc
    this->agent->get_behavior_tree().get_tree_name();
    //agent->look_at_point(targetPoint);
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

    const auto result = agent->move_toward_point(target->get_position(), dt);

    // check the distance between civilian and police
    if (result == true && Vec3::Distance(agent->get_position(), target->get_position()) <= 10.f)
    {
        on_success();
    }
    /*else
    {
        on_failure();
    }*/

    display_leaf_text();
}

void L_MoveToPolice::on_exit()
{
    agent->set_movement_speed(originSpeed);
}

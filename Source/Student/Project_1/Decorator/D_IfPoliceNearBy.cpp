#include <pch.h>
#include "L_SeekTarget.h"
#include "Agent/BehaviorAgent.h"
L_SeekTarget::L_SeekTarget(): validTargets(), senseRange(20.f)
{

}
void L_SeekTarget::on_enter()
{
    // set animation, speed, etc

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

        if (distance < senseRange)
        {
            validTargets.push_back(a);
        }

    }

    if (validTargets.size() == 0)
    {
        on_failure();
    }

	BehaviorNode::on_leaf_enter();
}

void L_SeekTarget::on_update(float dt)
{
    on_success();

    display_leaf_text();
}

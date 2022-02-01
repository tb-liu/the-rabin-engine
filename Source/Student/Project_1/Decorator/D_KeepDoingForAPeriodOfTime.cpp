#include <pch.h>
#include "D_KeepDoingForAPeriodOfTime.h"

D_KeepDoingForAPeriodOfTime::D_KeepDoingForAPeriodOfTime():counter(0)
{
    counter = RNG::range(15.f, 30.f);
}

void D_KeepDoingForAPeriodOfTime::on_enter()
{
    
    BehaviorNode::on_enter();
}

void D_KeepDoingForAPeriodOfTime::on_update(float dt)
{
    
    BehaviorNode *child = children.front();

    child->tick(dt);
    counter -= dt;
    if (counter <= 0)
    {
        on_success();
        child->set_status(NodeStatus::EXITING);

        return;
    }
    
    else if (child->succeeded() && counter > 0)
    {
        child->set_status(NodeStatus::READY);
    }

    display_leaf_text();
    
}

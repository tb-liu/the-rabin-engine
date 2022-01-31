#include <pch.h>
#include "D_KeepDoingForAPeriodOfTime.h"

D_KeepDoingForAPeriodOfTime::D_KeepDoingForAPeriodOfTime():counter(0)
{}

void D_KeepDoingForAPeriodOfTime::on_enter()
{
    counter = 200;
    BehaviorNode::on_enter();
}

void D_KeepDoingForAPeriodOfTime::on_update(float dt)
{
    // repeat the child until 4 successes, stop on any failure
    BehaviorNode *child = children.front();

    child->tick(dt);

    if (child->succeeded() == true)
    {
        counter -= dt;

        if (counter <= 0)
        {
            on_success();
        }
        else
        {
            child->set_status(NodeStatus::READY);
        }
    }
    else if (child->failed() == true)
    {
        on_failure();
    }
}

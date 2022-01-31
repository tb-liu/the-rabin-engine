#include <pch.h>
#include "D_KeepTryingUntilFourTimesOrSuccess.h"

D_KeepTryingUntilFourTimesOrSuccess::D_KeepTryingUntilFourTimesOrSuccess() : counter(0)
{}

void D_KeepTryingUntilFourTimesOrSuccess::on_enter()
{
    counter = 0;
    BehaviorNode::on_enter();
}

void D_KeepTryingUntilFourTimesOrSuccess::on_update(float dt)
{
    // repeat the child until 4 fail, stop on any success
    BehaviorNode *child = children.front();

    child->tick(dt);

    if (child->failed())
    {
        ++counter;

        if (counter == 4)
        {
            on_failure();
        }
        else
        {
            child->set_status(NodeStatus::READY);
        }
    }
    else if (child->succeeded())
    {
        on_success();
    }
}

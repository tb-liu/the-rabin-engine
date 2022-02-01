#include <pch.h>
#include "D_KeepTryingUntilFourTimesOrSuccess.h"

D_KeepTryingUntilFourTimesOrSuccess::D_KeepTryingUntilFourTimesOrSuccess() : counter(80.f)
{}

void D_KeepTryingUntilFourTimesOrSuccess::on_enter()
{
    counter = RNG::range(50.f, 100.f);
    BehaviorNode::on_enter();
}

void D_KeepTryingUntilFourTimesOrSuccess::on_update(float dt)
{
    // repeat the child until 4 fail, stop on any success
    BehaviorNode *child = children.front();
    counter -= dt;
    if (counter < 0)
    {
        on_failure();
        agent->set_active_status(false);
        child->set_status(NodeStatus::EXITING);
    }
    child->tick(dt);

    if (child->succeeded())
    {
        on_success();
    }
    else if (child->failed())
    {
        child->set_status(NodeStatus::READY);
    }
    display_leaf_text();
}

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
    }
    child->tick(dt);

    

    /*if (child->failed())
    {
        ++counter;
        
        if (counter == 4)
        {
            
        }
        else
        {
            child->set_status(NodeStatus::READY);
        }
    }*/
    if (child->succeeded())
    {
        on_success();
    }
}

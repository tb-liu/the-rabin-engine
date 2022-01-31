#include <pch.h>
#include "D_RepeatDoing.h"

D_RepeatDoing::D_RepeatDoing() : delay(0.0f)
{}

void D_RepeatDoing::on_enter()
{
    delay = RNG::range(1.0f, 2.0f);

    BehaviorNode::on_enter();
}

void D_RepeatDoing::on_update(float dt)
{
    delay -= dt;

    if (delay < 0.0f)
    {
        BehaviorNode *child = children.front();

        child->tick(dt);

        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
}

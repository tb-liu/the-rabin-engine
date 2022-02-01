#include <pch.h>
#include "L_CoolDown.h"
#include "Agent/BehaviorAgent.h"

void L_CoolDown::on_enter()
{
    counter = 5.f;
	BehaviorNode::on_leaf_enter();
}

void L_CoolDown::on_update(float dt)
{
    counter -= dt;
    if (counter <= 0)
    {
        on_success();
    }
    
    display_leaf_text();
}

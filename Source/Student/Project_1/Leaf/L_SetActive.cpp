#include <pch.h>
#include "L_SetActive.h"
#include "Agent/BehaviorAgent.h"

void L_SetActive::on_enter()
{
	
	BehaviorNode::on_leaf_enter();
}

void L_SetActive::on_update(float dt)
{
	agent->set_active_status(true);
	on_success();
    display_leaf_text();
}

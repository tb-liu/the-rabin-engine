#include <pch.h>
#include "L_IfActive.h"
#include "Agent/BehaviorAgent.h"

void L_IfActive::on_enter()
{
	
	BehaviorNode::on_leaf_enter();
}

void L_IfActive::on_update(float dt)
{
	if (agent->get_active_status())
	{
		on_success();
		
	}
	else
	{
		on_failure();
	}
	

    display_leaf_text();
}

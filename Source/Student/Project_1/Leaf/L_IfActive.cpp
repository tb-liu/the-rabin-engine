#include <pch.h>
#include "L_IfActive.h"
#include "Agent/BehaviorAgent.h"

void L_IfActive::on_enter()
{
	if (agent->get_active_status())
	{
		on_success();
		return;
	}
	on_failure();
}

void L_IfActive::on_update(float dt)
{
    display_leaf_text();
}

#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Alarm : public BaseNode<L_Alarm>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Agent * target;
};
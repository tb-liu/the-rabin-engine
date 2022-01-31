#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Capture : public BaseNode<L_Capture>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Agent * target;
};
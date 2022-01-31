#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Report : public BaseNode<L_Report>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Agent * target;
};
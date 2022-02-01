#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Engage : public BaseNode<L_Engage>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;
private:
    Agent* target;
    float originSpeed;
};
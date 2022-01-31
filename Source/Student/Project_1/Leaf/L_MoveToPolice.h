#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_MoveToPolice : public BaseNode<L_MoveToPolice>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;
private:
    Agent* target;
    float originSpeed;
};
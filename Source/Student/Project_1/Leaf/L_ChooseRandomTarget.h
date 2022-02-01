#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"
#include <vector>
class L_ChooseRandomTarget : public BaseNode<L_ChooseRandomTarget>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;
private:
    float counter;
    float speed;
    Agent * target;
};
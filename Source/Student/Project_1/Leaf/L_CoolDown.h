#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CoolDown : public BaseNode<L_CoolDown>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float counter;
};
#pragma once
#include "BehaviorNode.h"

class D_KeepDoingForAPeriodOfTime : public BaseNode<D_KeepDoingForAPeriodOfTime>
{
public:
    D_KeepDoingForAPeriodOfTime();

protected:
    float counter;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};
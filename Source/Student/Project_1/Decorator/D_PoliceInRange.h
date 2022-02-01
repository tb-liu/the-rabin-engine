#pragma once
#include "BehaviorNode.h"

class D_PoliceInRange : public BaseNode<D_PoliceInRange>
{
    
public:
    D_PoliceInRange();

protected:

    float Range;
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};
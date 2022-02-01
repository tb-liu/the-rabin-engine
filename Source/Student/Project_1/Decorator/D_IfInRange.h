#pragma once
#include "BehaviorNode.h"

class D_IfInRange : public BaseNode<D_IfInRange>
{
    
public:
    D_IfInRange();

protected:

    float Range;
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};
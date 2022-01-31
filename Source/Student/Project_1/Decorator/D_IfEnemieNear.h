#pragma once
#include "BehaviorNode.h"

class D_IfEnemieNear : public BaseNode<D_IfEnemieNear>
{
    
public:
    D_IfEnemieNear();

protected:

    float Range;
    virtual void on_enter() override;
    //virtual void on_update(float dt) override;
};
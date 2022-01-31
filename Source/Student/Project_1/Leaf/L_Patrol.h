#pragma once
#include "BehaviorNode.h"

class L_Patrol : public BaseNode<L_Patrol>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};
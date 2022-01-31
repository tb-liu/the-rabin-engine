#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Flee : public BaseNode<L_Flee>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override;
private:
    float originSpeed;
    Vec3 targetPoint;
};
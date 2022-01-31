#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"
#include <vector>
class L_SeekTarget : public BaseNode<L_SeekTarget>
{

protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float senseRange;
    std::vector<Agent*> validTargets;
};
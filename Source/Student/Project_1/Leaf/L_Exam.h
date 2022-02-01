#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Exam : public BaseNode<L_Exam>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Agent * target;
    Agent* cloestCriminal;
};
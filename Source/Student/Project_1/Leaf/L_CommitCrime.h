#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_CommitCrime : public BaseNode<L_CommitCrime>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Agent * target;
};
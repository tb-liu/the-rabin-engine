#pragma once
#include "BehaviorNode.h"

class D_RepeatDoing : public BaseNode<D_RepeatDoing>
{
public:
    D_RepeatDoing();

protected:
    float delay;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};
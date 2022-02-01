#pragma once
#include "BehaviorNode.h"

class D_KeepTryingUntilFourTimesOrSuccess : public BaseNode<D_KeepTryingUntilFourTimesOrSuccess>
{
public:
    D_KeepTryingUntilFourTimesOrSuccess();

protected:
    float counter;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};
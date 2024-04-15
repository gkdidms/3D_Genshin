#pragma once
#include "State.h"
class CStateIdle :
    public CState
{
private:
    CStateIdle();
    virtual ~CStateIdle() = default;

public:
    // CState을(를) 통해 상속됨
    virtual PLAYER_STATE Enter() override;
    virtual PLAYER_STATE Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateIdle* Create();
    virtual void Free() override;
};


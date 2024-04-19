#pragma once
#include "State.h"
class CStateJump :
    public CState
{
private:
    CStateJump();
    virtual ~CStateJump();

public:
    virtual PLAYER_STATE Enter(PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateJump* Create();
    virtual void Free();
};


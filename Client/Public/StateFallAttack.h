#pragma once
#include "State.h"
class CStateFallAttack :
    public CState
{
private:
    CStateFallAttack();
    virtual ~CStateFallAttack() = default;

public:
    virtual PLAYER_STATE Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateFallAttack* Create();
    virtual void Free() override;
};


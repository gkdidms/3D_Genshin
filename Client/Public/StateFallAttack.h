#pragma once
#include "State.h"
class CStateFallAttack :
    public CState
{
private:
    CStateFallAttack();
    virtual ~CStateFallAttack() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateFallAttack* Create();
    virtual void Free() override;
};


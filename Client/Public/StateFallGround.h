#pragma once
#include "State.h"
class CStateFallGround :
    public CState
{
private:
    CStateFallGround();
    virtual ~CStateFallGround() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateFallGround* Create();
    virtual void Free() override;
};


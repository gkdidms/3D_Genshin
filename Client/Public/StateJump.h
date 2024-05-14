#pragma once
#include "State.h"
class CStateJump :
    public CState
{
private:
    CStateJump();
    virtual ~CStateJump() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    virtual PLAYER_STATE ToIdle(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToRun(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToSprint(class CStateManager& pStateManager, PLAYER_STATE CurrentState);

public:
    static CStateJump* Create();
    virtual void Free();
};


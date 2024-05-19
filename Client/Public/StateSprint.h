#pragma once
#include "State.h"

#include "Client_Defines.h"

BEGIN(Client)
class CStateSprint :
    public CState
{
private:
    CStateSprint();
    virtual ~CStateSprint() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _float m_fDuration = { 1.3f };
    _float m_fCurrentTime = { 0.f };

private:
    virtual PLAYER_STATE ToIdle(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToRun(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToSprint(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToJump(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToAttack(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToElementalArt(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToElementalBurst(class CStateManager& pStateManager, PLAYER_STATE CurrentState);

public:
    static CStateSprint* Create();
    virtual void Free() override;
};
END
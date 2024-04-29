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
    virtual PLAYER_STATE Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    virtual PLAYER_STATE ToIdle(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToRun(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToSprint(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToJump(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToAttack(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToElementalArt(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToElementalBurst(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);

public:
    static CStateSprint* Create();
    virtual void Free() override;
};
END
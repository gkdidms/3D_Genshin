#pragma once
#include "State.h"

BEGIN(Client)
class CStateIdle :
    public CState
{
private:
    CStateIdle();
    virtual ~CStateIdle() = default;

public:
    // CState을(를) 통해 상속됨
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateIdle* Create();
    virtual void Free() override;
};

END
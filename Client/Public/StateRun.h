#pragma once
#include "State.h"

BEGIN(Client)
class CStateRun :
    public CState
{
private:
    CStateRun();
    virtual ~CStateRun() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateRun* Create();
    virtual void Free() override;
};

END

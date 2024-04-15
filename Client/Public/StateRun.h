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
    virtual PLAYER_STATE Enter() override;
    virtual PLAYER_STATE Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateRun* Create();
    virtual void Free() override;
};

END

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
    virtual PLAYER_STATE Enter() override;
    virtual PLAYER_STATE Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateSprint* Create();
    virtual void Free() override;
};
END
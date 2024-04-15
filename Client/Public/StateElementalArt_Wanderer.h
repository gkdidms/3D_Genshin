#pragma once
#include "State.h"

#include "Client_Defines.h"

BEGIN(Client)
class CStateElementalArt_Wanderer :
    public CState
{
private:
    CStateElementalArt_Wanderer();
    virtual ~CStateElementalArt_Wanderer() = default;

public:
    PLAYER_STATE Enter() override;
    PLAYER_STATE Update(CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    PLAYER_STATE Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateElementalArt_Wanderer* Create();
    virtual void Free() override;
};
END

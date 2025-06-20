#pragma once
#include "State.h"
#include "Client_Defines.h"

BEGIN(Client)
class CStateElementalArt_Tighanri :
    public CState
{
private:
    CStateElementalArt_Tighanri();
    virtual ~CStateElementalArt_Tighanri() = default;

public:
    PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    PLAYER_STATE Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    PLAYER_STATE Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateElementalArt_Tighanri* Create();
    virtual void Free() override;
};

END
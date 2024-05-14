#pragma once
#include "State.h"
BEGIN(Client)
class CStateElementalBurst :
    public CState
{
private:
    CStateElementalBurst();
    virtual ~CStateElementalBurst() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateElementalBurst* Create();
    virtual void Free() override;
};
END

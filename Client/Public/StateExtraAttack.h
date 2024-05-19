#pragma once
#include "State.h"

BEGIN(Client)
class CStateExtraAttack :
    public CState
{
private:
    CStateExtraAttack();
    virtual ~CStateExtraAttack() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState);

public:
    static CStateExtraAttack* Create();
    virtual void Free() override;
};
END

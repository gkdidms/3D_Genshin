#pragma once
#include "State.h"

BEGIN(Client)
class CStateHit :
    public CState
{
private:
    CStateHit();
    virtual ~CStateHit() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateHit* Create();
    virtual void Free() override;
};
END

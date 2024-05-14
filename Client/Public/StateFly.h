#pragma once
#include "State.h"

BEGIN(Client)
class CStateFly :
    public CState
{
private:
    CStateFly();
    virtual ~CStateFly() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState);

public:
    static CStateFly* Create(); 
    virtual void Free() override;
};

END
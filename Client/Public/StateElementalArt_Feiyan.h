#pragma once
#include "State.h"

BEGIN(Client)
class CStateElementalArt_Feiyan :
    public CState
{
private:
    CStateElementalArt_Feiyan();
    virtual ~CStateElementalArt_Feiyan() = default;

public:
    HRESULT Initialize();

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState);

public:
    static CStateElementalArt_Feiyan* Create();
    virtual void Free() override;
};

END
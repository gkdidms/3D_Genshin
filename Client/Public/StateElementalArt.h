#pragma once
#include "State.h"

BEGIN(Client)
class CStateElementalArt :
    public CState
{
private:
    CStateElementalArt();
    virtual ~CStateElementalArt() = default;

public:
    HRESULT Initialize();

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState);

private:
    vector<CState*> m_ElemenetalArt;

public:
    static CStateElementalArt* Create();
    virtual void Free() override;
};

END